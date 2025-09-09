#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

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

  // forward declarations
  //
  class operation;

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

    template <typename... Args> void
    attach (Args &&...args);

  private:
    std::unique_ptr<odb::transaction> txn_;
    std::vector<operation> operations;
  };

  // operation
  //
  class LIBIW4X_SYMEXPORT operation
  {
  public:
    explicit operation (transaction &txn, std::function<void ()> operation);
    explicit operation (transaction &txn, std::uintptr_t source, std::uintptr_t destination);

    template <typename Self>
    decltype (auto)
    operator() (this Self &&self)
    {
      if (!self.operation_)
        throw operation_error ("operation not initialized");

      self.operation_ ();
    }

  private:
    transaction &txn_;
    std::function<void ()> operation_;
  };
}

namespace iw4x
{
  // transaction
  //

  template <typename... Args> void transaction::
  attach (Args &&...args)
  {
    // The transaction log must capture *intent* rather than *effect*. If we
    // were to instantiate an operation here, then sequencing would already be
    // compromised: part of the work would have escaped the control of the
    // transaction. Instead, we defer construction into a callable to record
    // only the intent and leave realization to the transaction's execution
    // phase.
    //
    // Arguments are decayed and stored by value to guarantee that intent is
    // self-contained. A transaction cannot rely on the caller's stack frame or
    // reference categories surviving until commit. What we require is not
    // perfect preservation of type fidelity but reproducibility of the same
    // operation when replayed at a later stage (commit or rollback).
    //
    // The lambda `o` is therefore a suspension: it binds the transaction and
    // its frozen arguments, but it does not execute. Later, when the log is
    // drained, it can be invoked in the sequence defined by the transaction and
    // all operations will be realized under the same authority and in the same
    // order.
    //
    // Every deferred callable is then normalized into an `operation` object
    // before insertion into the log. That is, the log only need to reason about
    // a single abstraction. Mixing raw lambdas and operations would fracture
    // semantics and make correctness dependent on call-site convention.
    //
    auto o ([&txn = *this,
                     args = std::make_tuple (std::decay_t<Args> (args)...)]
    {
      std::apply ([&txn] (auto &&...args)
      {
        operation _ (txn, std::forward<decltype (args)> (args)...);
      }, args);
    });

    operations.emplace_back (operation (*this, std::move (o)));
  }
}
