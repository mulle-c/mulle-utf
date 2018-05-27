#!/usr/bin/env bash

PROJECTDIR="`dirname "$PWD"`"
PROJECTNAME="`basename "${PROJECTDIR}"`"
LIBRARY_SHORTNAME="mulle-utf"
#MAKE=make  # nmake not good

. "mulle-tests/test-c-common.sh"
. "mulle-tests/test-tools-common.sh"
. "mulle-tests/test-sharedlib-common.sh"
. "mulle-tests/build-test-common.sh"