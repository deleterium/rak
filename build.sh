#!/usr/bin/env bash

# First argument to the build type. Use "Debug" or default to "Release"
BUILD_TYPE=${1:-Release}

# Other arguments will be passed as defines to the code: "ENABLE_FEATURE=ON"
EXTRA_DEFINES=()
for ((i=2; i<=$#; i++)); do
    EXTRA_DEFINES+=("-D${!i}")
done

cmake -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE" "${EXTRA_DEFINES[@]}"
cmake --build build
