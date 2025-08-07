//
//  https://github.com/stbrumme/hash-library
//

#include <array>
#include <random>
#include <string>
#include <thread>
#include <optional>
#include <cstdint>
#include <cstdio>
#include <ctime>       // clock_gettime(), localtime_r(), clock_nanosleep()
#include <sys/time.h>  // struct timespec
#include <sys/stat.h>  // mkdir(), stat()
#include <pthread.h>   // pthread_schedsetparam()
#include <sched.h>     // sched_get_priority_max()

#include <zmq.hpp>
#include <msgpack.hpp>
#include "conf.h"
#include "sample_data.h"
#include "handshake.h"

namespace twe = test_websocket_echoback;

//const int kSampleNum = 1000;
//const int kLoopNum = 10;

std::vector<twe::SampleData_t> tx_data;
std::vector<msgpack::sbuffer> tx_buf;
std::vector<char> rx_buf;
std::vector<twe::SampleData_t> rx_data;

void randomize(int flood_size) {
  std::random_device rng;
  std::uniform_real_distribution<twe::SampleFloatType> dist(-1.0, 1.0);
  
  for(int i=0; i<flood_size; i++) {
    twe::SampleData_t &datum = tx_data[i];

    for(size_t dof=0; dof<datum.data.size(); dof++) {
      datum.data[dof] = dist(rng);
    }
  }
}

inline void add_interval(struct timespec *sp, int interval) {
  std::int64_t nsec = sp->tv_nsec + interval;
  while (nsec >= 1000000000) {
    sp->tv_sec  += 1;
    sp->tv_nsec -= 1000000000;
  }
  sp->tv_nsec = nsec;
}


void sender(myutil::Handshake *hs, twe::Conf &conf) {
  std::string pub_addr = conf.get<std::string>("pub_addr");
  int flood_size = conf.get<int>("flood_size");
  int loop_num = conf.get<int>("loop_num");
  int interval = conf.get<int>("interval_ms")*1000;  // nanoseconds
  if (interval < 1000) interval=1000; 
  
  std::uint64_t hs_count=0;

  zmq::context_t zctx;
  zmq::socket_t pubskt(zctx, zmq::socket_type::pub);
  pubskt.connect(pub_addr);
  std::fprintf(stderr, "-- sender: pub to '%s'\n", pub_addr.c_str());

  std::FILE *fp = std::fopen("sender.csv", "w");
  struct timespec next_tick;
  clock_gettime(CLOCK_MONOTONIC, &next_tick);
  add_interval(&next_tick, interval);
  
  for(int loop=0; loop<loop_num; loop++) {
    // refresh random data
    randomize(flood_size);

    // wait for a signal from receiver
    hs_count = hs->wait(hs_count);    printf("-- got signal\n");
    
    // send dummy data at the begining (I don't knoy why but the first published data will be lost)
    if (0) {  // if (loop == 0) {
      tx_buf[0].clear();
      msgpack::pack(tx_buf[0], tx_data[0]);
      //zmq::const_buffer zbuf = ;
      pubskt.send(zmq::buffer(tx_buf[0].data(), tx_buf[0].size()), zmq::send_flags::none);
      usleep(1000);
    }

    // wait a cycle time
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_tick, NULL);
    
    // flood publish data
    for(int idx=0; idx<flood_size; idx++) {
      // aliases
      twe::SampleData_t &datum = tx_data[idx];
      msgpack::sbuffer &txbuf = tx_buf[idx];

      // record a timestamp
      datum.timestamp(0);
      
      // pack
      txbuf.clear();
      msgpack::pack(txbuf, datum);
      
      // publish
      pubskt.send(zmq::buffer(txbuf.data(), txbuf.size()), zmq::send_flags::none);

#if 0
      {
        char dirname[64];
        std::snprintf(dirname, 64, "txdata_%06d", loop);
        mkdir(dirname, 0755);
        
        char fname[128];
        std::snprintf(fname, 128, "%s/txdata_%06d_%05d.dat", dirname, loop, i);
        FILE *mf = std::fopen(fname, "w");
        std::fwrite(tx_buf[i].data(), tx_buf[i].size(), 1, mf);
        std::fclose(mf);
      }
#endif
      
    }

    // update next cycle tick
    add_interval(&next_tick, interval);
    
    // save a log file
    for(int i=0; i<flood_size; i++) {
      tx_data[i].dump_csvrow(fp);
    }

  }  // for loop<loop_num

  std::fclose(fp);
}


void receiver(myutil::Handshake *hs, const twe::Conf &conf) {
  std::string sub_addr = conf.get<std::string>("sub_addr");
  int flood_size = conf.get<int>("flood_size");
  int loop_num = conf.get<int>("loop_num");
  
  zmq::context_t zctx;
  zmq::socket_t subskt(zctx, zmq::socket_type::sub);
  subskt.connect(sub_addr);
  subskt.set(zmq::sockopt::subscribe, "");
  std::fprintf(stderr, "-- receiver: sub from '%s'\n", sub_addr.c_str());
  
  std::FILE *fp = std::fopen("receiver.csv", "w");

  for(int loop=0; loop<loop_num; loop++) {
    hs->notify_all();  printf("-- notify\n");

    for(int i=0; i<flood_size; i++) {
#if 0
      char dirname[64];
      std::snprintf(dirname, 64, "txdata_%06d", loop);
      mkdir(dirname, 0755);
      
      char fpath[128];
      std::snprintf(fpath, 128, "%s/txdata_%06d_%05d.dat", dirname, loop, i);
      
      struct stat stdat;
      lstat(fpath, &stdat);
      int rsize = stdat.st_size;
      //printf("-- rsize: %d\n", rsize);
      char rbuf[rsize];
      
      FILE *mf = std::fopen(fpath, "r");
      std::fread(rbuf, rsize, 1, mf);
      std::fclose(mf);
#endif

      // receive a subscribe datum
      zmq::message_t msg;
      auto ret = subskt.recv(msg, zmq::recv_flags::none);  // zmq::recv_result_t (std::optional<size_t>)
      
      if (ret) {
        // unpack
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char *>(msg.data()), msg.size()); //  unpack(rbuf, rsize);
        const msgpack::object &obj = oh.get();
        obj.convert(rx_data[i]);

        // record a timestamp
        rx_data[i].timestamp(2);

        //printf("-- %4d/%06d\n", loop, i);
      }
    }

    for(int i=0; i<flood_size; i++) {
      rx_data[i].dump_csvrow(fp);
    }
      
  }  // for loop<loop_num

  std::fclose(fp);

}


void set_schedparam(void) {
  struct sched_param sp;
  sp.sched_priority = sched_get_priority_max(SCHED_FIFO) - 5;
  int ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
  if (ret != 0) {
    char mesg[128];
    snprintf(mesg, 128, "++ failed to pthread_setschedparam(): %s", std::strerror(ret));
    std::fputs(mesg, stderr);
    std::fputs("\n", stderr);
  }
}

int main(int argc, char *argv[]) {

  const char *conf_path="conf.yml";
  if (argc > 1) conf_path = argv[1];
  twe::Conf conf("sideA", conf_path);

  set_schedparam();

  {
    int flood_size = conf.get<int>("flood_size");
    tx_data.resize(flood_size);
    tx_buf.resize(flood_size);
    rx_buf.resize(flood_size);
    rx_data.resize(flood_size);

    int data_size = conf.get<int>("data_size");
    for(unsigned int idx=0; idx<tx_data.size(); idx++) {
      tx_data[idx].data.resize(data_size);
      rx_data[idx].data.resize(data_size);
    }
    std::printf("-- resized flood_sizes: %d, data_size: %d\n", flood_size, data_size);
  }
  
#if 0
  for (auto key : { "pub_addr", "sub_addr"}) {
    std::printf("-- '%s': %s\n", key, conf.get<std::string>(key).c_str());
  }

  for (auto key: { "data_size", "flood_size", "interval_ms", "loop_num" }) {
    std::printf("-- '%s': %d\n", key, conf.get<int>(key));
  }
#endif
  
  myutil::Handshake hs;

  std::thread rcvr(receiver, &hs, conf);

  sender(&hs, conf);

  rcvr.join();
  
  return 0;
}
