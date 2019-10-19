#!/bin/bash

export PROJECT_ROOT=$(realpath $(dirname $0))

############# Command Line Interface ###############
if [ "$#" -eq 0 ]; then
	echo "Error: please specify a target"
	exit 1
fi
TARGET="$1"
shift
until [ -z "$1" ]
do
	if [ "$1" == "--debug" ]; then
		OUTPUT_MODE="stderr"
	elif [ "$1" == "--full-output" ]; then
		OUTPUT_MODE="full"
	fi
	shift
done
############ Command Line Interface ###############

export TARGET=$TARGET
source "${PROJECT_ROOT}/.export_target_info.sh"

./build.sh target "$TARGET"
BUILD_STATUS=$?
if [ ! "$BUILD_STATUS" -eq 0 ]; then
	exit 1
fi

if [ $TARGET_IS_TEST ]; then
	if [ "$OUTPUT_MODE" == "stderr" ]; then
		"$TARGET_BINARY_PATH" 1>/dev/null
	elif [ "$OUTPUT_MODE" == "full" ]; then
		"$TARGET_BINARY_PATH"
	else
		"$TARGET_BINARY_PATH" 2>/dev/null
	fi
else
	"$TARGET_BINARY_PATH"
fi

TARGET_STATUS="$?"
exit $TARGET_STATUS

