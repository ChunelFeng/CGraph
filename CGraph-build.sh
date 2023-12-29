#!/bin/bash

rm -rf build

cmake -DCGRAPH_BUILD_FUNCTIONAL_TESTS=ON -DCGRAPH_BUILD_PERFORMANCE_TESTS=ON . -Bbuild
cd build || { ! echo "enter build failed"; exit 1; }
make -j8
echo -e "\033[34m congratulations, automatic compile CGraph finish... \033[0m"
