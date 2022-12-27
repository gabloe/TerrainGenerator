#!/bin/bash

echo -e

BUILD () {
    mkdir build
    pushd build
    cmake ..
    make all
    popd
}

CLEAN () {
    if [ -d ./build ]
    then
        echo "Deleting build directory"
        rm -r build
    else
        echo "Nothing to do"
    fi
}

RUN () {
    if [ ! -d ./build ]
    then
        BUILD
    fi
    ./build/terrain-generator
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
    RUN
fi