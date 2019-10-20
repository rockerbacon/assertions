#!/bin/bash

PROJECT_ROOT=$(realpath $(dirname $0))
DEPENDENCIES_DIR="$PROJECT_ROOT/external_dependencies"
DEPENDENCIES_LIB_DIR="$DEPENDENCIES_DIR/lib"
DEPENDENCIES_INCLUDE_DIR="$DEPENDENCIES_DIR/include"

mkdir -p "$DEPENDENCIES_LIB_DIR"
mkdir -p "$DEPENDENCIES_INCLUDE_DIR"

############### Command Line Interface ##################
print_help () {
	echo "Help:"
	echo "Manage dependencies for the project"
	echo
	echo "Usage: ./dependencies.sh ACTION"
	echo
	echo "ACTION:"
	echo "	add: add new dependency to the project"
	echo "	clean: delete all downloaded dependencies (everything inside ./external_dependencies)"
}

if [ "$1" == "--help" ]; then
	print_help
	exit 0
elif [ "$1" == "add" ]; then
	source "$PROJECT_ROOT/.assertions/dependency_manager/add.sh"
elif [ "$1" == "clean" ]; then
	echo "Are you sure you want to delete all downloaded dependencies? (y/n)"
	read CONFIRMATION
	if [ "${CONFIRMATION[0]}" == "y" ] || [ "${CONFIRMATION[1]}" == "Y" ]; then
		rm -rf "$PROJECT_ROOT/external_dependencies"
		echo "Downloaded dependencies deleted"
	else
		echo "Operation cancelled"
	fi
else
	echo "Error: unknown action"
	echo
	print_help
	exit 1
fi
############### Command Line Interface ##################

