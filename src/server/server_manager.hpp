/**
 * @file    server_manager.hpp
 * @brief   This file contains the declaration of the ServerManager class.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <served/served.hpp>
#include <memory>
#include "../database/database_manager.hpp"

namespace server {

class ServerManager {
private:
    std::unique_ptr<database::DatabaseManager> database_;
    std::unique_ptr<served::net::server> server_;
    served::multiplexer mux_;
    int concurrency_capacity_;

private:
    /**
     * @brief A member function that handle GET method for device/id routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleGetDevice(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle PUT method for device/id routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleUpdateDevice(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle DELETE method for device/id routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleDeleteDevice(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle POST method for device routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleAddDevice(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle GET method for device routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleGetDevices(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle GET method for device routes with filters.
     * @param res The response object.
     * @param req The request object.
     */
    void handleGetDevicesWithFilters(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle GET method for device routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleGetAllDevices(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle GET method for location/id routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleGetLocation(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle PUT method for location/id routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleUpdateLocation(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle DELETE method for location/id routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleDeleteLocation(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle POST method for location routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleAddLocation(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle GET method for location routes.
     * @param res The response object.
     * @param req The request object.
     */
    void handleGetAllLocations(served::response &res, const served::request &req);

    /**
     * @brief A member function that handle not allowed methods.
     * @param res The response object.
     * @param req The request object.
     */
    void handleNotAllowed(served::response &res, const served::request &req);

public:
    /**
     * @brief A constructor for the ServerManager class.
     * @param db_path The path to the database.
     * @param host The host address.
     * @param port The port number.
     * @param concurrency_capacity The number of threads in the thread pool.
     */
    ServerManager(const std::string& db_path, const std::string& host, int port, int concurrency_capacity);

    /**
     * @brief A destructor for the ServerManager class.
     */
    ~ServerManager();
    
    /**
     * @brief A member function that initializes the server.
     */
    void init();

    /**
     * @brief A member function that initializes the location routes for the server.
     */
    void initLocationRoutes();

    /**
     * @brief A member function that initializes the device routes for the server.
     */
    void initDeviceRoutes();

    /**
     * @brief A member function that starts the server.
     */
    void start();

    /**
     * @brief A member function that stops the server.
     */
    void stop();

};  // class ServerManager

} // namespace server

#endif // SERVER_MANAGER_HPP
