#include <libiw4x/detour.hxx>

namespace iw4x
{
  namespace detour
  {
    // transaction_error
    //

    transaction_error::
    transaction_error (const std::string &message)
      : message_ (message)
    {
    }

    transaction_error::
    ~transaction_error () noexcept
    {
    }

    const std::string &transaction_error::
    message () const
    {
      return message_;
    }

    void transaction_error::
    print (std::ostream &os) const
    {
      os << "transaction error '" << message ().c_str () << "'";
    }

    const char *transaction_error::
    what () const noexcept
    {
      return "transaction error";
    }
  }
}
