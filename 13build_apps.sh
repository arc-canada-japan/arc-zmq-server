#!/bin/sh -x

#(cd archives; ./00wget.sh)

. ./00vars.sh

CMAKEOPTS="-DCMAKE_PREFIX_PATH=${TGTDIR}"
#CMAKEOPTS="-DCMAKE_SYSTEM_PREFIX_PATH=${TGTDIR} -DCMAKE_INSTALL_PREFIX=${TGTDIR}"

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
# libs if needed
#
if [ ! -d ${TGTDIR} ]; then
    ./11build_libs.sh
fi


#
# sideAB
#
rm -rf build_sideAB_cpp
mkdir build_sideAB_cpp
(cd build_sideAB_cpp; cmake ${CMAKEOPTS} ../sideAB_cpp; cmake --build .)
(cp sideAB_cpp/conf.yml build_sideAB_cpp)

#
# prx0ws
#
rm -rf build_prx0ws_cpp
mkdir build_prx0ws_cpp
(cd build_prx0ws_cpp; cmake ${CMAKEOPTS} ../prx0ws_cpp; cmake --build .)

cp build_prx0ws_cpp/prx0ws relay_bin/

