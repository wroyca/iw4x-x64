#pragma once

#include <exception>
#include <ostream>
#include <string>
#include <memory>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <libiw4x/export.hxx>

namespace iw4x
{
  namespace detour
  {
    // Exceptions in this subsystem are represented not by raw strings or the
    // standard exception hierarchy but by a dedicated interface. The rationale
    // is that detour-specific errors typically carry richer semantics than a
    // diagnostic message. In particular, many error conditions are best
    // understood when formatted with context (e.g., which symbol failed to
    // patch, what range was invalid, which system call returned an unexpected
    // result).
    //
    class LIBIW4X_SYMEXPORT exception : public std::exception
    {
    public:
      // Each concrete exception must be able to describe itself in human-
      // readable form. The `print()` contract is that the method should write a
      // complete, self-contained diagnostic message into the supplied stream
      // without throwing.
      //
      virtual void
      print (std::ostream &) const = 0;
    };
  }

  // Stream insertion operator
  //
  // Provides uniform syntax for emitting detour exceptions. Internally this
  // simply delegates to `print()`, thereby ensuring that any user-defined
  // exception type has its own formatting logic applied consistently. Note
  // that we return the stream to preserve operator chaining semantics.
  //
  std::ostream&
  operator<< (std::ostream&, const detour::exception&);

  // database_error
  //
  class LIBIW4X_SYMEXPORT database_error : public detour::exception
  {
  public:
    explicit database_error (const std::string &message);

    const std::string &
    message () const noexcept;

    virtual void
    print (std::ostream &) const;

    virtual const char *
    what () const noexcept;

  private:
    std::string message_;
  };

  // transaction_error
  //
  class LIBIW4X_SYMEXPORT transaction_error : public detour::exception
  {
  public:
    explicit transaction_error (const std::string &message);

    const std::string &
    message () const noexcept;

    virtual void
    print (std::ostream &) const;

    virtual const char *
    what () const noexcept;

  private:
    std::string message_;
  };

  // operation_error
  //
  class LIBIW4X_SYMEXPORT operation_error : public detour::exception
  {
  public:
    explicit operation_error (const std::string &message);

    const std::string &
    message () const noexcept;

    virtual void
    print (std::ostream &) const;

    virtual const char *
    what () const noexcept;

  private:
    std::string message_;
  };

  // database
  //
  class LIBIW4X_SYMEXPORT database
  {
  public:
    database ();

    template <typename Self> decltype (auto)
    operator() (this Self &&self)
    {
      if (!self.db_)
        throw database_error ("database not initialized");

      return (*self.db_);
    }

  private:
    std::unique_ptr<odb::database> db_;
  };

  // transaction
  //
  class LIBIW4X_SYMEXPORT transaction
  {
  public:
    explicit
    transaction (database &db);

    // Access underlying ODB transaction
    //
    template <typename Self> decltype (auto)
    operator() (this Self &&self)
    {
      if (!self.txn_)
        throw transaction_error ("transaction not initialized");

      return (*self.txn_);
    }

  private:
    std::unique_ptr<odb::transaction> txn_;
  };

  // operation
  //
  class LIBIW4X_SYMEXPORT operation
  {
  public:
    explicit
    operation (transaction &txn);

  private:
    transaction &txn_;
  };
}
