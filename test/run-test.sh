#!/bin/bash

# Setup testing environment
cp -r ../src/ ../include ../CMakeLists.txt Dockerfile/build.sh Dockerfile/MachineTemplate/source
cp -r ../src/ ../include ../CMakeLists.txt Dockerfile/build.sh Dockerfile/LoadLinker/source

# Run test
docker-compose -f docker-compose.yml up --build -d

# Clean up
rm -rf build
rm -rf Dockerfile/MachineTemplate/source/*
rm -rf Dockerfile/LoadLinker/source/*