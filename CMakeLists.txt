cmake_minimum_required(VERSION 3.10)
project(DeviceServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If SQLite3 is installed in a non-standard location, specify its path here
set(SQLite3_INCLUDE_DIRS "/usr/include")
set(SQLite3_LIBRARIES "/usr/lib/x86_64-linux-gnu/libsqlite3.so")

# If served is installed in a non-standard location, specify its path here
set(SERVED_INCLUDE_DIRS "/usr/local/include/served")
set(SERVED_LIBRARIES "/usr/local/lib/libserved.so")

# If JsonCpp is installed in a non-standard location, specify its path here
set(JSONCPP_INCLUDE_DIRS "/usr/include/jsoncpp/json")
set(JSONCPP_LIBRARIES "/usr/lib/x86_64-linux-gnu/libjsoncpp.so")

# Include directories for SQLite3 and optionally for served and JsonCpp
include_directories(${SQLite3_INCLUDE_DIRS} ${SERVED_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})

# Add the executable and specify the source files
add_executable(server 
    src/main.cpp 
    src/server/server_manager.cpp 
    src/database/database_manager.cpp
)

# Link the libraries to the executable
target_link_libraries(server ${SQLite3_LIBRARIES} ${SERVED_LIBRARIES} ${JSONCPP_LIBRARIES})