#include <iostream>
#include <string>

#include <libiw4x/version.hxx>

using namespace std;

int
main (int argc, char* argv[])
{
  if (argc > 1 && string (argv[1]) == "--version")
  {
    cout << LIBIW4X_VERSION_STR << endl;
  }
}
