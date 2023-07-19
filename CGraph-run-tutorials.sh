#!/bin/bash

if [ ! -d "build" ]; then
    echo -e "\033[31m[ERROR] Directory /build does not exist.\033[0m"
    exit 1
fi

cd build

for file in *; do
    # 执行所有可执行文件，并且过滤掉文件夹
    if [ -f "$file" ] && [ -x "$file" ]; then
        echo "**** Running $file ****"
        "./$file"
        echo " "
    fi
done

echo -e "\033[34m congratulations, automatic test CGraph finish... \033[0m"