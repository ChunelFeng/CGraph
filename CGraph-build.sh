#!/bin/bash

rm -rf build

cmake . -Bbuild
cd build || { ! echo "enter build failed"; exit 1; }
make -j8
echo -e "\033[34m congratulations, automatic compile CGraph finish... \033[0m"
