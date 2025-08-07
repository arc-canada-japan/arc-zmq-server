#!/usr/bin/env python3

import zmq
import time

def main(useWebSocket=False):

    addr = 'ws://127.111.111.111:80/atobsub'
    
    ctx = zmq.Context()
    sub = ctx.socket(zmq.SUB)
    sub.connect(addr)
    sub.setsockopt_string(zmq.SUBSCRIBE, '')

    while True:
        msg =sub.recv_string()
        print(msg)
        pass

    return


if __name__=='__main__':
    main()
    pass

