#!/bin/sh -x

. ./00vars.sh


/bin/rm -rf build_sideAB_cpp build_prx0ws_cpp
/bin/rm -rf sideAB_cpp/${ZEROMQ_BASE}
/bin/rm -rf sideAB_cpp/${CPPZMQ_BASE}
/bin/rm -rf sideAB_cpp/${MSGPACK_BASE}
/bin/rm -rf prx0ws_cpp/${ZEROMQ_BASE}

/bin/rm -rf libs/build libs/src
#/bin/rm -rf libs/target_* 

/bin/rm -f *~ */*~ */*/*~



#/bin/rm -rf archives/${ZEROMQ_BASE}
#/bin/rm -rf archives/build_zeromq
