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
      odb::database &d (db ());
      try
      {
        transaction txn (db);
        odb::transaction &t (txn ());

        txn.attach ("test_param", 42);
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
