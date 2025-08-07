#!/bin/sh -x

OSCODE=`lsb_release -sc`.`uname -m`
BTOA_PORT=1602

if [ -e prx0ws ]; then
    ./prx0ws ${BTOA_PORT}
elif [ -e prx0ws.${OSCODE} ]; then
    ./prx0ws.${OSCODE} ${BTOA_PORT}
fi

#./prx0ws 1602

