//
//  https://github.com/stbrumme/hash-library
//

#include <array>
#include <cstdio>
#include <ctime>       // clock_gettime(), localtime_r(),

#include <zmq.hpp>
#include <msgpack.hpp>
#include "conf.h"
#include "sample_data.h"

namespace twe = test_websocket_echoback;

const int kBufSize = 10000;

std::array<twe::SampleData_t, kBufSize> rx_data;
std::array<msgpack::sbuffer, kBufSize> tx_buf;


void echoback(twe::Conf &conf) {

  std::string sub_addr = conf.get<std::string>("sub_addr");
  std::string pub_addr = conf.get<std::string>("pub_addr");

  zmq::context_t zctx;
  zmq::socket_t subskt(zctx, zmq::socket_type::sub);
  subskt.connect(sub_addr);
  subskt.set(zmq::sockopt::subscribe, "");

  zmq::socket_t pubskt(zctx, zmq::socket_type::pub);
  pubskt.connect(pub_addr);
  std::fprintf(stderr, "-- echoback: from '%s' to '%s'\n", sub_addr.c_str(), pub_addr.c_str());

  int idx=0;
  while (true) {

    // receive
    zmq::message_t msg;
    auto ret = subskt.recv(msg, zmq::recv_flags::none);

    if (ret) {
      // unpack
      msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char *>(msg.data()), msg.size()); //  unpack(rbuf, rsize);
      const msgpack::object &obj = oh.get();
      obj.convert(rx_data[idx]);
      
      // record a timestamp
      rx_data[idx].timestamp(1);
      
      // pack
      tx_buf[idx].clear();
      msgpack::pack(tx_buf[idx], rx_data[idx]);
      
      // send
      pubskt.send(zmq::buffer(tx_buf[idx].data(), tx_buf[idx].size()), zmq::send_flags::none);
      
      // ring buffering
      if (++idx == kBufSize) idx=0;
    }  // if (ret)
  }  // while (true)
}

int main(int argc, char *argv[]) {

  const char *conf_path="conf.yml";
  if (argc > 1) conf_path = argv[1];
  twe::Conf conf("sideB", conf_path);

  //const char *sub_addr="ws://127.0.0.1:80/atobsub";
  //const char *pub_addr="ws://127.0.0.1:80/btoapub";

  echoback(conf);
  
  return 0;
}
