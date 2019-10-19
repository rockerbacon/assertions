#!/bin/bash

PROJECT_ROOT=$(realpath $(dirname $0))
BUILD_DIR="$PROJECT_ROOT/build"

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

if [ "$ACTION" == "clean" ]; then
	rm -rf "$BUILD_DIR"
elif [ "$ACTION" == "cmake" ]; then
	cmake "$PROJECT_ROOT"
elif [ "$ACTION" == "all" ]; then
	if [ ! -f "$BUILD_DIR/Makefile" ]; then
		cmake "$PROJECT_ROOT"
	fi
	make
elif [ "$ACTION" == "target" ]; then
	if [ ! -f "$BUILD_DIR/Makefile" ]; then
		cmake "$PROJECT_ROOT"
	fi
	for TARGET in "${TARGETS[@]}"
	do
		export TARGET=$TARGET
		source "$PROJECT_ROOT/.export_target_info.sh"
		if [ $TARGET_BUILD_IS_OUTDATED ]; then
			make $TARGET_RULE
		fi
	done
else
	echo "Error: unknown action $ACTION"
	exit 1
fi

BUILD_STATUS=$?
if [ ! "$BUILD_STATUS" -eq 0 ]; then
	exit $BUILD_STATUS
fi

