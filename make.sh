#!/bin/sh

cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=~/apps/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build

