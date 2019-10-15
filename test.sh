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

ESCAPED_TESTS_DIR=$(echo $TESTS_DIR | sed 's/\//\\\//g; s/\./\\\./g')

determine_current_test_full_name () {
	CURRENT_TEST=$(echo $CURRENT_TEST | sed "s/^${ESCAPED_TESTS_DIR}\///; s/.cpp$//")
}

determine_current_test_source_file () {
	TEST_SOURCE_FILE="${TESTS_DIR}/${CURRENT_TEST}.cpp"
}

determine_current_test_binary_file () {
	TEST_BINARY_BUILD_RULE=$(echo $CURRENT_TEST | sed "s/\//_/g")
	TEST_BINARY_BUILD_RULE="tests_${TEST_BINARY_BUILD_RULE}"
	TEST_BINARY_FILE="${BUILD_DIR}/tests/${TEST_BINARY_BUILD_RULE}"
}

add_tests_from_folder () {
	TESTS_IN_FOLDER="${CURRENT_TEST}/*"
	for INNER_TEST in $TESTS_IN_FOLDER
	do
		TESTS+=("$INNER_TEST")
	done
}

if [ "$#" -gt 0 ] && [ "$1" != "all" ]; then
	TESTS=("${TESTS_DIR}/$1")
	shift
	until [ -z "$1" ]
	do
		TESTS+=("${TESTS_DIR}/${1}")
		shift
	done
else
	TESTS=("${TESTS_DIR}")
fi

TOTAL_FAILED_TESTS=0
TOTAL_SUCCESSFUL_TESTS=0
IGNORED_TESTS=0

"$SCRIPT_DIR/build.sh" --cmake-only

exec 3>&1 # save stdout address

echo	# line feed
echo "-------------------INDIVIDUAL TESTS-------------------"
until [ "${TESTS[0]}" == "" ]
do

	CURRENT_TEST="${TESTS[0]}"
	TESTS=("${TESTS[@]:1}")

	if [ -d "${CURRENT_TEST}" ]; then
		add_tests_from_folder
	else
		determine_current_test_full_name
		determine_current_test_source_file
		determine_current_test_binary_file
		if [ -f "$TEST_SOURCE_FILE" ]; then

			echo "Building $CURRENT_TEST..."
			BUILD_OUTPUT=$("$SCRIPT_DIR/build.sh" --no-cmake  $TEST_BINARY_BUILD_RULE 2>&1)

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

		echo	# line feed
	fi

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

