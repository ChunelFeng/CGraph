FROM ubuntu:18.04
LABEL maintainer=woodx<woodx9@163.com>

ENV CGRAPH_PATH /home/CGraph
WORKDIR $CGRAPH_PATH

RUN apt-get -y upgrade
RUN apt-get -y update
RUN apt-get -y install cmake
RUN apt-get -y install make
RUN apt-get -y install g++

CMD /bin/sh -c "/bin/bash"
