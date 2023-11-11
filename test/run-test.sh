#!/bin/bash

# Setup testing environment
for container in {1..4}
do
  cp -r ../src/ ../include ../CMakeLists.txt Dockerfile/build.sh Dockerfile/Machine$container/source
done
cp -r ../src/ ../include ../CMakeLists.txt Dockerfile/build.sh Dockerfile/LoadLinker/source

# Run test
docker-compose -f docker-compose.yml up --build -d

# Clean up
rm -rf build
for container in {1..4}
do
  rm -rf Dockerfile/Machine$container/source/*
done
rm -rf Dockerfile/LoadLinker/source/*
