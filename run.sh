#!/bin/bash

PROJECT_ROOT=$(realpath $(dirname $0))
TESTS_SRC_DIR="$PROJECT_ROOT/tests"
MAIN_SRC_DIR="$PROJECT_ROOT/src/main"
BUILD_DIR="$PROJECT_ROOT/build"
TESTS_BUILD_DIR="$BUILD_DIR/tests"
PROGRAM_BUILD_DIR="$BUILD_DIR/release/bin"

ESCAPED_PROJECT_ROOT=$(echo "$PROJECT_ROOT" | sed 's/\//\\\//g; s/\./\\\./g')

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

determine_target_rule() {
	TARGET_CLEAN_PATH=$(echo "$TARGET" | sed "s/${ESCAPED_PROJECT_ROOT}\///g; s/src\/main\///; s/.cpp$//")
	TARGET_RULE=$(echo $TARGET_CLEAN_PATH | sed "s/\//_/g")
}

determine_target_source() {
	TARGET_RELATIVE_PATH="$TARGET_CLEAN_PATH.cpp"
	if [ $TARGET_IS_TEST ]; then
		TARGET_SOURCE="$PROJECT_ROOT/$TARGET_RELATIVE_PATH"
	else
		TARGET_SOURCE="$MAIN_SRC_DIR/$TARGET_RELATIVE_PATH"
	fi
	if [ ! -f "$TARGET_SOURCE" ]; then
		echo "Error: no such target"
		exit 1
	fi
}

determine_target_is_test () {
	CONTAINS_TEST_PREFIX=$(echo $TARGET_RULE | grep -oe '^tests_')
	if [ "$CONTAINS_TEST_PREFIX" != "" ]; then
		TARGET_IS_TEST=true
	fi
}

determine_target_executable() {
	if [ $TARGET_IS_TEST ]; then
		TARGET_EXECUTABLE="$TESTS_BUILD_DIR/$TARGET_RULE"
	else
		TARGET_EXECUTABLE="$PROGRAM_BUILD_DIR/$TARGET_RULE"
	fi
}

determine_target_needs_rebuild() {
	if [ ! -f "$TARGET_EXECUTABLE" ]; then
		TARGET_NEEDS_REBUILD=true
	else
		TARGET_SOURCE_MODIFICATION_TIME=$(stat -c %Y "$TARGET_SOURCE")
		TARGET_EXECUTABLE_MODIFICATION_TIME=$(stat -c %Y "$TARGET_EXECUTABLE")
		if [ "$TARGET_SOURCE_MODIFICATION_TIME" -gt "$TARGET_EXECUTABLE_MODIFICATION_TIME" ]; then
			TARGET_NEEDS_REBUILD=true
		fi
	fi
}

determine_target_rule
determine_target_is_test
determine_target_source
determine_target_executable
determine_target_needs_rebuild

if [ $TARGET_NEEDS_REBUILD ]; then
	./build.sh target "$TARGET"
fi

if [ "$TARGET_IS_TEST" ]; then
	if [ "$OUTPUT_MODE" == "stderr" ]; then
		"$TARGET_EXECUTABLE" 1>/dev/null
	elif [ "$OUTPUT_MODE" == "full" ]; then
		"$TARGET_EXECUTABLE"
	else
		"$TARGET_EXECUTABLE" 2>/dev/null
	fi
else
	"$TARGET_EXECUTABLE"
fi

