#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libnsfdb/.libs/libnsfdb.1.dylib && test -f ./pynsfdb/.libs/pynsfdb.so;
then
	install_name_tool -change /usr/local/lib/libnsfdb.1.dylib ${PWD}/libnsfdb/.libs/libnsfdb.1.dylib ./pynsfdb/.libs/pynsfdb.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

