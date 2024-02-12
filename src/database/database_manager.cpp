/**
 * @file    database_manager.cpp
 * @brief   This file contains the implementation of the DatabaseManager class.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#include <iostream>
#include "database_manager.hpp"

namespace database {

DatabaseManager::DatabaseManager(const std::string& db_name) 
    : db_name_(db_name)
    , db_(nullptr) {}

DatabaseManager::~DatabaseManager() {
    close();
}

void DatabaseManager::init() {
    if (!open()) {
        std::cerr << "Failed to open database: " << db_name_ << std::endl;
        return;
    }
    if (!enableForeignKeys()) {
        std::cerr << "Failed to enable foreign keys" << std::endl;
        return;
    }
    if (!createTablesIfNeeded()) {
        std::cerr << "Failed to create tables" << std::endl;
        return;
    }
}

bool DatabaseManager::open() { 
    if (sqlite3_open(db_name_.c_str(), &db_) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    return true;
}


void DatabaseManager::close() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool DatabaseManager::enableForeignKeys() {
    char* errMsg;
    std::string fk_on = "PRAGMA foreign_keys = ON;";
    if (sqlite3_exec(db_, fk_on.c_str(), NULL, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Error setting foreign key pragma: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::executeStatement(sqlite3_stmt* stmt) {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db_) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::createTablesIfNeeded() {
    // SQL statement to create your tables
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS devices (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            type TEXT NOT NULL,
            serial_number TEXT UNIQUE NOT NULL,
            creation_date TEXT NOT NULL,
            location_id INTEGER,
            FOREIGN KEY (location_id) REFERENCES locations(id) ON DELETE RESTRICT
        );
        CREATE TABLE IF NOT EXISTS locations (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            type TEXT NOT NULL
        );
    )";

    // Execute SQL to create tables
    char* errorMessage;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating tables: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool DatabaseManager::addDevice(const Device& device) {
    // SQL statement to insert a new device
    const char* sql = "INSERT INTO Devices (name, type, serial_number, creation_date, location_id) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, device.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, device.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, device.serial_number.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, device.creation_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, device.location_id);

    return executeStatement(stmt);
}

std::optional<Device> DatabaseManager::getDevice(int id) {
    // SQL statement to get a device
    const char* sql = "SELECT * FROM Devices WHERE id = ?;";
    sqlite3_stmt* stmt;
    Device device;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        device.id = sqlite3_column_int(stmt, 0);
        device.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        device.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        device.serial_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        device.creation_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        device.location_id = sqlite3_column_int(stmt, 5);
    } else {
        std::cerr << "No device found with id: " << id << std::endl;
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    sqlite3_finalize(stmt);
    return device;
}

std::vector<Device> DatabaseManager::getAllDevices() {
    // SQL statement to get all devices
    const char* sql = "SELECT * FROM Devices;";
    sqlite3_stmt* stmt;
    std::vector<Device> devices;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return devices;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Device device;
        device.id = sqlite3_column_int(stmt, 0);
        device.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        device.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        device.serial_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        device.creation_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        device.location_id = sqlite3_column_int(stmt, 5);
        devices.push_back(device);
    }

    sqlite3_finalize(stmt);
    return devices;
}

bool DatabaseManager::updateDevice(const Device& device) {
    // SQL statement to update a device
    const char* sql = "UPDATE Devices SET name = ?, type = ?, serial_number = ?, creation_date = ?, location_id = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, device.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, device.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, device.serial_number.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, device.creation_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, device.location_id);
    sqlite3_bind_int(stmt, 5, device.id);

    return executeStatement(stmt);
}

bool DatabaseManager::deleteDevice(int id) {
    // SQL statement to delete a device
    const char* sql = "DELETE FROM Devices WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    return executeStatement(stmt);
}

std::vector<Device> DatabaseManager::getDevicesWithFilters(const std::string& name, const std::string& type, const std::string& serial_number, const std::string& creation_date_start, const std::string& creation_date_end, const std::string& location) {
    // SQL statement to get all devices with filters
    std::string sql = "SELECT devices.* FROM devices INNER JOIN locations ON devices.location_id = locations.id WHERE 1 = 1";
    std::vector<Device> devices;
    if (!name.empty()) sql += " AND devices.name = '" + name + "'";
    if (!type.empty()) sql += " AND devices.type = '" + type + "'";
    if (!serial_number.empty()) sql += " AND devices.serial_number = '" + serial_number + "'";
    if (!creation_date_start.empty()) sql += " AND devices.creation_date >= '" + creation_date_start + "'";
    if (!creation_date_end.empty()) sql += " AND devices.creation_date <= '" + creation_date_end + "'";
    if (!location.empty()) sql += " AND locations.name = '" + location + "'";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db_) << std::endl;
        return devices;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Device device;
        device.id = sqlite3_column_int(stmt, 0);
        device.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        device.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        device.serial_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        device.creation_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        device.location_id = sqlite3_column_int(stmt, 5);
        devices.push_back(device);
    }

    sqlite3_finalize(stmt);
    return devices;
}

bool DatabaseManager::addLocation(const Location& location) {
    // SQL statement to insert a new location
    const char* sql = "INSERT INTO Locations (name, type) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, location.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, location.type.c_str(), -1, SQLITE_STATIC);

    return executeStatement(stmt);
}

std::optional<Location> DatabaseManager::getLocation(int id) {
    // SQL statement to get a location
    const char* sql = "SELECT * FROM Locations WHERE id = ?;";
    sqlite3_stmt* stmt;
    Location location;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        location.id = sqlite3_column_int(stmt, 0);
        location.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        location.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    } else {
        std::cerr << "No location found with id: " << id << std::endl;
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    sqlite3_finalize(stmt);
    return location;
}

std::vector<Location> DatabaseManager::getAllLocations() {
    // SQL statement to get all locations
    const char* sql = "SELECT * FROM Locations;";
    sqlite3_stmt* stmt;
    std::vector<Location> locations;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return locations;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Location location;
        location.id = sqlite3_column_int(stmt, 0);
        location.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        location.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        locations.push_back(location);
    }

    sqlite3_finalize(stmt);
    return locations;
}

bool DatabaseManager::updateLocation(const Location& location) {
    // SQL statement to update a location
    const char* sql = "UPDATE Locations SET name = ?, type = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, location.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, location.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, location.id);

    return executeStatement(stmt);
}

bool DatabaseManager::deleteLocation(int id) {
    // SQL statement to delete a location
    const char* sql = "DELETE FROM Locations WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    return executeStatement(stmt);
}

} // namespace database
