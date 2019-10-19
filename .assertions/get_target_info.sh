#!/bin/bash

if [ "$TARGET" != "$CURRENT_TARGET" ]; then
	TESTS_SRC_DIR="$PROJECT_ROOT/tests"
	MAIN_SRC_DIR="$PROJECT_ROOT/src/main"
	BUILD_DIR="$PROJECT_ROOT/build"
	TESTS_BUILD_DIR="$BUILD_DIR/tests"
	PROGRAM_BUILD_DIR="$BUILD_DIR/release/bin"

	ESCAPED_PROJECT_ROOT=$(echo "$PROJECT_ROOT" | sed 's/\//\\\//g; s/\./\\\./g')

	export_target_rule () {
		TARGET_CLEAN_PATH=$(echo "$TARGET" | sed "s/${ESCAPED_PROJECT_ROOT}\///g; s/src\/main\///; s/.cpp$//")
		export TARGET_RULE=$(echo $TARGET_CLEAN_PATH | sed "s/\//_/g")
	}

	export_target_is_test () {
		CONTAINS_TEST_PREFIX=$(echo $TARGET_RULE | grep -oe '^tests_')
		if [ "$CONTAINS_TEST_PREFIX" != "" ]; then
			export TARGET_IS_TEST=true
		fi
	}

	export_target_source_path() {
		TARGET_RELATIVE_PATH="$TARGET_CLEAN_PATH.cpp"
		if [ $TARGET_IS_TEST ]; then
			export TARGET_SOURCE_PATH="$PROJECT_ROOT/$TARGET_RELATIVE_PATH"
		else
			export TARGET_SOURCE_PATH="$MAIN_SRC_DIR/$TARGET_RELATIVE_PATH"
		fi
		if [ ! -f "$TARGET_SOURCE_PATH" ]; then
			echo "Error: no target '${TARGET}'"
		fi
	}

	export_target_binary_path() {
		if [ $TARGET_IS_TEST ]; then
			export TARGET_BINARY_PATH="$TESTS_BUILD_DIR/$TARGET_RULE"
		else
			export TARGET_BINARY_PATH="$PROGRAM_BUILD_DIR/$TARGET_RULE"
		fi
	}

	export_target_build_is_outdated() {
		if [ ! -f "$TARGET_BINARY_PATH" ]; then
			export TARGET_BUILD_IS_OUTDATED=true
		else
			TARGET_SOURCE_MODIFICATION_TIME=$(stat -c %Y "$TARGET_SOURCE_PATH")
			TARGET_BINARY_MODIFICATION_TIME=$(stat -c %Y "$TARGET_BINARY_PATH")
			if [ "$TARGET_SOURCE_MODIFICATION_TIME" -gt "$TARGET_BINARY_MODIFICATION_TIME" ]; then
				export TARGET_BUILD_IS_OUTDATED=true
			else
				export TARGET_BUILD_IS_OUTDATED=""
			fi
		fi
	}

	export_target_rule
	export_target_is_test
	export_target_source_path
	export_target_binary_path
	export_target_build_is_outdated

	export CURRENT_TARGET=$TARGET
fi
