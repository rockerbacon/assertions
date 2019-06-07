#!/bin/bash

SUCCESSFUL_TESTS=0
FAILED_TESTS=0

default_text_color=7	#color used to reset terminal text to default color. 0 will reset to black and 7 to white. See the tput command documentation for other colors
red_color=`tput setaf 1`
green_color=`tput setaf 2`
reset_color=`tput setaf $default_text_color`
up_line=`tput cuu 1`
clear_line=`tput el 1`

TESTS_DIR=$(cat makefile | grep -e 'TESTS_DIR\s*=' | sed 's/TESTS_DIR[][:space:]]*=[][:space:]]*//')
TEST_BUILD_DIR=$(cat makefile | grep -e 'TEST_BUILD_DIR\s*=' | sed 's/TEST_BUILD_DIR[[:space:]]*=[[:space:]]*//')

determine_current_test_full_name () {
	TEST_NAME=$(echo $CURRENT_TEST | sed "s/^test_//; s/.cpp$//")
	TEST_NAME="test_${TEST_NAME}"
}

determine_current_test_source_file () {
	TEST_SOURCE_FILE="${TESTS_DIR}/${TEST_NAME}.cpp"
}

determine_current_test_binary_file () {
	TEST_BINARY_FILE="${TEST_BUILD_DIR}/${TEST_NAME}"
}

if [ $# -gt 0 ]; then

	FAILED_TESTS=0
	SUCCESSFUL_TESTS=0
	IGNORED_TESTS=0

	echo	# line feed
	echo "-------------------INDIVIDUAL TESTS-------------------"
	until [ -z "$1" ]
	do

		CURRENT_TEST=$1

		echo "Testing ${CURRENT_TEST}..."
		if [ -f "${TESTS_DIR}/${CURRENT_TEST}" ]; then
			echo "Executing tests from specific folder not yet supported, use regular expressions instead"
		else
			determine_current_test_full_name
			determine_current_test_source_file
			determine_current_test_binary_file
			if [ -f "$TEST_SOURCE_FILE" ]; then
				make -s $TEST_BINARY_FILE
				BUILD_STATUS=$?
				if [ $BUILD_STATUS -eq 0 ]; then
					TEST_OUTPUT=$("${TEST_BUILD_DIR}/$TEST_NAME")
					TEST_SUCCESSES=$(echo $TEST_OUTPUT | grep -o "' OK" | wc -l)
					TEST_FAILURES=$(echo $TEST_OUTPUT | grep -o 'failed: ' | wc -l)
					SUCCESSFUL_TESTS=`expr $SUCCESSFUL_TESTS + $TEST_SUCCESSES`
					FAILED_TESTS=`expr $FAILED_TESTS + $TEST_FAILURES`
					echo "	${TEST_OUTPUT}" | tr '\n' '\0' | sed 's/\0/\n\t/g' | sed 's/\n\t__successful_test_cases.*$//I'
				else
					echo "	${red_color}build failed for ${TEST_SOURCE_FILE}${reset_color}"
					FAILED_TESTS=`expr $FAILED_TESTS + 1`
				fi
			else
				echo "${red_color}no source file ${TEST_SOURCE_FILE}${reset_color}"
			fi

		fi

		echo	# line feed

		shift

	done
	echo "-------------------INDIVIDUAL TESTS-------------------"

	echo	# line feed
	echo "-------------------TESTS SUMMARY-------------------"
	if [ $SUCCESSFUL_TESTS -gt 0 ]; then
		echo "${green_color}$SUCCESSFUL_TESTS passed${reset_color}"
	else
		echo "$SUCCESSFUL_TESTS passed${reset_color}"
	fi
	if [ $FAILED_TESTS -gt 0 ]; then
		echo "${red_color}$FAILED_TESTS failed${reset_color}"
	else
		echo "$FAILED_TESTS failed${reset_color}"
	fi
	echo "-------------------TESTS SUMMARY-------------------"

else
	echo "Specify tests to be run"
fi
