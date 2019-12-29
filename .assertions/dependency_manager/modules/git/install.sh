#!/bin/bash

SCRIPT_DIR=$(dirname $0)
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/../../../..")
MODULE_ROOT=$(realpath "$SCRIPT_DIR")
DEPENDENCIES_DIR="$PROJECT_ROOT/external_dependencies"
DEPENDENCIES_OBJ_DIR="$DEPENDENCIES_DIR/objs"
REPOSITORIES_DIR="$DEPENDENCIES_DIR/git"

rollback_installation () {
	if [ -d "$DEPENDENCY_REPOSITORY_DIR/.git" ]; then
		echo "Rolling back: deleting '$DEPENDENCY_REPOSITORY_DIR'"
		rm -rf "$DEPENDENCY_REPOSITORY_DIR"
	fi
}

mkdir -p "$REPOSITORIES_DIR"

##################### Command Line Interface ##########################
GIT_URL="$1"
if [ "$GIT_URL" == "" ]; then
	echo "Error: unspecified git URL"
	exit 1
fi
GIT_COMMIT="$2"
GIT_OBJS_DIR="$3"
GIT_INCLUDE_DIR="$4"
##################### Command Line Interface ##########################

GIT_URL_IS_HTTP=$(echo "$GIT_URL" | grep -oe "^http")
if [ "$GIT_URL_IS_HTTP" == "" ]; then
	echo "Error: not an HTTP git URL"
	exit 1
fi

RELATIVE_DEPENDENCY_REPOSITORY_DIR=$(echo "$GIT_URL" | sed "s/^.*\///; s/\.git$//")
DEPENDENCY_REPOSITORY_DIR="$REPOSITORIES_DIR/$RELATIVE_DEPENDENCY_REPOSITORY_DIR"

if [ -d "$DEPENDENCY_REPOSITORY_DIR" ]; then
	echo "Info: Dependency '$DEPENDENCY_REPOSITORY_DIR' already cloned" 1>&2
else
	cd "$REPOSITORIES_DIR"
	git clone "$GIT_URL"
	GIT_EXECUTION_STATUS=$?
	if [ "$GIT_EXECUTION_STATUS" != "0" ]; then
		exit 1
	fi
fi
cd "$DEPENDENCY_REPOSITORY_DIR"

if [ "$GIT_COMMIT" == "" ]; then
	LASTEST_TAGGED_COMMIT=$(git tag --sort refname | tail -n 1)
	echo "Info: commit not specified, using latest tagged commit ($LASTEST_TAGGED_COMMIT)" 1>&2
	GIT_COMMIT=$LASTEST_TAGGED_COMMIT
fi

git checkout $GIT_COMMIT
CHECKOUT_STATUS=$?
if [ "$CHECKOUT_STATUS" != "0" ]; then
	echo "Error: not a valid commit: '$GIT_COMMIT'"
	rollback_installation
	exit 1
fi

if [ "$GIT_OBJS_DIR" == "" ]; then
	GIT_OBJS_DIR="src/objs"
	echo "Info: OBJS_DIR not specified, using '$GIT_OBJS_DIR'"
fi

if [ "$GIT_INCLUDE_DIR" == "" ]; then
	GIT_INCLUDE_DIR="src/objs"
	echo "Info: INCLUDE_DIR not specified, using '$GIT_INCLUDE_DIR'"
fi

if [ ! -d "$DEPENDENCY_REPOSITORY_DIR/$GIT_OBJS_DIR" ]; then
		echo "Error: no directory '$GIT_OBJS_DIR' in project's root"
		rollback_installation
		exit 1
fi

if [ ! -d "$DEPENDENCY_REPOSITORY_DIR/$GIT_INCLUDE_DIR" ]; then
		echo "Error: no directory '$GIT_INCLUDE_DIR' in project's root"
		rollback_installation
		exit 1
fi

DEPENDENCY_INSTALL_DIR="$DEPENDENCIES_OBJ_DIR/$RELATIVE_DEPENDENCY_REPOSITORY_DIR"
mkdir -p "$DEPENDENCY_INSTALL_DIR"

ln -s "$DEPENDENCY_REPOSITORY_DIR/$GIT_OBJS_DIR/"* "$DEPENDENCY_INSTALL_DIR/"
if [ "$GIT_OBJS_DIR" != "$GIT_INCLUDE_DIR" ]; then
	ln -s "$DEPENDENCY_REPOSITORY_DIR/$GIT_INCLUDE_DIR/*" "$DEPENDENCY_INSTALL_DIR/"
fi

if [ -f "$DEPENDENCY_REPOSITORY_DIR/dependencies.sh" ]; then
	echo "Info: recursively installing dependencies"
	"$DEPENDENCY_REPOSITORY_DIR/dependencies.sh" install
	ln -s "$DEPENDENCY_REPOSITORY_DIR/external_dependencies/objs/"* "$DEPENDENCIES_OBJ_DIR/"
fi

echo "Info: dependency configured: $GIT_URL $GIT_COMMIT \"$GIT_OBJS_DIR\" \"$GIT_INCLUDE_DIR\""

