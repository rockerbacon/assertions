#!/bin/bash

################ Command Line Interface ##################
print_add_help () {
	echo "Help:"
	echo "Add new dependencies to the project. Dependency information is stored inside the project so that other developers are able to easily get them"
	echo
	echo "Usage: ./dependency.sh add TYPE"
	echo
	echo "TYPE:"
	echo "	git [GIT REPOSITORY]: add a git dependency to the project, where GIT REPOSITORY is the same URL you'd use to clone a project"
}

if [ "$2" == "--help" ]; then
	print_add_help
	exit 0
elif [ "$2" == "git" ]; then
	DEPENDENCY_TYPE="$2"
	if [ "$3" != "" ]; then
		DEPENDENCY_GIT_URL="$3"
	else
		echo "Error: unspecified git repository"
		echo
		print_add_help
		exit 1
	fi
else
	echo "Error: unknown dependency type"
	echo
	print_add_help
	exit 1
fi
################ Command Line Interface ##################

rollback_installation () {
	if [ -d "$DEPENDENCY_GIT_CLONE_DIR/.git" ]; then
		echo "Rolling back: deleting '$DEPENDENCY_GIT_CLONE_DIR'"
		rm -rf "$DEPENDENCY_GIT_CLONE_DIR"
	fi
}

if [ "$DEPENDENCY_TYPE" == "git" ]; then

	DEPENDENCIES_GIT_DIR="$DEPENDENCIES_DIR/git"
	mkdir -p "$DEPENDENCIES_GIT_DIR"
	cd "$DEPENDENCIES_GIT_DIR"

	DEPENDENCY_RELATIVE_GIT_CLONE_DIR=$(echo "$DEPENDENCY_GIT_URL" | grep -oe "\/.*\.git" | sed 's/\///; s/\.git$//')
	DEPENDENCY_GIT_CLONE_DIR="$DEPENDENCIES_GIT_DIR/$DEPENDENCY_RELATIVE_GIT_CLONE_DIR"

	git clone "$DEPENDENCY_GIT_URL"
	GIT_EXECUTION_STATUS=$?

	#echo $CLONE_OUTPUT
	if [ "$GIT_EXECUTION_STATUS" != "0" ]; then
		echo
		print_add_help
		exit 1
	fi

	if [ -f "$DEPENDENCY_GIT_CLONE_DIR/.assertions/language" ]; then
		DEPENDENCY_LANGUAGE=$(cat "$DEPENDENCY_GIT_CLONE_DIR/.assertions/language")
		if [ "$DEPENDENCY_LANGUAGE" == "cpp" ]; then
			echo
			echo "Error: dependency management is still WIP"
			rollback_installation
			exit 1
		fi
	else
		echo
		echo "Error: dependencies can only be from projects using the Assertions C++ Framework, for now. Support for other projects is still WIP"
		rollback_installation
		exit 1
	fi

fi
