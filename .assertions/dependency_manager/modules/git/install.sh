#!/bin/bash

SCRIPT_DIR=$(dirname $0)
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/../../../..")
MODULE_ROOT=$(realpath "$SCRIPT_DIR")
DEPENDENCIES_DIR="$PROJECT_ROOT/external_dependencies"
DEPENDENCIES_OBJ_DIR="$DEPENDENCIES_DIR/objs"
DEPENDENCIES_LOCAL_OBJ_DIR="$DEPENDENCIES_DIR/local_objs"
REPOSITORIES_DIR="$DEPENDENCIES_DIR/git"

rollback_installation () {
	if [ -d "$DEPENDENCY_REPOSITORY_DIR/.git" ]; then
		echo "Rolling back: deleting '$DEPENDENCY_REPOSITORY_DIR'"
		rm -rf "$DEPENDENCY_REPOSITORY_DIR"
	fi
}

mkdir -p "$REPOSITORIES_DIR"

##################### Command Line Interface ##########################
GIT_PATH="$1"
shift
if [ "$GIT_PATH" == "" ]; then
	echo "Error: unspecified git path"
	exit 1
fi
FROZEN_ARGS="$GIT_PATH"

until [ -z "$1" ]; do

	case "$1" in

	--version)
		GIT_COMMIT="$2"
		shift
	;;

	--local-only)
		LOCAL_ONLY=true
		FROZEN_ARGS="$FROZEN_ARGS --local-only"
	;;

	--objs-dir)
		GIT_OBJS_DIR="$2"
		FROZEN_ARGS="$FROZEN_ARGS --objs-dir '$GIT_OBJS_DIR'"
		shift
	;;

	--include-dir)
		GIT_INCLUDE_DIR="$2"
		FROZEN_ARGS="$FROZEN_ARGS --include-dir '$GIT_INCLUDE_DIR'"
		shift
	;;

	--before-linking)
		BEFORE_LINKING_SCRIPT="$2"
		FROZEN_ARGS="$FROZEN_ARGS --before-linking '$POST_DOWNLOAD_SCRIPT'"
		shift
	;;

	--alias)
		DEPENDENCY_NAME="$2"
		FROZEN_ARGS="$FROZEN_ARGS --alias '$DEPENDENCY_NAME'"
		shift
	;;

	--domain)
		GIT_SERVER_DOMAIN="$2"
		FROZEN_ARGS="$FROZEN_ARGS --domain '$GIT_SERVER_DOMAIN'"
		shift
	;;

	--use-http)
		DOWNLOAD_PROTOCOL="http"
		FROZEN_ARGS="$FROZEN_ARGS --use-http"
	;;

	esac

	shift
done
##################### Command Line Interface ##########################

if [ "$IGNORE_LOCAL_DEPENDENCIES" == "true" ] && [ "$LOCAL_ONLY" == "true" ]; then
	echo "Info: skipping local dependency 'git ${GIT_PATH}'" 1>&2
	exit 0
else
	echo "Info: installing dependency 'git ${GIT_PATH}'" 1>&2
fi

if [ "$DEPENDENCY_NAME" == "" ]; then
	DEPENDENCY_NAME=$(echo "$GIT_PATH" | sed "s/^.*\///")
fi

DEPENDENCY_REPOSITORY_DIR="$REPOSITORIES_DIR/$DEPENDENCY_NAME"

if [ "$DOWNLOAD_PROTOCOL" == "" ]; then
	DOWNLOAD_PROTOCOL="https"
fi

if [ "$GIT_SERVER_DOMAIN" == "" ]; then
	GIT_SERVER_DOMAIN="github.com"
fi

REPOSITORY_URL="$DOWNLOAD_PROTOCOL://$GIT_SERVER_DOMAIN/$GIT_PATH.git"

if [ -d "$DEPENDENCY_REPOSITORY_DIR" ]; then
	echo "Info: Dependency '$DEPENDENCY_REPOSITORY_DIR' already cloned" 1>&2
else
	git clone "$REPOSITORY_URL" "$DEPENDENCY_REPOSITORY_DIR"; CLONE_STATUS=$?
	if [ $CLONE_STATUS -ne 0 ]; then
		exit 1
	fi
fi
cd "$DEPENDENCY_REPOSITORY_DIR"

if [ "$GIT_COMMIT" == "" ]; then
	GIT_COMMIT=$(git tag --sort refname | tail -n 1)
	echo "Info: commit not specified, using latest tagged commit ($GIT_COMMIT)" 1>&2
fi
FROZEN_ARGS="$FROZEN_ARGS --version $GIT_COMMIT"

echo "Info: checking out $GIT_COMMIT" 1>&2
git checkout -q $GIT_COMMIT; CHECKOUT_STATUS=$?
if [ $CHECKOUT_STATUS -ne 0 ]; then
	echo "Error: not a valid commit: '$GIT_COMMIT'"
	rollback_installation
	exit 1
fi

if [ "$BEFORE_LINKING_SCRIPT" != "" ]; then
	echo "Info: executing script '$BEFORE_LINKING_SCRIPT'" 1>&2
	$BEFORE_LINKING_SCRIPT
fi

if [ "$GIT_OBJS_DIR" == "" ]; then
	GIT_OBJS_DIR="src/objs"
	echo "Info: --objs-dir not specified, using '$GIT_OBJS_DIR'" 1>&2
fi

if [ "$GIT_INCLUDE_DIR" == "" ]; then
	GIT_INCLUDE_DIR="src/objs"
	echo "Info: --include-dir not specified, using '$GIT_INCLUDE_DIR'" 1>&2
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

if [ "$LOCAL_ONLY" == "" ]; then
	LOCAL_ONLY="false"
fi

if [ "$LOCAL_ONLY" == "false" ]; then
	DEPENDENCY_INSTALL_DIR="$DEPENDENCIES_OBJ_DIR/$DEPENDENCY_NAME"
else
	DEPENDENCY_INSTALL_DIR="$DEPENDENCIES_LOCAL_OBJ_DIR/$DEPENDENCY_NAME"
fi
mkdir -p "$DEPENDENCY_INSTALL_DIR"

echo "Info: linking '$DEPENDENCY_REPOSITORY_DIR/$GIT_OBJS_DIR/*' in '$DEPENDENCY_INSTALL_DIR/'" 1>&2
ln -s "$DEPENDENCY_REPOSITORY_DIR/$GIT_OBJS_DIR/"* "$DEPENDENCY_INSTALL_DIR/"
if [ "$GIT_OBJS_DIR" != "$GIT_INCLUDE_DIR" ]; then
	echo "Info: linking '$DEPENDENCY_REPOSITORY_DIR/$GIT_INCLUDE_DIR/*' in '$DEPENDENCY_INSTALL_DIR/'" 1>&2
	ln -s "$DEPENDENCY_REPOSITORY_DIR/$GIT_INCLUDE_DIR/"* "$DEPENDENCY_INSTALL_DIR/"
fi

if [ -f "$DEPENDENCY_REPOSITORY_DIR/dependencies.sh" ]; then
	echo "Info: recursively installing dependencies" 1>&2
	"$DEPENDENCY_REPOSITORY_DIR/dependencies.sh" install --ignore-local-dependencies
	HAS_RECURSIVE_DEPENDENCIES=$(ls -A "$DEPENDENCY_REPOSITORY_DIR/external_dependencies/objs")
	if [ "$HAS_RECURSIVE_DEPENDENCIES" != "" ]; then
		echo "Info: linking '$DEPENDENCY_REPOSITORY_DIR/external_dependencies/objs/*' in '$DEPENDENCIES_OBJ_DIR/'" 1>&2
		ln -s "$DEPENDENCY_REPOSITORY_DIR/external_dependencies/objs/"* "$DEPENDENCIES_OBJ_DIR/"
	fi
fi

echo "Info: dependency configured: $FROZEN_ARGS"

