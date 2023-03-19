#!/bin/bash

image_name="cgraph_env"
#获取脚本所在目录，即/CGraph/
workdir=$(cd `dirname $0`;pwd)


#判断cgraph_env镜像是否存在，没有就创建
if docker images "$image_name" | grep -q "$image_name"; then
    #有了就不用再build iamge了
    echo "Found image"
else
    #build docker env 
    docker build -f $workdir/dockerfile -t $image_name $workdir
fi

#判断有没有容器存在，容器名：用户名_CGraph
#有容器则进入
#没有容器就创建一个

container_name="${USER}_CGraph"

if docker ps -a | grep -q "$container_name"; then
    docker start -i $container_name
else
    #run docker container  
    docker run -it --name="$container_name" -v ${wokdir}:/home/CGraph cgraph_env
fi


