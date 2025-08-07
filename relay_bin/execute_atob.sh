#!/bin/sh -x

OSCODE=`lsb_release -sc`.`uname -m`
ATOB_PORT=1600

if [ -e prx0ws ]; then
    ./prx0ws ${ATOB_PORT}
elif [ -e prx0ws.${OSCODE} ]; then
    ./prx0ws.${OSCODE} ${ATOB_PORT}
fi


#./prx0ws 1600
