#!/bin/sh -x

. ../../00vars.sh

if [ ! -x /usr/bin/wget ]; then
    sudo apt install wget
fi

if [ ! -f ${ZEROMQ_TAR} ]; then
    wget https://github.com/zeromq/libzmq/releases/download/v${ZEROMQ_VER}/${ZEROMQ_TAR}
fi

if [ ! -f ${CPPZMQ_TAR} ]; then
    wget -O ${CPPZMQ_TAR} https://github.com/zeromq/cppzmq/archive/refs/tags/v${CPPZMQ_VER}.tar.gz
fi

if [ ! -f ${MSGPACK_TAR} ]; then
    wget https://github.com/msgpack/msgpack-c/releases/download/cpp-${MSGPACK_VER}/${MSGPACK_TAR}
fi

if [ ! -f ${YAMLCPP_TAR} ]; then
    wget -O ${YAMLCPP_TAR} https://github.com/jbeder/yaml-cpp/archive/refs/tags/${YAMLCPP_VER}.tar.gz
fi
