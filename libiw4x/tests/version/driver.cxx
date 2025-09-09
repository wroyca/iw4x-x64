#include <iostream>
#include <string>

#include <libiw4x/version.hxx>

#undef NDEBUG
#include <cassert>

int
main (int argc, char* argv[])
{
  using namespace std;

  if (argc > 1 && string (argv[1]) == "--version")
  {
    cout << LIBIW4X_VERSION_STR << endl;
  }
}
