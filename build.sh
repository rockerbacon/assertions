#!/bin/bash

PROJECT_ROOT=$(realpath $(dirname $0))
TESTS_SRC_DIR="$PROJECT_ROOT/tests"
MAIN_SRC_DIR="$PROJECT_ROOT/src/main"
BUILD_DIR="$PROJECT_ROOT/build"

ESCAPED_PROJECT_ROOT=$(echo "$PROJECT_ROOT" | sed 's/\//\\\//g; s/\./\\\./g')

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

######### Command Line Interface #########
if [ "$#" -eq 0 ]; then
	ACTION="all"
else
	ACTION="$1"
	shift
	until [ -z "$1" ]
	do
		TARGETS+=("$1")
		shift
	done
fi
######### Commnd Line Interface #########

determine_target_rule () {
	TARGET_CLEAN_PATH=$(echo "$TARGET" | sed "s/${ESCAPED_PROJECT_ROOT}\///g; s/src\/main\///; s/.cpp$//")
	TARGET_RULE=$(echo $TARGET_CLEAN_PATH | sed "s/\//_/g")
	TARGET_EXISTS=$(make help | grep -o "$TARGET_RULE")
	if [ "$TARGET_EXISTS" == "" ]; then
		echo "Error: no target '$TARGET'"
		exit 1
	fi
}

if [ "$ACTION" == "clean" ]; then
	rm -rf "$BUILD_DIR"
elif [ "$ACTION" == "cmake" ]; then
	cmake "$PROJECT_ROOT"
	BUILD_STATUS=$?
elif [ "$ACTION" == "all" ]; then
	if [ ! -f "$BUILD_DIR/Makefile" ]; then
		cmake "$PROJECT_ROOT"
	fi
	make
	BUILD_STATUS=$?
elif [ "$ACTION" == "target" ]; then
	if [ ! -f "$BUILD_DIR/Makefile" ]; then
		cmake "$PROJECT_ROOT"
	fi
	for TARGET in "${TARGETS[@]}"
	do
		determine_target_rule
		make $TARGET_RULE
		BUILD_STATUS=$?
	done
else
	echo "Error: unknown action $ACTION"
	exit 1
fi

if [ ! "$BUILD_STATUS" -eq 0 ]; then
	exit $BUILD_STATUS
fi

