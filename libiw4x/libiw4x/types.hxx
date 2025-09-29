#pragma once

extern "C"
{
  #include <io.h>
}

#include <array>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

namespace iw4x
{
  using std::array;
  using std::call_once;
  using std::cerr;
  using std::cout;
  using std::dec;
  using std::endl;
  using std::exception;
  using std::hex;
  using std::mutex;
  using std::once_flag;
  using std::ostringstream;
  using std::runtime_error;
  using std::scoped_lock;
  using std::string;
  using std::terminate;
  using std::unique_ptr;
  using std::vector;
  using std::function;
}
