#!/bin/bash
rm -r build
mkdir -p build && cd build
if [[ "$1" == "--debug" ]]; then
    echo "Building in debug mode..."
    cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
else
    echo "Building in release mode..."
    cmake .. -G Ninja
fi

ninja