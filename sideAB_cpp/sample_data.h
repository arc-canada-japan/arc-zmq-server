//
// sample_data.h
//
//

#ifndef TEST_WEBSOCKET_ECHOBACK_SAMPLE_DATA_H_
#define TEST_WEBSOCKET_ECHOBACK_SAMPLE_DATA_H_

#include <array>
#include <vector>
#include <chrono>
#include <msgpack.hpp>
#include <sys/time.h>
#include <ctime>
#include <cstdint>

#ifdef USE_DUMP
#include "sha256.h"
#endif

namespace test_websocket_echoback {

template <typename FloatType, int default_data_size> struct SampleData_st {
  std::vector<FloatType> data;
  //std::array<struct timespec, 3> laptime;
  std::array<std::chrono::system_clock::time_point, 3> laptime;
  MSGPACK_DEFINE(data, laptime);

  void timestamp(unsigned int slot) {
    if (slot < 3) laptime[slot] = std::chrono::system_clock::now();
    //if (slot < 3) clock_gettime(CLOCK_REALTIME, &(laptime[slot]));
  }

  SampleData_st() {
    data.resize(default_data_size);
  }
  
#ifdef USE_DUMP
  void dump_csvrow(std::FILE *fp) {
    SHA256 sha256;
    sha256.add(data.data(), sizeof(FloatType) * data.size());
    std::string hashstr = sha256.getHash();

    {
      for (auto tp : laptime) {
#if 1  // chrono::system_clock::time_point
        std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp2 = std::chrono::time_point_cast<std::chrono::seconds>(tp);
        std::chrono::duration<double> subsecs = tp-tp2;
        std::time_t secs = std::chrono::system_clock::to_time_t(tp);
        std::uint32_t nanosecs = subsecs.count()*1000000000;
#else   // struct timespec
        std::time_t secs = tp.tv_sec;
        std::uint32_t nanosecs = tp.tv_nsec;
#endif
        std::tm gt;
        gmtime_r(&(secs), &gt);

        const int  kBufSize=128;
        char buf[kBufSize];
        std::strftime(buf, kBufSize, "%F %T.", &gt);
        std::fputs(buf, fp);

        std::snprintf(buf, kBufSize, "%09d", nanosecs);
        buf[6]=0;  // first 6 charactors
        std::fputs(buf, fp);
        std::fputs("Z,", fp);
      }
    }

    std::fputs(hashstr.c_str(), fp);
    std::fputs("\n", fp);
  
  }
#endif  // ifdef USE_DUMP

  
};  // struct SampleData_st<>


//
// SampleData_t
//

//const int kSampleDOF = 32;
const int kDefaultDataSize = 32;
typedef double SampleFloatType;
//typedef float SampleFloatType;

typedef struct SampleData_st<SampleFloatType, kDefaultDataSize> SampleData_t;


}  // namespace test_websocket_echoback

#endif  // TEST_WEBSOCKET_ECHOBACK_SAMPLE_DATA_H_
