#include "base/init_ghost.h"

#include <glog/logging.h>

#include <csignal>

namespace base {

void InitGhost(int* argc, char*** argv) {
  // Socket RST could result in SIGPIPE, which we should tolerate.
  // https://blog.erratasec.com/2018/10/tcpip-sockets-and-sigpipe.html
  std::signal(SIGPIPE, SIG_IGN);
}

}  // namespace base

