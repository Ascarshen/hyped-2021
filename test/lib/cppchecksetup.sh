#!/bin/bash


command -v cppcheck > /dev/null 2>&1
if [ $? -eq 1 ]
then
    if [ "$1" = "CI" ]
    then 
        echo "Error: CPPCheck is not installed on Travis"
        exit 1
    fi
    
    echo "CPPCheck not found, unpacking from tar ..."
    mkdir -p lib/cppcheck
    tar -xf lib/cppcheck.tar.gz -C lib/cppcheck --strip-components 1

    cd lib/cppcheck 
    echo "Building CPPCheck Analyzer"
	echo "This might take a while..."
    make MATCHCOMPILER=yes FILESDIR=./ HAVE_RULES=yes CXXFLAGS="-O2 -DNDEBUG -Wall -Wno-sign-compare -Wno-unused-function" 1> /dev/null
fi
