# Detailed Database Manager Structure Documentation

## Overview
The database is designed to store and manage device and location data. It consists of two main tables: `devices` and `locations`.

## Tables and Columns

### Devices Table
Stores information about devices.

- `id`: INTEGER PRIMARY KEY AUTOINCREMENT  
  Unique identifier for each device.
- `name`: TEXT NOT NULL  
  The name of the device.
- `type`: TEXT NOT NULL  
  The type/category of the device.
- `serial_number`: TEXT UNIQUE NOT NULL  
  A unique serial number for the device.
- `creation_date`: TEXT NOT NULL  
  The date the device was created/registered.
- `location_id`: INTEGER  
  Foreign key linking to the locations table. Indicates the location of the device.

### Locations Table
Stores information about locations.

- `id`: INTEGER PRIMARY KEY AUTOINCREMENT  
  Unique identifier for each location.
- `name`: TEXT NOT NULL  
  The name of the location.
- `type`: TEXT NOT NULL  
  The type/category of the location.

## Relationships
The devices table has a foreign key (`location_id`) referencing the `id` in the locations table. This relationship allows for linking devices to their respective locations.

## Indexes and Constraints
- Primary keys (`id`) in both tables are indexed for efficient retrieval.
- The `serial_number` in the devices table is unique, ensuring no duplicate serial numbers.
- Foreign key constraints ensure referential integrity between the devices and locations tables.