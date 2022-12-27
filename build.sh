#!/bin/bash

echo -e

function BUILD () {
    echo "Building"
    mkdir build
    pushd build
    cmake ..
    make all
    popd
}

function CLEAN () {
    if [ -d ./build ]
    then
        echo "Deleting build directory"
        rm -r build
    else
        echo "Nothing to do"
    fi
}

function RUN () {
    if [ ! -d ./build ]
    then
        BUILD
    fi
    if [ -z "$@" ]
    then
        ./build/terrain-generator
    else
        ./build/terrain-generator $@
    fi
}

CMD="$1"
if [ -z "$CMD" ]
then
    CMD="build"
fi

echo "Command: $CMD"

if [ $CMD = "build" ]
then
    BUILD
fi

if [ $CMD = "clean" ]
then
    CLEAN
fi

if [ $CMD = "run" ]
then
    RUN ${@:2}
fi