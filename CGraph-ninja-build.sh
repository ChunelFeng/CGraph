#!/bin/bash

rm -rf build

cmake -G Ninja -Bbuild
cd build || { ! echo "enter ninja-build failed"; exit 1; }
ninja
echo -e "\033[34m congratulations, automatic compile CGraph finish by ninja... \033[0m"
