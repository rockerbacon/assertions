#!/bin/bash

SUCCESSFUL_TESTS=0
FAILED_TESTS=0

default_text_color=7	#color used to reset terminal text to default color. 0 will reset to black and 7 to white. See the tput command documentation for other colors
red_color=`tput setaf 1`
green_color=`tput setaf 2`
reset_color=`tput setaf $default_text_color`
up_line=`tput cuu 1`
clear_line=`tput el 1`

SCRIPT_DIR=$(realpath $(dirname $0))
TESTS_DIR="$SCRIPT_DIR/tests"
BUILD_DIR="$SCRIPT_DIR/build"

determine_current_test_full_name () {
	ESCAPED_TESTS_DIR=$(echo $TESTS_DIR | sed 's/\//\\\//g; s/\./\\\./g')
	ESCAPED_BUILD_DIR=$(echo $TEST_BUILD_DIR | sed 's/\//\\\//g; s/\./\\\./g')
	TEST_NAME=$(echo $CURRENT_TEST | sed "s/^${ESCAPED_TESTS_DIR}\///; s/^${ESCAPED_BUILD_DIR}\///;" | sed "s/^test_//; s/.cpp$//")
	TEST_FULL_NAME="test_${TEST_NAME}"
}

determine_current_test_source_file () {
	TEST_SOURCE_FILE="${TESTS_DIR}/${TEST_FULL_NAME}.cpp"
}

determine_current_test_binary_file () {
	TEST_BINARY_FILE="${BUILD_DIR}/${TEST_FULL_NAME}"
}

if [ "$#" -gt 0 ] && [ "$1" != "all" ]; then
	TESTS=$1
	shift
	until [ -z "$1" ]
	do
		TESTS="${TESTS} ${1}"
		shift
	done
else
	TESTS="${TESTS_DIR}/*.cpp"
fi

TOTAL_FAILED_TESTS=0
TOTAL_SUCCESSFUL_TESTS=0
IGNORED_TESTS=0

"$SCRIPT_DIR/build.sh" --cmake-only

exec 3>&1 # save stdout address

echo	# line feed
echo "-------------------INDIVIDUAL TESTS-------------------"
for CURRENT_TEST in $TESTS
do

	determine_current_test_full_name
	if [ -d "${TESTS_DIR}/${CURRENT_TEST}" ]; then
		echo "Executing tests from specific folder not yet supported"
	else
		determine_current_test_source_file
		determine_current_test_binary_file
		if [ -f "$TEST_SOURCE_FILE" ]; then

			echo "Building $TEST_FULL_NAME..."
			BUILD_OUTPUT=$("$SCRIPT_DIR/build.sh" --no-cmake  $TEST_FULL_NAME 2>&1)

			BUILD_STATUS=$?
			if [ $BUILD_STATUS -eq 0 ]; then
				echo "Build finished successfully"

				TEST_STDERR_OUTPUT=$($TEST_BINARY_FILE 2>&1 1>&3)	# stderr is captured, stdout is left alone
				TEST_RETURN=$?
				SUCCESSFUL_TESTS_THIS_RUN=$(echo "$TEST_STDERR_OUTPUT" | grep -oe "\"successful_tests\":[0-9]*" | sed "s/\"successful_tests\"://")
				FAILED_TESTS_THIS_RUN=$(echo "$TEST_STDERR_OUTPUT" | grep -oe "\"failed_tests\":[0-9]*" | sed "s/\"failed_tests\"://")
				TOTAL_SUCCESSFUL_TESTS=`expr $SUCCESSFUL_TESTS_THIS_RUN + $TOTAL_SUCCESSFUL_TESTS`
				TOTAL_FAILED_TESTS=`expr $FAILED_TESTS_THIS_RUN + $TOTAL_FAILED_TESTS`
			else
				echo "${red_color}Build failed${reset_color}"
				echo "${BUILD_OUTPUT}"
				TOTAL_FAILED_TESTS=`expr 1 + $TOTAL_FAILED_TESTS`
			fi
		else
			echo "${red_color}no source file ${TEST_SOURCE_FILE}${reset_color}"
		fi

	fi

	echo	# line feed

done
echo "-------------------INDIVIDUAL TESTS-------------------"

exec 3>&- # clear FD

echo	# line feed
echo "-------------------TESTS SUMMARY-------------------"
if [ $TOTAL_SUCCESSFUL_TESTS -gt 0 ]; then
	echo "${green_color}$TOTAL_SUCCESSFUL_TESTS tests passed${reset_color}"
else
	echo "$TOTAL_SUCCESSFUL_TESTS tests passed${reset_color}"
fi
if [ $TOTAL_FAILED_TESTS -gt 0 ]; then
	echo "${red_color}$TOTAL_FAILED_TESTS tests failed${reset_color}"
else
	echo "$TOTAL_FAILED_TESTS tests failed${reset_color}"
fi
echo "-------------------TESTS SUMMARY-------------------"

exit $TOTAL_FAILED_TESTS

