#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libnsfdb/.libs/libnsfdb.1.dylib && test -f ./pynsfdb/.libs/pynsfdb.so
then
	install_name_tool -change /usr/local/lib/libnsfdb.1.dylib ${PWD}/libnsfdb/.libs/libnsfdb.1.dylib ./pynsfdb/.libs/pynsfdb.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

