/**
 * @file    metadata.hpp
 * @brief   This file contains the declaration of the metadata structs for the database.
 * @author  Mert Ozer
 * @date    26.11.2023
 * @version 1.0
 */

#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>


struct Device {
    int id;
    std::string name;
    std::string type;
    std::string serial_number;
    std::string creation_date;
    int location_id; // Add location_id to link with the locations table
};

struct Location {
    int id;
    std::string name;
    std::string type;
};


#endif // METADATA_HPP
