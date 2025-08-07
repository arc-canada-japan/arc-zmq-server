#!/bin/sh -x

. ./00vars.sh

OSCODE=`lsb_release -sc`

SRCDIR=`realpath ${PWD}/libs/src`
BLDDIR=`realpath ${PWD}/libs/build`
ARCDIR=`realpath ${PWD}/libs/archives`

CMAKEOPTS_COMMON="-DCMAKE_INSTALL_PREFIX=${TGTDIR}"
ZEROMQ_CMAKEOPTS="-DENABLE_DRAFTS=ON -DBUILD_SHARED=OFF -DBUILD_STATIC=ON -DBUILD_TESTS=OFF ${CMAKEOPTS_COMMON}"
CPPZMQ_CMAKEOPTS="-DENABLE_DRAFTS=ON -DCPPZMQ_BUILD_TESTS=OFF ${CMAKEOPTS_COMMON}"
MSGPACK_CMAKEOPTS="-DMSGPACK_CXX11=ON -DMSGPACK_USE_BOOST=ON -DMSGPACK_BUILD_TESTS=OFF -DMSGPACK_USE_STATIC_BOOST=OFF ${CMAKEOPTS_COMMON}"
YAMLCPP_CMAKEOPTS="-DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=ON -DYAML_BUILD_SHARED_LIBS=OFF -DYAML_CPP_BUILD_TESTS=OFF ${CMAKEOPTS_COMMON}"


#
# base dirs
#
if [ ! -d ${SRCDIR} ]; then
    mkdir ${SRCDIR}
fi

if [ ! -d ${TGTDIR} ]; then
    mkdir ${TGTDIR}
fi


#
# download files if needed
#
( cd ${ARCDIR}; ./00wget.sh )

#
# install essential packages if needed
#
PKGS="build-essential cmake pkg-config libboost-dev"
PKGS_NUM=`echo ${PKGS} | wc -w`
PKGS_NUM_INSTALLED=`apt -qq list ${PKGS} | grep installed | wc -l`

if [ ${PKGS_NUM_INSTALLED} != ${PKGS_NUM} ]; then
    sudo apt update
    sudo apt install ${PKGS}
fi

#
# libzmq
#
ZEROMQ_SRCDIR=${SRCDIR}/${ZEROMQ_BASE}
ZEROMQ_BLDDIR=${BLDDIR}/${ZEROMQ_BASE}
if [ ! -d ${ZEROMQ_SRCDIR} ]; then
    (cd ${SRCDIR}; tar xvfz ${ARCDIR}/${ZEROMQ_TAR})
fi

if [ ! -d ${ZEROMQ_BLDDIR} ]; then
    mkdir -p ${ZEROMQ_BLDDIR}
fi

(cd ${ZEROMQ_BLDDIR}; cmake ${ZEROMQ_CMAKEOPTS} ${ZEROMQ_SRCDIR}; cmake --build . ; cmake --install . )


#
# cppzmq (only headers, no binary libs)
#
CPPZMQ_SRCDIR=${SRCDIR}/${CPPZMQ_BASE}
CPPZMQ_BLDDIR=${BLDDIR}/${CPPZMQ_BASE}
if [ ! -d ${CPPZMQ_SRCDIR} ]; then
    (cd ${SRCDIR}; tar xvfz ${ARCDIR}/${CPPZMQ_TAR})
fi

if [ ! -d ${CPPZMQ_BLDDIR} ]; then
    mkdir -p ${CPPZMQ_BLDDIR}
fi

(cd ${CPPZMQ_BLDDIR}; cmake ${CPPZMQ_CMAKEOPTS} ${CPPZMQ_SRCDIR}; cmake --install . )


#
# msgpack (only headers, no binary libs)
#
MSGPACK_SRCDIR=${SRCDIR}/${MSGPACK_BASE}
MSGPACK_BLDDIR=${BLDDIR}/${MSGPACK_BASE}
if [ ! -d ${MSGPACK_SRCDIR} ]; then
    (cd ${SRCDIR}; tar xvfz ${ARCDIR}/${MSGPACK_TAR})
fi

if [ ! -d ${MSGPACK_BLDDIR} ]; then
    mkdir -p ${MSGPACK_BLDDIR}
fi

(cd ${MSGPACK_BLDDIR}; cmake ${MSGPACK_CMAKEOPTS} ${MSGPACK_SRCDIR}; cmake --install . )


#
# yaml-cpp
#
YAMLCPP_SRCDIR=${SRCDIR}/${YAMLCPP_BASE}
YAMLCPP_BLDDIR=${BLDDIR}/${YAMLCPP_BASE}
if [ ! -d ${YAMLCPP_SRCDIR} ]; then
    (cd ${SRCDIR}; tar xvfz ${ARCDIR}/${YAMLCPP_TAR})
fi

if [ ! -d ${YAMLCPP_BLDDIR} ]; then
    mkdir -p ${YAMLCPP_BLDDIR}
fi

(cd ${YAMLCPP_BLDDIR}; cmake  ${YAMLCPP_CMAKEOPTS} ${YAMLCPP_SRCDIR}; cmake --build . ; cmake --install . )
