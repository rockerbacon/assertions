#!/bin/bash

script_dir=$(realpath $(dirname $0))
project_root=$(realpath "$script_dir/..")

cd "$project_root"

rm -rf .local_tests

