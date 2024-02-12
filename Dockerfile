# Use an official Ubuntu base image
FROM ubuntu:22.04

# Set non-interactive shell to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Set the working directory in the container
WORKDIR /usr/src/app

# Install the necessary packages
RUN apt-get update && apt-get install -y build-essential && apt-get install -y \
    g++ \
    cmake \
    make \
    sqlite3 \
    libsqlite3-dev \
    libboost-all-dev \
    libjsoncpp-dev \
    git

# Clone and build the served library
RUN git clone https://github.com/meltwater/served.git \
    && mkdir served/build && cd served/build \
    && cmake .. && make && make install

# Copy the current directory contents into the container
COPY . .

# Create a build directory and run CMake and make from there
RUN mkdir build && cd build \
    && cmake .. && make

# Expose the port the server listens on
EXPOSE 8080

# Define the working directory as build to run the server
WORKDIR /usr/src/app/build

# Run the server when the container launches
CMD ["./server"]
