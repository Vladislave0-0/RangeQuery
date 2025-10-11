#!/usr/bin/env bash

set -eo pipefail
# set -x

cmake --build build/
./build/tree
rm -f expression.png
dot expression.dot -Tpng -o expression.png
