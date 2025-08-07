
#include <cstdio>
#include <zmq.hpp>


int main(int argc, char *argv[]) {

  int xsub_port = 2000;
  if (argc>1) {
    sscanf(argv[1], "%i", &xsub_port);
  }
  int xpub_port = xsub_port+1;

  int kBufSize=32;
  char xsub_addr[kBufSize];
  std::snprintf(xsub_addr, kBufSize, "ws://*:%d", xsub_port);
  char xpub_addr[kBufSize];
  std::snprintf(xpub_addr, kBufSize, "ws://*:%d", xpub_port);

  zmq::context_t zctx;
  
  zmq::socket_t xsub(zctx, zmq::socket_type::xsub);
  xsub.bind(xsub_addr);
  fprintf(stderr, "-- xsub: '%s'\n", xsub_addr);

  zmq::socket_t xpub(zctx, zmq::socket_type::xpub);
  xpub.bind(xpub_addr);
  fprintf(stderr, "-- xpub: '%s'\n", xpub_addr);

  zmq::proxy(xpub, xsub);

  return 0;
}
