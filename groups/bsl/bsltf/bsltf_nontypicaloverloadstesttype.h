// bsltf_nontypicaloverloadstesttype.h                                -*-C++-*-
#ifndef INCLUDED_BSLTF_NONTYPICALOVERLOADSTESTTYPE
#define INCLUDED_BSLTF_NONTYPICALOVERLOADSTESTTYPE

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a class that overloads the non-typical operators.
//
//@CLASSES:
//   bsltf::NonTypicalOverloadsTestType: test class that non-typical operators
//
//@DESCRIPTION: This component provides a single, unconstrained
// (value-semantic) attribute class, `NonTypicalOverloadsTestType`, that
// defines overloads to some non-typically-overloaded operators ('operator
// new', `operator delete`, and `operator&`) to ensure that they do not get
// invoked.
//
///Attributes
///----------
// ```
// Name                Type         Default
// ------------------  -----------  -------
// data                int          0
// ```
// * `data`: representation of the class value
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Demonstrating Calling Operator New/Delete Induce Assertions
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose that we wanted to show that calling `operator new` and
// `operator delete on `NonTypicalOverloadsTestType' causes an assertion.
//
// First, we verify that calling `operator new` will result in an
// assertion:
// ```
// bsls::AssertTestHandlerGuard guard;
// BSLS_ASSERTTEST_ASSERT_OPT_FAIL(new NonTypicalOverloadsTestType());
// ```
// Finally, we verify that calling `operator delete` will result in an
// assertion:
// ```
// NonTypicalOverloadsTestType obj;
// BSLS_ASSERTTEST_ASSERT_OPT_FAIL(delete obj);
// ```

#include <bslscm_version.h>

#include <bsls_assert.h>
#include <bsls_keyword.h>
#include <bsls_platform.h>

#include <cstddef>       // for 'std::size_t'

namespace BloombergLP {
namespace bsltf {

                        // =================================
                        // class NonTypicalOverloadsTestType
                        // =================================

/// This unconstrained (value-semantic) attribute class ensures that the
/// operator new member and operator delete member never gets called by
/// overloading and asserting in them.  See the Attributes section under
/// @DESCRIPTION in the component-level documentation for information on the
/// class attributes.
class NonTypicalOverloadsTestType {

  private:
    // DATA
    int d_data;

  private:
    // PRIVATE MANIPULATORS
#ifdef BSLS_PLATFORM_CMP_MSVC
    NonTypicalOverloadsTestType* operator&();
        // Overload 'operator&' and assert this method is not called.  This is
        // defined only on the Microsoft platform, because we in general do not
        // support types that overrides 'operator&' in our standard containers
        // implementation except on Windows.  See the component-level
        // documentation of 'bsls_util' for more information.  Note that this
        // method is deliberately *not* declared as 'const' in order to
        // increase the opportunities to provoke issues at the time of
        // compilation.
#endif

  public:
    // CLASS METHODS

    /// Overload `operator new` and assert this method is not called.
    static void* operator new(std::size_t size);

    /// Overload in place `new` and assert this method is not called.
    static void* operator new(std::size_t size, void *ptr);

#if !defined(BSLTF_NONTYPICALOVERLOADSTESTTYPE_TEST_DRIVER)
    static void operator delete(void *ptr);
#else
    static void operator delete(void *ptr)
                                    BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(false);
#endif
        // Overload 'operator delete' and assert this method is not called.

#if !defined(BSLTF_NONTYPICALOVERLOADSTESTTYPE_TEST_DRIVER)
    static void operator delete(void *ptr, void *);
#else
    static void operator delete(void *ptr, void *)
                                    BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(false);
#endif
        // Overload in place 'operator delete' and assert this method is not
        // called.  Not used but required to avoid compiler warnings (MSVC).

    // CREATORS

    /// Create a `NonTypicalOverloadsTestType` object having the (default)
    /// attribute values:
    /// ```
    /// data() == 0
    /// ```
    NonTypicalOverloadsTestType();

    /// Create a `NonTypicalOverloadsTestType` object having the specified
    /// `data` attribute value.
    explicit NonTypicalOverloadsTestType(int data);

    /// Create a `NonTypicalOverloadsTestType` object having the same value
    /// as the specified `original` object.
    NonTypicalOverloadsTestType(const NonTypicalOverloadsTestType& original);

#if !defined(BSLTF_NONTYPICALOVERLOADSTESTTYPE_TEST_DRIVER) ||                \
    !defined(BSLS_PLATFORM_CMP_MSVC)
    ~NonTypicalOverloadsTestType();
#else
    // MSVC will terminate due to an exception in a `noexcept false` `operator
    // delete` if the corresponding destructor is not also marked
    // `noexcept(false)`
    ~NonTypicalOverloadsTestType() BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(false);
#endif
        // Destroy this object.

    // MANIPULATORS

    /// Assign to this object the value of the specified `rhs` object, and
    /// return a reference providing modifiable access to this object.
    NonTypicalOverloadsTestType& operator=(
                                       const NonTypicalOverloadsTestType& rhs);

    /// Set the `data` attribute of this object to the specified `value`.
    void setData(int value);

    // ACCESSORS

    /// Return the value of the `data` attribute of this object.
    int data() const;
};

// FREE OPERATORS

/// Return `true` if the specified `lhs` and `rhs` objects have the same
/// value, and `false` otherwise.  Two `NonTypicalOverloadsTestType` objects
/// have the same if their `data` attributes are the same.
bool operator==(const NonTypicalOverloadsTestType& lhs,
                const NonTypicalOverloadsTestType& rhs);

/// Return `true` if the specified `lhs` and `rhs` objects do not have the
/// same value, and `false` otherwise.  Two `NonTypicalOverloadsTestType`
/// objects do not have the same value if their `data` attributes are not
/// the same.
bool operator!=(const NonTypicalOverloadsTestType& lhs,
                const NonTypicalOverloadsTestType& rhs);

// ============================================================================
//                  INLINE AND TEMPLATE FUNCTION IMPLEMENTATIONS
// ============================================================================

                        // ---------------------------------
                        // class NonTypicalOverloadsTestType
                        // ---------------------------------

// CLASS METHODS
inline
void* NonTypicalOverloadsTestType::operator new(std::size_t size)
{
    BSLS_ASSERT_OPT(0);

    return ::operator new(size);
}

inline
void* NonTypicalOverloadsTestType::operator new(std::size_t, void *ptr)
{
    BSLS_ASSERT_OPT(0);

    return ptr;
}

inline
#if !defined(BSLTF_NONTYPICALOVERLOADSTESTTYPE_TEST_DRIVER)
void NonTypicalOverloadsTestType::operator delete(void *)
#else
void NonTypicalOverloadsTestType::operator delete(void *)
                                     BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(false)
#endif
{
    BSLS_ASSERT_OPT(0);
}

inline
#if !defined(BSLTF_NONTYPICALOVERLOADSTESTTYPE_TEST_DRIVER)
void NonTypicalOverloadsTestType::operator delete(void *, void *)
#else
void NonTypicalOverloadsTestType::operator delete(void *, void *)
                                     BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(false)
#endif
{
    BSLS_ASSERT_OPT(0);
}

// CREATORS
inline
NonTypicalOverloadsTestType::NonTypicalOverloadsTestType()
: d_data(0)
{
}

inline
NonTypicalOverloadsTestType::NonTypicalOverloadsTestType(int data)
: d_data(data)
{
}

inline
NonTypicalOverloadsTestType::NonTypicalOverloadsTestType(
                                   const NonTypicalOverloadsTestType& original)
: d_data(original.d_data)
{
}

inline
#if !defined(BSLTF_NONTYPICALOVERLOADSTESTTYPE_TEST_DRIVER) ||                \
    !defined(BSLS_PLATFORM_CMP_MSVC)
NonTypicalOverloadsTestType::~NonTypicalOverloadsTestType()
#else
NonTypicalOverloadsTestType::~NonTypicalOverloadsTestType()
                                     BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(false)
#endif
{
    d_data = ~d_data & 0xf0f0f0f0;
}

// MANIPULATORS
inline
NonTypicalOverloadsTestType& NonTypicalOverloadsTestType::operator=(
                                        const NonTypicalOverloadsTestType& rhs)
{
    d_data = rhs.d_data;
    return *this;
}

inline
void NonTypicalOverloadsTestType::setData(int value)
{
    d_data = value;
}

#ifdef BSLS_PLATFORM_CMP_MSVC
inline
NonTypicalOverloadsTestType* NonTypicalOverloadsTestType::operator&()
{
    BSLS_ASSERT_OPT(0);

    return 0;
}
#endif

// ACCESSORS
inline
int NonTypicalOverloadsTestType::data() const
{
    return d_data;
}

// FREE OPERATORS
inline
bool operator==(const NonTypicalOverloadsTestType& lhs,
                const NonTypicalOverloadsTestType& rhs)
{
    return lhs.data() == rhs.data();
}

inline
bool operator!=(const NonTypicalOverloadsTestType& lhs,
                const NonTypicalOverloadsTestType& rhs)
{
    return lhs.data() != rhs.data();
}

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
