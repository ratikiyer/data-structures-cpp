# Use an official C++ development image as a base
FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV PROJECT_DIR=/usr/src/app
ENV BUILD_DIR=/usr/src/app/build

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Create project directories
WORKDIR ${PROJECT_DIR}

# Copy the current directory's content into the container
COPY . ${PROJECT_DIR}

# Create a build directory
RUN mkdir -p ${BUILD_DIR}

# Default entrypoint to run bash for interactive builds
ENTRYPOINT ["bash"]
