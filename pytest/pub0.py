#!/usr/bin/env python3

import zmq
import time

def main():

    addr = 'ws://127.111.111.111:80/atobpub'
    ctx = zmq.Context()
    pub = ctx.socket(zmq.PUB)
    pub.connect(addr)

    count=0

    while True:
        msg = "Pyzmq/pub0 %d"%(count)
        pub.send_string(msg)
        count += 1
        print(msg)
        time.sleep(0.5)
        pass

    return


if __name__=='__main__':
    main()
    pass
