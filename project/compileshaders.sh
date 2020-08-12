#!/bin/sh
find ./lib/shader -name "*.shader" -exec shader2source.exe {} \;
make -j4
