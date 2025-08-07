//
// handshake.h
//

#ifndef MYUTIL_HANDSHAKE_H_
#define MYUTIL_HANDSHAKE_H_

#include <mutex>
#include <condition_variable>
#include <cstdint>

namespace myutil {

class Handshake {
 private:
  std::uint64_t count_;
  std::mutex mtx_;
  std::condition_variable cvar_;

 public:
  Handshake() : count_(0) { }
  virtual ~Handshake() { }

  void notify_all(void) {
    mtx_.lock();
    count_++;
    cvar_.notify_all();
    mtx_.unlock();
  }

  std::uint64_t wait(std::uint64_t prev) {
    std::unique_lock<std::mutex> lock(mtx_);
    while(prev == count_) {
      cvar_.wait(lock);
    }
    return count_;
  }
    
};

}  // namespace myutil

#endif  // MYUTIL_HANDSHAKE_H_
