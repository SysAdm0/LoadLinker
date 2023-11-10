#!/bin/bash

# Build project
mkdir build && cd build
cmake -G "Unix Makefiles" ../../ && cmake --build .

# Setup testing environment
cd ../
cp build/LoadLinker Dockerfile/LoadLinker/
cp build/LoadLinkerAgent Dockerfile/LoadLinkerAgent/

# Run test
docker-compose -f docker-compose.yml up --build -d

# Clean up
rm -rf build
rm Dockerfile/LoadLinker/LoadLinker
rm Dockerfile/LoadLinkerAgent/LoadLinkerAgent