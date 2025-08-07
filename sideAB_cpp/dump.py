#!/usr/bin/env python3

import msgpack
import sys

def dump(fpath):
    print(f"--- {fpath}")
    print(msgpack.unpack(open(fpath, "rb")))
    return

if __name__=='__main__':
    if (len(sys.argv)>1):
        for fpath in sys.argv[1:]:
            dump(fpath)
            pass
    else:
        dump("txdata_000000/txdata_000000_00999.dat");
        pass
    pass





      
