#include <iostream>

#include <libiw4x/detour.hxx>

#undef NDEBUG
#include <cassert>

int
main ()
{
  using namespace std;
  using namespace iw4x;

  try
  {
    database db;

    try
    {
      auto& d (db ());
    }

    catch (const database_error& ex)
    {
      ex.print (cerr);
      return 1;
    }
  }

  catch (const detour::exception& ex)
  {
    ex.print(cerr);
    return 1;
  }
}
