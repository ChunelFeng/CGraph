#!/bin/bash

rm -rf build

cmake -DCGRAPH_BUILD_FUNCTIONAL_TESTS=ON -DCGRAPH_BUILD_PERFORMANCE_TESTS=ON -G Ninja -Bbuild
cd build || { ! echo "enter ninja-build failed"; exit 1; }
ninja -j8
echo -e "\033[34m congratulations, automatic compile CGraph finish by ninja... \033[0m"
