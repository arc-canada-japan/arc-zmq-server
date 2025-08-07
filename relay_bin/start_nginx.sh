#!/bin/sh -x

OSCODE=`lsb_release -sc`

if [ "x${OSCODE}" = "xfocal" ] || [ "x${OSCODE}" = "xjammy" ] ; then
   sudo nginx -c ${PWD}/nginx.conf
else
    sudo nginx -c ${PWD}/nginx.conf -e ${PWD}/nginx.log
fi
