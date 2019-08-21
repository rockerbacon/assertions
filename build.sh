#!/bin/bash

SCRIPT_PATH=$(realpath $(dirname $0))
BUILD_PATH="${SCRIPT_PATH}/build"

mkdir -p "$BUILD_PATH"
cd "$BUILD_PATH"
cmake "$SCRIPT_PATH" -G "Unix Makefiles"
make
