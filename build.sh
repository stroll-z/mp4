#! /usr/bin/env bash

mkdir -p cmake-build
mkdir -p test/bin

cd cmake-build
cmake ..
make
