/**
 * @file    main.cpp
 * @brief   This file contains the main function for the project. It creates a server object and starts it.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#include "server/server_manager.hpp"
#include "utilities/config.hpp"

int main() {

    server::ServerManager server(PATH_TO_DB, LOCAL_HOST, PORT, THREAD_POOL_SIZE);  // Create a server object

    try {
        server.init();  // Initialize the server
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
