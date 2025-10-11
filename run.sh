#!/usr/bin/env bash

set -eo pipefail
# set -x

cmake --build build/
./build/tree
rm -f ./output/expression.png
dot ./output/expression.dot -Tpng -o ./output/expression.png
