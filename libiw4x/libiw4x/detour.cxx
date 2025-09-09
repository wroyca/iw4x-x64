#include <libiw4x/detour.hxx>

#include <odb/sqlite/database.hxx>
#include <odb/sqlite/exceptions.hxx>

using namespace std;

namespace iw4x
{
  ostream &
  operator<< (ostream &os, const detour::exception &ex)
  {
    ex.print (os);
    return os;
  }

  // database_error
  //

  database_error::
  database_error (const string &m)
    : message_ (m)
  {
  }

  const string &database_error::
  message () const noexcept
  {
    return message_;
  }

  void database_error::
  print (ostream &os) const
  {
    os << "transaction error '" << message ().c_str () << "'";
  }

  const char *database_error::
  what () const noexcept
  {
    return message_.c_str ();
  }

  // transaction_error
  //

  transaction_error::
  transaction_error (const string &m)
    : message_ (m)
  {
  }

  const string &transaction_error::
  message () const noexcept
  {
    return message_;
  }

  void transaction_error::
  print (ostream &os) const
  {
    os << "transaction error '" << message ().c_str () << "'";
  }

  const char *transaction_error::
  what () const noexcept
  {
    return message_.c_str ();
  }

  // operation_error
  //

  operation_error::
  operation_error (const string &m)
    : message_ (m)
  {
  }

  const string &operation_error::
  message () const noexcept
  {
    return message_;
  }

  void operation_error::
  print (ostream &os) const
  {
    os << "transaction error '" << message ().c_str () << "'";
  }

  const char *operation_error::
  what () const noexcept
  {
    return message_.c_str ();
  }

  // database
  //

  database::
  database ()
  {
    try
    {
      // Create in-memory SQLite database as shown in ODB manual section 18.2
      //
      db_ = make_unique<odb::sqlite::database> (
        ":memory:",
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
      );
    }
    catch (const odb::exception& e)
    {
      throw database_error (format ("unable to create database: {}", e.what ()));
    }
  }

  // transaction
  //

  transaction::
  transaction (database& db)
  {
    try
    {
      // Begin transaction on database following ODB manual section 3.5
      //
      txn_ = make_unique<odb::transaction> (
        db ().begin ()
      );
    }
    catch (const odb::exception& e)
    {
      throw transaction_error (format ("unable to begin transaction: {}", e.what ()));
    }
  }
}
