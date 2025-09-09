#include <libiw4x/detour.hxx>

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
    // ...
  }

  // transaction
  //

  transaction::
  transaction ()
  {
    // ...
  }

  // operation
  //

  operation::
  operation ()
  {
    // ...
  }
}
