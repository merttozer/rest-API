# RESTful API for Managing Devices and Locations in a Factory Environment

This project implements a RESTful API server in C++. It utilizes SQLite for database management, JSON for data exchange, and Boost libraries for enhanced performance. The server is designed to run on Ubuntu and is containerized using Docker for easy setup and deployment.

## Prerequisites

Before you begin, ensure you have Docker installed on your machine. If you prefer to run the server natively, you will need:
- Ubuntu 22.04
- CMake and Make
- g++ or another C++ compiler
- SQLite, Boost, Served and JSONCpp libraries

## Installation

### Native Installation

To install the necessary libraries on Ubuntu, run:

```bash
   sudo apt-get update
   sudo apt-get install build-essential 
   sudo apt-get install g++ 
   sudo apt-get install cmake 
   sudo apt-get install make 
   sudo apt-get install sqlite3 libsqlite3-dev
   sudo apt-get install libjsoncpp-dev
   sudo apt-get install libboost-all-dev
   sudo apt-get install git 
   git clone https://github.com/meltwater/served.git \
      && mkdir served/build && cd served/build \
      && cmake .. && make && make install
```

### Docker Installation

To build and run the server using Docker, no additional installation steps are required apart from having Docker installed.

## Building the Server

### Native Build

1. Extract project from compressed tar archive

2. Build the project using CMake:

```bash
   mkdir build
   cd build
   cmake ..
   make
```

### Docker Build

1. Build the Docker image for the server:

```bash
   docker build -t device-server .
```

## Running the Server

### Native Run

After building the project, you can run the server with:

```bash
   ./build/server
```

### Docker Run

Start the server in a Docker container:

```bash
   docker run -p 8080:8080 device-server
```

## Interacting with the Server

Interact with the server using HTTP client tools like `curl`. Example API calls:

# Example GET request

```bash
curl -X GET http://0.0.0.0:8080/devices
```

# Example POST request
```bash
curl -X POST http://0.0.0.0:8080/locations -H "Content-Type: application/json" -d '{"name": "Main Office", "type": "Office"}'
```

