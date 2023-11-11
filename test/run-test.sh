#!/bin/bash

# Build project
mkdir build && cd build
cmake -G "Unix Makefiles" ../../ && cmake --build .

# Setup testing environment
cd ../
cp build/LoadLinker Dockerfile/LoadLinker/
for container in {1..4}
do
  cp build/LoadLinkerAgent Dockerfile/Machine$container
done

# Run test
docker-compose -f docker-compose.yml up --build -d

# Clean up
rm -rf build
rm Dockerfile/LoadLinker/LoadLinker
for container in {1..4}
do
  rm Dockerfile/Machine$container/LoadLinkerAgent
done
