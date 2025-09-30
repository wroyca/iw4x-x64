#pragma once

#include <array>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

extern "C"
{
  #include <io.h>
}

namespace iw4x
{
  using std::array;
  using std::call_once;
  using std::cerr;
  using std::cout;
  using std::dec;
  using std::endl;
  using std::exception;
  using std::function;
  using std::hex;
  using std::ios;
  using std::mutex;
  using std::once_flag;
  using std::ostringstream;
  using std::runtime_error;
  using std::scoped_lock;
  using std::string;
  using std::terminate;
  using std::to_string;
  using std::unique_ptr;
  using std::vector;

  using cstring = char*;
}
