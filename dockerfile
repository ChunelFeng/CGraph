FROM ubuntu:18.04
MAINTAINER woodx<woodx9@163.com>

ENV MYPATH /home/CGraph
WORKDIR $MYPATH

RUN apt -y update
RUN apt -y install cmake
RUN apt -y install make
RUN apt -y install g++

CMD /bin/sh -c "/bin/bash"

