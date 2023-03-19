#!/bin/bash

image_name="cgraph_env"
workdir="$(cd "$(dirname "$0")" && pwd)"
container_name="CGraph_${USER}"

if [ "$(uname)" != "Linux" ]; then
    echo -e "\033[33mPlease work in Linux env. Stop...\033[0m"
    exit 0
fi

# 判断cgraph_env镜像是否存在，没有就创建
if ! docker images "$image_name" | grep -q "$image_name"; then
    docker build -f "$workdir"/dockerfile -t $image_name "$workdir"
    echo -e "\033[32mBuild $image_name success.\033[0m"
fi

# 如果未开启，则开启并且进入
if [ -z "$(docker ps -q -f name="$container_name")" ]; then
    docker run -it --rm --name="$container_name" -v "$workdir":/home/CGraph cgraph_env /bin/bash
else
    echo -e "\033[33mContainer ${container_name} is still running.\033[0m"
fi
