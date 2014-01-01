// customer.h                                                         -*-C++-*-
#ifndef INCLUDED_CUSTOMER
#define INCLUDED_CUSTOMER

//@PURPOSE: Provide an attribute class for characterizing a bank's customers.
//
//@CLASSES:
//  pkg::Customer: attributes characterizing a bank customer
//
//@SEE_ALSO:
//
//@DESCRIPTION: This component provides a single, simply constrained
// (value-semantic) attribute class, 'pkg::Customer', that is used to
// characterize the customers of a bank.
//
///Attributes
///----------
//..
//  Name                Type              Default  Simple Constraints
//  ------------------  -----------       -------  ------------------
//  firstName           bsl::string       ""       none
//  lastName            bsl::string       ""       none
//  accounts            bsl::vector<int>  0        none
//  id                  int               0        id > 0
//..
//: o 'firstName': The first name of the customer.
//:
//: o 'lastName': The last name of the customer.
//:
//: o 'accounts': The list of accounts associated with a customer.
//:
//: o 'id': The customer's identification number.
//
///Usage
///-----
// This section illustrates intended use of this component.

#ifndef INCLUDED_BSLALG_SWAPUTIL
#include <bslalg_swaputil.h>
#endif

#ifndef INCLUDED_BSLMA_USESBSLMAALLOCATOR
#include <bslma_usesbslmaallocator.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

#ifndef INCLUDED_BSL_STRING
#include <bsl_string.h>
#endif

#ifndef INCLUDED_BSL_VECTOR
#include <bsl_vector.h>
#endif

namespace Enterprise {
namespace pkg {

using namespace BloombergLP;

// Forward declaration for 'bslma::Allocator'
namespace BloombergLP { namespace bslma { class Allocator; } }

                        // ==============
                        // class Customer
                        // ==============

class Customer {
    // This simply constrained (value-semantic) attribute class represents the
    // information about a bank's customer.  A customer's first and last name
    // are represented as 'bsl::string' objects, the associated accounts are
    // stored in a 'bsl::vector<int>', and the employee identification number
    // is represented by an 'int'.  Note that the class invariants are
    // identically the constraints on the individual attributes.
    //
    // This class:
    //: o supports a complete set of *value-semantic* operations
    //:   o except for 'bslx' serialization
    //: o is *exception-neutral* (agnostic)
    //: o is *alias-safe*
    //: o is 'const' *thread-safe*

    // DATA
    bsl::string      d_firstName;       // first name
    bsl::string      d_lastName;        // last name
    bsl::vector<int> d_accounts;        // account numbers
    int              d_id;              // customer identification number

  public:
    // CREATORS
    Customer(bslma::Allocator *basicAllocator = 0);
        // Create a 'Customer' object having the (default) attribute values:
        //..
        //  firstName() == ""
        //  lastName()  == ""
        //  accounts()  == 0
        //  id()        == 0
        //..
        // Optionally specify a 'basicAllocator' used to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    Customer(const bslstl::StringRef&  firstName,
             const bslstl::StringRef&  lastName,
             const bsl::vector<int>&   accounts,
             int                       id,
             bslma::Allocator         *basicAllocator = 0);
        // Create a 'Customer' object having the specified 'firstName',
        // 'lastName', 'accounts', and 'id'' attribute values.  Optionally
        // specify a 'basicAllocator' used to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    Customer(const Customer& original, bslma::Allocator *basicAllocator = 0);
        // Create a 'Customer' object having the same value as the specified
        // 'original' object.  Optionally specify a 'basicAllocator' used to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    //! ~Customer() = default;
        // Destroy this object.

    // MANIPULATORS
    Customer& operator=(const Customer& rhs);
        // Assign to this object the value of the specified 'rhs' object, and
        // return a reference providing modifiable access to this object.

    void setFirstName(const bslstl::StringRef& value);
        // Set the 'firstName' attribute of this object to the specified
        // 'value'.

    void setLastName(const bslstl::StringRef& value);
        // Set the 'lastName' attribute of this object to the specified
        // 'value'.

    void setAccounts(const bsl::vector<int>& value);
        // Set the 'accounts' attribute of this object to the specified
        // 'value'.

    void setId(int value);
        // Set the 'id' attribute of this object to the specified 'value'.
        // The behavior is undefined unless 'value > 0'.

                                  // Aspects

    void swap(Customer& other);
        // Efficiently exchange the value of this object with the value of the
        // specified 'other' object.  This method provides the no-throw
        // exception-safety guarantee.  The behavior is undefined unless this
        // object was created with the same allocator as 'other'.

    // ACCESSORS
    const bsl::string& firstName() const;
        // Return a reference providing non-modifiable access to the
        // 'firstName' attribute of this object.

    const bsl::string& lastName() const;
        // Return a reference providing non-modifiable access to the
        // 'lastName' attribute of this object.

    const bsl::vector<int>& accounts() const;
        // Return a reference providing non-modifiable access to the
        // 'accounts' attribute of this object.

    int id() const;
        // Return the value of the 'id' attribute of this object.

                                  // Aspects

    bslma::Allocator *allocator() const;
        // Return the allocator used by this object to supply memory.  Note
        // that if no allocator was supplied at construction the currently
        // installed default allocator is used.

    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Write the value of this object to the specified output 'stream' in
        // a human-readable format, and return a reference to 'stream'.
        // Optionally specify an initial indentation 'level', whose absolute
        // value is incremented recursively for nested objects.  If 'level' is
        // specified, optionally specify 'spacesPerLevel', whose absolute
        // value indicates the number of spaces per indentation level for this
        // and all of its nested objects.  If 'level' is negative, suppress
        // indentation of the first line.  If 'spacesPerLevel' is negative,
        // format the entire output on one line, suppressing all but the
        // initial indentation (as governed by 'level').  If 'stream' is not
        // valid on entry, this operation has no effect.  Note that the
        // format is not fully specified, and can change without notice.
};

// FREE OPERATORS
bool operator==(const Customer& lhs, const Customer& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
    // value, and 'false' otherwise.  Two 'Customer' objects have the same
    // value if all of the corresponding values of their 'firstName',
    // 'lastName', 'accounts', and 'id' attributes are the same.

bool operator!=(const Customer& lhs, const Customer& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects do not have the
    // same value, and 'false' otherwise.  Two 'Customer' objects do not have
    // the same value if any of the corresponding values of their 'firstName',
    // 'lastName', 'accounts', or 'id' attributes are not the same.

bsl::ostream& operator<<(bsl::ostream& stream, const Customer& object);
    // Write the value of the specified 'object' to the specified
    // output 'stream' in a single-line format, and return a reference
    // providing modifiable access to 'stream'.  If 'stream' is not valid on
    // entry, this operation has no effect.  Note that this human-readable
    // format is not fully specified and can change without notice.  Also note
    // that this method has the same behavior as 'object.print(stream, 0, -1)',
    // but with the attribute names elided.

// FREE FUNCTIONS
void swap(Customer& a, Customer& b);
    // Efficiently exchange the values of the specified 'a' and 'b' objects.
    // This function provides the no-throw exception-safety guarantee.  The
    // behavior is undefined unless the two objects were created with the same
    // allocator.

}  // close package namespace
}  // close enterprise namespace

// TRAITS
namespace BloombergLP{
namespace bslma {

template <> struct UsesBslmaAllocator<Customer> : bsl::true_type {};

}
}

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                        // --------------
                        // class Customer
                        // --------------

namespace Enterprise {
namespace pkg {

// CREATORS
inline
Customer::Customer(bslma::Allocator *basicAllocator)
: d_firstName(basicAllocator)
, d_lastName(basicAllocator)
, d_accounts(basicAllocator)
, d_id(0)
{
}

inline
Customer::Customer(const bslstl::StringRef&  firstName,
                   const bslstl::StringRef&  lastName,
                   const bsl::vector<int>&   accounts,
                   int                       id,
                   bslma::Allocator         *basicAllocator)
: d_firstName(firstName.begin(), firstName.end(), basicAllocator)
, d_lastName(lastName.begin(), lastName.end(), basicAllocator)
, d_accounts(accounts, basicAllocator)
, d_id(id)
{
    BSLS_ASSERT_SAFE(!firstName.isEmpty());
    BSLS_ASSERT_SAFE(!lastName.isEmpty());
}

inline
Customer::Customer(const Customer& original, bslma::Allocator *basicAllocator)
: d_firstName(original.d_firstName, basicAllocator)
, d_lastName(original.d_lastName, basicAllocator)
, d_accounts(original.d_accounts, basicAllocator)
, d_id(original.d_id)
{
}

// MANIPULATORS
inline
Customer& Customer::operator=(const Customer& rhs)
{
    d_firstName = rhs.d_firstName;
    d_lastName  = rhs.d_lastName;
    d_accounts  = rhs.d_accounts;
    d_id        = rhs.d_id;
    return *this;
}

inline
void Customer::setFirstName(const bslstl::StringRef& value)
{
    BSLS_ASSERT_SAFE(!value.isEmpty());

    d_firstName.assign(value.begin(), value.end());
}

inline
void Customer::setLastName(const bslstl::StringRef& value)
{
    BSLS_ASSERT_SAFE(!value.isEmpty());

    d_lastName.assign(value.begin(), value.end());
}

inline
void Customer::setAccounts(const bsl::vector<int>& value)
{
    d_accounts = value;
}

inline
void Customer::setId(int value)
{
    BSLS_ASSERT_SAFE(value > 0);

    d_id = value;
}

                                  // Aspects

inline
void Customer::swap(Customer& other)
{
    BSLS_ASSERT_SAFE(allocator() == other.allocator());

    bslalg::SwapUtil::swap(&d_firstName, &other.d_firstName);
    bslalg::SwapUtil::swap(&d_lastName,  &other.d_lastName);
    bslalg::SwapUtil::swap(&d_accounts,  &other.d_accounts);
    bslalg::SwapUtil::swap(&d_id,        &other.d_id);
}

// ACCESSORS
inline
const bsl::string& Customer::firstName() const
{
    return d_firstName;
}

inline
const bsl::string& Customer::lastName() const
{
    return d_lastName;
}

inline
const bsl::vector<int>& Customer::accounts() const
{
    return d_accounts;
}

inline
int Customer::id() const
{
    return d_id;
}

                                  // Aspects

inline
bslma::Allocator *Customer::allocator() const
{
    return d_firstName.get_allocator().mechanism();
}

// FREE OPERATORS
inline
bool operator==(const Customer& lhs, const Customer& rhs)
{
    return lhs.firstName() == rhs.firstName()
        && lhs.lastName()  == rhs.lastName()
        && lhs.accounts()  == rhs.accounts()
        && lhs.id()        == rhs.id();
}

inline
bool operator!=(const Customer& lhs,
                const Customer& rhs)
{
    return lhs.firstName() != rhs.firstName()
        || lhs.lastName()  != rhs.lastName()
        || lhs.accounts()  != rhs.accounts()
        || lhs.id()        != rhs.id();
}

// FREE FUNCTIONS
inline
void swap(Customer& a, Customer& b)
{
    a.swap(b);
}

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg Finance L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
