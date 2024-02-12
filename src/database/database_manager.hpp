/**
 * @file    database_manager.hpp
 * @brief   This file contains the declaration of the DatabaseManager class.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "../utilities/metadata.hpp"

namespace database {

class DatabaseManager {
private:
    sqlite3* db_;
    std::string db_name_;

    /**
     * @brief A member function that open the database.
     * @return True if the database is opened successfully, false otherwise.
     */
    bool open();

    /**
     * @brief A member function that creates the tables if they do not exist.
     * @return True if the tables are created successfully, false otherwise.
     */
    bool createTablesIfNeeded();

    /**
     * @brief A member function that enables foreign keys.
     * @return True if the foreign keys are enabled successfully, false otherwise.
     */
    bool enableForeignKeys();

    /**
     * @brief A member function that executes the given statement.
     * @param stmt The statement to be executed.
     * @return True if the statement is executed successfully, false otherwise.
     */
    bool executeStatement(sqlite3_stmt* stmt);

public:

    /**
     * @brief A constructor for the DatabaseManager class.
     * @param db_name The name of the database.
     */
    DatabaseManager(const std::string& db_name);

    /**
     * @brief A destructor for the DatabaseManager class.
     */
    ~DatabaseManager();

    /**
     * @brief A member function that initializes the database.
     *        It opens the database, creates the tables if they do not exist and enables foreign keys.
     * @return True if the database is initialized successfully, false otherwise.
     */
    void init();

    /**
     * @brief A member function that closes the database.
     */
    void close();

    /**
     * @brief A member function that adds a device to the database.
     * @param device The device to be added.
     * @return True if the device is added successfully, false otherwise.
     */
    bool addDevice(const Device& device);

    /**
     * @brief A member function that updates a device in the database.
     * @param device The device to be updated.
     * @return True if the device is updated successfully, false otherwise.
     */
    bool updateDevice(const Device& device);

    /**
     * @brief A member function that deletes a device from the database.
     * @param id The id of the device to be deleted.
     * @return True if the device is deleted successfully, false otherwise.
     */
    bool deleteDevice(int id);

    /**
     * @brief A member function that gets a device from the database.
     * @param id The id of the device to be retrieved.
     * @return The device if it is retrieved successfully, an empty optional otherwise.
     */
    std::optional<Device> getDevice(int id);

    /**
     * @brief A member function that gets all devices from the database.
     * @return A vector of devices if they are retrieved successfully, an empty vector otherwise.
     */
    std::vector<Device> getAllDevices();

    /**
     * @brief A member function that gets all devices from the database with the given filters.
     * @param name The name of the device.
     * @param type The type of the device.
     * @param serial_number The serial number of the device.
     * @param creation_date_start The start date of the creation date of the device.
     * @param creation_date_end The end date of the creation date of the device.
     * @param location The location of the device.
     * @return A vector of devices if they are retrieved successfully, an empty vector otherwise.
     */
    std::vector<Device> getDevicesWithFilters(const std::string& name, const std::string& type, 
                                              const std::string& serial_number, const std::string& creation_date_start, 
                                              const std::string& creation_date_end, const std::string& location);
    
    /**
     * @brief A member function that gets a location from the database.
     * @param id The id of the location to be retrieved.
     * @return The location if it is retrieved successfully, an empty optional otherwise.
     */
    std::optional<Location> getLocation(int id);

    /**
     * @brief A member function that gets all locations from the database.
     * @return A vector of locations if they are retrieved successfully, an empty vector otherwise.
     */
    std::vector<Location> getAllLocations();

    /**
     * @brief A member function that adds a location to the database.
     * @param location The location to be added.
     * @return True if the location is added successfully, false otherwise.
     */
    bool addLocation(const Location& location);

    /**
     * @brief A member function that updates a location in the database.
     * @param location The location to be updated.
     * @return True if the location is updated successfully, false otherwise.
     */
    bool updateLocation(const Location& location);

    /**
     * @brief A member function that deletes a location from the database.
     * @param id The id of the location to be deleted.
     * @return True if the location is deleted successfully, false otherwise.
     */
    bool deleteLocation(int id);
};

} // namespace database

#endif // DATABASE_MANAGER_HPP
