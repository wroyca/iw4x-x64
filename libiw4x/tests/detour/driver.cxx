#include <iostream>
#include <cstdint>

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
      odb::database &d (db ());
      try
      {
        transaction txn (db);
        odb::transaction &t (txn ());

        uintptr_t a (0x000000);
        uintptr_t b (0x000000);

        txn.attach (a, b);
        t.commit ();
      }
      catch (const transaction_error &ex)
      {
        ex.print (cerr);
        return 1;
      }
      catch (const operation_error &ex)
      {
        ex.print (cerr);
        return 1;
      }
    }
    catch (const database_error &ex)
    {
      ex.print (cerr);
      return 1;
    }
  }
  catch (const detour::exception &ex)
  {
    ex.print (cerr);
    return 1;
  }
}
