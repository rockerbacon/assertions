#!/bin/bash

script_dir=$(realpath $(dirname $0))
project_root=$(realpath "$script_dir/..")

cd $project_root

mkdir -p .local_tests
environment_dir=$(mktemp -d .local_tests/XXXXXX)

cp		*.sh 			"$environment_dir/"
cp	-r	.assertions		"$environment_dir/"
cp	-R	tests			"$environment_dir/"
cp		CMakeLists.txt	"$environment_dir/"

echo "$environment_dir"

