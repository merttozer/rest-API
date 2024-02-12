/**
 * @file    server_manager.cpp
 * @brief   This file contains the implementation of the ServerManager class.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#include <jsoncpp/json/json.h>
#include "../utilities/http_status_codes.hpp"
#include "server_manager.hpp"

namespace server {

ServerManager::ServerManager(const std::string& db_path, const std::string& host, int port, int concurrency_capacity)
    : concurrency_capacity_(concurrency_capacity)
    , mux_()
    , database_(std::make_unique<database::DatabaseManager>(db_path))
    , server_(std::make_unique<served::net::server>(host, std::to_string(port), mux_)){}

ServerManager::~ServerManager() {
    stop(); // Stop server
    database_->close();  // Close database connection
}

void ServerManager::start() {
    std::cout << "Server Manager is starting..." << std::endl;
    server_->run(concurrency_capacity_);
}

void ServerManager::stop() {
    if (server_) {
        server_->stop();
    }
}

void ServerManager::init() {
    database_->init(); // Initialize database
    initDeviceRoutes(); // Initialize routes
    initLocationRoutes(); // Initialize routes
}

void ServerManager::initDeviceRoutes() {
    mux_.handle("/devices/{id}")
        .get(std::bind(&ServerManager::handleGetDevice, this, std::placeholders::_1, std::placeholders::_2))
        .put(std::bind(&ServerManager::handleUpdateDevice, this, std::placeholders::_1, std::placeholders::_2))
        .del(std::bind(&ServerManager::handleDeleteDevice, this, std::placeholders::_1, std::placeholders::_2))
        .post(std::bind(&ServerManager::handleNotAllowed, this, std::placeholders::_1, std::placeholders::_2));

    mux_.handle("/devices")
        .get(std::bind(&ServerManager::handleGetDevices, this, std::placeholders::_1, std::placeholders::_2))
        .post(std::bind(&ServerManager::handleAddDevice, this, std::placeholders::_1, std::placeholders::_2))
        .put(std::bind(&ServerManager::handleNotAllowed, this, std::placeholders::_1, std::placeholders::_2))
        .del(std::bind(&ServerManager::handleNotAllowed, this, std::placeholders::_1, std::placeholders::_2));
}

void ServerManager::initLocationRoutes() {
    mux_.handle("/locations/{id}")
        .get(std::bind(&ServerManager::handleGetLocation, this, std::placeholders::_1, std::placeholders::_2))
        .put(std::bind(&ServerManager::handleUpdateLocation, this, std::placeholders::_1, std::placeholders::_2))
        .del(std::bind(&ServerManager::handleDeleteLocation, this, std::placeholders::_1, std::placeholders::_2))
        .post(std::bind(&ServerManager::handleNotAllowed, this, std::placeholders::_1, std::placeholders::_2));
    
    mux_.handle("/locations")
        .get(std::bind(&ServerManager::handleGetAllLocations, this, std::placeholders::_1, std::placeholders::_2))
        .post(std::bind(&ServerManager::handleAddLocation, this, std::placeholders::_1, std::placeholders::_2))
        .put(std::bind(&ServerManager::handleNotAllowed, this, std::placeholders::_1, std::placeholders::_2))
        .del(std::bind(&ServerManager::handleNotAllowed, this, std::placeholders::_1, std::placeholders::_2));
}

void ServerManager::handleGetDevice(served::response &res, const served::request &req) {
    int id = std::stoi(req.params["id"]);
    auto optionalDevice = database_->getDevice(id);
    if (optionalDevice.has_value()) {
        Device device = optionalDevice.value();
        Json::Value jsonDevice;
        jsonDevice["id"] = device.id;
        jsonDevice["name"] = device.name;
        jsonDevice["type"] = device.type;
        jsonDevice["serial_number"] = device.serial_number;
        jsonDevice["creation_date"] = device.creation_date;
        jsonDevice["location_id"] = device.location_id;
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body(jsonDevice.toStyledString());
    } else {
        res.set_status(HttpStatus::NOT_FOUND);
        res.set_body("{\"error\": \"Device not found.\"}\n");
    }
}

void ServerManager::handleUpdateDevice(served::response &res, const served::request &req) {
    int id = std::stoi(req.params["id"]);
    Json::Value jsonRequest;
    std::istringstream(req.body()) >> jsonRequest;
    Device updatedDevice;
    updatedDevice.id = id;
    updatedDevice.name = jsonRequest["name"].asString();
    updatedDevice.type = jsonRequest["type"].asString();
    updatedDevice.serial_number = jsonRequest["serial_number"].asString();
    updatedDevice.creation_date = jsonRequest["creation_date"].asString();
    updatedDevice.location_id = jsonRequest["location_id"].asInt();
    if (database_->updateDevice(updatedDevice)) {
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"Device updated successfully.\"}\n");
    } else {
        res.set_status(HttpStatus::INTERNAL_SERVER_ERROR);
        res.set_body("{\"error\": \"Failed to update device.\"}\n");
    }
}

void ServerManager::handleDeleteDevice(served::response &res, const served::request &req) {
    int id = std::stoi(req.params["id"]);
    if (database_->deleteDevice(id)) {
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"Device deleted successfully.\"}\n");
    } else {
        res.set_status(HttpStatus::NOT_FOUND);
        res.set_body("{\"error\": \"Device not found.\"}\n");
    }
}

void ServerManager::handleGetDevices(served::response &res, const served::request &req) {
    // Check for the presence of any filter query parameters
    bool hasFilters = false;
    for (const auto & query_param : req.query) {
        if (!query_param.second.empty()) {
            hasFilters = true;
            break;
        }
    }
    if (hasFilters) {
        // Call and return the filtered list
        handleGetDevicesWithFilters(res, req);
    } else {
        // Call and return the list of all devices
        handleGetAllDevices(res, req);
    }
}

void ServerManager::handleGetAllDevices(served::response &res, const served::request &req) {
    std::vector<Device> devices = database_->getAllDevices();
    if (devices.empty()) {
        res.set_status(HttpStatus::NO_CONTENT);
    } else {
        Json::Value jsonResponse;
        for (const auto& device : devices) {
            Json::Value jsonDevice;
            jsonDevice["id"] = device.id;
            jsonDevice["name"] = device.name;
            jsonDevice["type"] = device.type;
            jsonDevice["serial_number"] = device.serial_number;
            jsonDevice["creation_date"] = device.creation_date;
            jsonDevice["location_id"] = device.location_id;
            jsonResponse.append(jsonDevice);
        }
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body(jsonResponse.toStyledString());
    }
}

void ServerManager::handleGetDevicesWithFilters(served::response &res, const served::request &req) {
    auto name = req.query.get("name");
    auto type = req.query.get("type");
    auto serial_number = req.query.get("serial_number");
    auto creation_date_start = req.query.get("creation_date_start");
    auto creation_date_end = req.query.get("creation_date_end");
    auto location = req.query.get("location");

    auto devices = database_->getDevicesWithFilters(name, type, serial_number, creation_date_start, creation_date_end, location);
    if(devices.empty()){
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"No devices found.\"}\n");
    }
    else {
        Json::Value jsonResponse;
        for (const auto& device : devices) {
            Json::Value jsonDevice;
            jsonDevice["id"] = device.id;
            jsonDevice["name"] = device.name;
            jsonDevice["type"] = device.type;
            jsonDevice["serial_number"] = device.serial_number;
            jsonDevice["creation_date"] = device.creation_date;
            jsonDevice["location_id"] = device.location_id;
            jsonResponse.append(jsonDevice);
        }
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body(jsonResponse.toStyledString());
    }
}

void ServerManager::handleAddDevice(served::response &res, const served::request &req) {
    Json::Value jsonRequest;
    std::istringstream(req.body()) >> jsonRequest;
    Device newDevice;
    newDevice.name = jsonRequest["name"].asString();
    newDevice.type = jsonRequest["type"].asString();
    newDevice.serial_number = jsonRequest["serial_number"].asString();
    newDevice.creation_date = jsonRequest["creation_date"].asString();
    newDevice.location_id = jsonRequest["location_id"].asInt();
    if (database_->addDevice(newDevice)) {
        res.set_status(HttpStatus::CREATED);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"Device added successfully.\"}\n");
    } else {
        res.set_status(HttpStatus::INTERNAL_SERVER_ERROR);
        res.set_body("{\"error\": \"Failed to add device.\"}\n");
    }
}

void ServerManager::handleGetLocation(served::response &res, const served::request &req) {
    int id = std::stoi(req.params["id"]);
    auto optionalLocation = database_->getLocation(id);
    if (optionalLocation.has_value()) {
        Location location = optionalLocation.value();
        Json::Value jsonLocation;
        jsonLocation["id"] = location.id;
        jsonLocation["name"] = location.name;
        jsonLocation["type"] = location.type;
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body(jsonLocation.toStyledString());
    } else {
        res.set_status(HttpStatus::NOT_FOUND);
        res.set_body("{\"error\": \"Location not found.\"}\n");
    }
}

void ServerManager::handleUpdateLocation(served::response &res, const served::request &req) {
    int id = std::stoi(req.params["id"]);
    Json::Value jsonRequest;
    std::istringstream(req.body()) >> jsonRequest;
    Location updatedLocation;
    updatedLocation.id = id;
    updatedLocation.name = jsonRequest["name"].asString();
    updatedLocation.type = jsonRequest["type"].asString();

    if (database_->updateLocation(updatedLocation)) {
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"Location updated successfully.\"}\n");
    } else {
        res.set_status(HttpStatus::INTERNAL_SERVER_ERROR);
        res.set_body("{\"error\": \"Failed to update location.\"}\n");
    }
}

void ServerManager::handleDeleteLocation(served::response &res, const served::request &req) {
    int id = std::stoi(req.params["id"]);
    if (database_->deleteLocation(id)) {
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"Location deleted successfully.\"}\n");
    } else {
        res.set_status(HttpStatus::NOT_FOUND);
        res.set_body("{\"error\": \"Location not found.\"}\n");
    }
}

void ServerManager::handleGetAllLocations(served::response &res, const served::request &req) {
    std::vector<Location> locations = database_->getAllLocations();
    if (locations.empty()) {
        res.set_status(HttpStatus::NO_CONTENT);
    } else {
        Json::Value jsonResponse;
        for (const auto& location : locations) {
            Json::Value jsonLocation;
            jsonLocation["id"] = location.id;
            jsonLocation["name"] = location.name;
            jsonLocation["type"] = location.type;
            jsonResponse.append(jsonLocation);
        }
        res.set_status(HttpStatus::OK);
        res.set_header("Content-Type", "application/json");
        res.set_body(jsonResponse.toStyledString());
    }
}

void ServerManager::handleAddLocation(served::response &res, const served::request &req) {
    Json::Value jsonRequest;
    std::istringstream(req.body()) >> jsonRequest;
    Location newLocation;
    newLocation.name = jsonRequest["name"].asString();
    newLocation.type = jsonRequest["type"].asString();

    if (database_->addLocation(newLocation)) {
        res.set_status(HttpStatus::CREATED);
        res.set_header("Content-Type", "application/json");
        res.set_body("{\"message\": \"Location added successfully.\"}\n");
    } else {
        res.set_status(HttpStatus::INTERNAL_SERVER_ERROR);
        res.set_body("{\"error\": \"Failed to add location.\"}\n");
    }
}

void ServerManager::handleNotAllowed(served::response &res, const served::request &req) {
    res.set_status(HttpStatus::METHOD_NOT_ALLOWED);
    res.set_body("{\"error\": \"Method not allowed.\"}\n");
}

} // namespace server
