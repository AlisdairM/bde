// bsls_keyword.h                                                     -*-C++-*-
#ifndef INCLUDED_BSLS_KEYWORD
#define INCLUDED_BSLS_KEYWORD

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide macros for forward language dialect compatibility.
//
//@CLASSES:
//
//@MACROS:
//  BSLS_KEYWORD_CONSTEXPR: C++11 'constexpr' keyword
//  BSLS_KEYWORD_CONSTEXPR_MEMBER: for 'static constexpr' data members
//  BSLS_KEYWORD_CONSTEXPR_RELAXED: C++14 'constexpr' keyword (Deprecated)
//  BSLS_KEYWORD_CONSTEXPR_CPP14: C++14 'constexpr' keyword
//  BSLS_KEYWORD_CONSTEXPR_CPP17: C++17 'constexpr' keyword
//  BSLS_KEYWORD_DELETED: C++11 '= delete' function definition
//  BSLS_KEYWORD_EXPLICIT: C++11 'explicit' for conversion operators
//  BSLS_KEYWORD_FINAL: C++11 'final' keyword
//  BSLS_KEYWORD_INLINE_CONSTEXPR: Combination macro for 'inline constexpr'
//  BSLS_KEYWORD_INLINE_VARIABLE: C++17 'inline' keyword for variables
//  BSLS_KEYWORD_NOEXCEPT: C++11 'noexcept' keyword
//  BSLS_KEYWORD_NOEXCEPT_AVAILABLE: 'C++11' 'noexcept' flag
//  BSLS_KEYWORD_NOEXCEPT_OPERATOR(expr): C++11 'noexcept' operation
//  BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(...): C++11 noexcept function qualifier
//  BSLS_KEYWORD_OVERRIDE: C++11 'override' keyword
//
//@DESCRIPTION: This component provides a suite of macros that simplify the use
// of language keywords that may not exist in all supported dialects of the C++
// language.  For example, 'BSLS_KEYWORD_NOEXCEPT' is replaced with 'noexcept'
// on compilers supporting at least the C++11 language standard, and replaced
// with nothing on compilers supporting an older (e.g., C++03) standard.  The
// goal is to allow implementation of components such that they can take
// advantage of some C++11 or later features when compiled with C++11 or later
// mode enabled while also correctly compiling in C++03 mode.  The
// functionality of the respective features won't be available in C++03 mode.
//
///Macro Summary
///-------------
// The following are the macros provided by this component.
//
//: 'BSLS_KEYWORD_CONSTEXPR':
//:     This macro inserts the keyword 'constexpr' when compiling with C++11
//:     or later mode and inserts nothing when compiling with C++03 mode.
//:
//: 'BSLS_KEYWORD_CONSTEXPR_MEMBER':
//:     This macro inserts the keyword 'constexpr' when compiling with C++11
//:     or later mode and inserts the keyword 'const' when compiling with C++03
//:     mode.  This macro is intended to support declaring static data members.
//:
//: 'BSLS_KEYWORD_CONSTEXPR_RELAXED':
//:     !DEPRECATED! Use 'BSLS_KEYWORD_CONSTEXPR_CPP14' instead.  This macro
//:     inserts the keyword 'constexpr' when compiling with C++14 or later mode
//:     and inserts nothing when compiling with C++03/C++11 mode.
//:
//: 'BSLS_KEYWORD_CONSTEXPR_CPP14':
//:     This macro inserts the keyword 'constexpr' when compiling with C++14
//:     or later mode and inserts nothing when compiling with C++03/C++11 mode.
//:     See Example 2 below for a better description of the differences between
//:     'constexpr' between C++11, C++14, and C++17.
//:
//: 'BSLS_KEYWORD_CONSTEXPR_CPP17':
//:     This macro inserts the keyword 'constexpr' when compiling with C++17
//:     or later mode and inserts nothing when compiling with C++03/C++11/C++14
//:     mode.  See Example 2 below for a better description of the differences
//:     between 'constexpr' between C++11, C++14, and C++17.
//:
//: 'BSLS_KEYWORD_DELETED':
//:     This macro inserts the text '= delete' when compiling with C++11
//:     or later mode and inserts nothing when compiling with C++03 mode.
//:
//: 'BSLS_KEYWORD_EXPLICIT':
//:     This macro inserts the keyword 'explicit' when compiling with C++11
//:     or later mode and inserts nothing when compiling with C++03 mode.
//:
//: 'BSLS_KEYWORD_FINAL':
//:     This macro inserts the keyword 'final' when compiling with C++11 or
//:     later mode and inserts nothing when compiling with C++03 mode.
//:
//: 'BSLS_KEYWORD_INLINE_CONSTEXPR'
//:     This macro inserts the keywords 'inline constexpr' when compiling with
//:     C++17 or later mode and inserts the best approximation in earlier
//:     dialects, ultimately degrading down to 'static const' in C++03.
//:
//: 'BSLS_KEYWORD_INLINE_VARIABLE'
//:     This macro inserts the keyword 'inline' when compiling with C++17 or
//:     later mode and inserts nothing when compiling with C++03/C++11/C++14
//:     mode.
//:
//: 'BSLS_KEYWORD_NOEXCEPT':
//:     This macro inserts the keyword 'noexcept' when compiling with C++11
//:     or later mode and inserts nothing when compiling with C++03 mode.
//:
//: 'BSLS_KEYWORD_NOEXCEPT_AVAILABLE':
//:     This macro expands to 'true' when the 'noexcept' feature is available
//:     and 'false' otherwise.
//:
//: 'BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(BOOL_EXPRESSION)':
//:     This macro inserts the exception specification
//:     'noexcept(BOOL_EXPRESSION)' when compiling with C++11 or later mode and
//:     inserts nothing when compiling with C++03 mode.  This macro is used to
//:     specify which version of noexcept is intended when multiple 'noexcept's
//:     are used in a single statement.
//:
//: 'BSLS_KEYWORD_NOEXCEPT_OPERATOR(expr)':
//:     This macro inserts the operation 'noexcept(expr)' when compiling with
//:     C++11 or later mode and inserts the literal 'false' when compiling with
//:     C++03 mode.
//:
//: 'BSLS_KEYWORD_OVERRIDE'
//:     This macro inserts the keyword 'override' when compiling with C++11
//:     or later mode and inserts nothing when compiling with C++03 mode.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Preparing C++03 Code for C++11 Features
/// - - - - - - - - - - - - - - - - - - - - - - - - -
// To use these macros, simply insert them where the corresponding C++11
// keyword would go.  When compiling with C++03 mode there will be no effect
// but when compiling with C++11 mode additional restrictions will apply.  When
// compiling with C++11 mode the restriction will be checked providing some
// additional checking over what is done with C++11.
//
// C++ uses the 'explicit' keyword to indicate that constructors taking just
// one argument are not considered for implicit conversions.  Instead, they can
// only be used for explicit conversions.  C++ also provides the ability to
// define conversion operators but prior to C++11 these conversion operators
// are considered for implicit conversion.  C++11 allows the use of the
// 'explicit' keyword with conversion operators to avoid its use for implicit
// conversions.  The macro 'BSLS_KEYWORD_EXPLICIT' can be used to mark
// conversions as explicit conversions which will be checked when compiling
// with C++11 mode.  For example, an 'Optional' type may have an explicit
// conversion to 'bool' to indicate that the value is set (note the conversion
// operator):
//..
//  template <class TYPE>
//  class Optional
//  {
//      TYPE* d_value_p;
//  public:
//      Optional(): d_value_p() {}
//      explicit Optional(const TYPE& value): d_value_p(new TYPE(value)) {}
//      ~Optional() { delete d_value_p; }
//      // ...
//
//      BSLS_KEYWORD_EXPLICIT operator bool() const { return d_value_p; }
//  };
//..
// When using an object of the 'Optional' class in a condition it is desirable
// that it converts to a 'bool':
//..
//  void testFunction() {
//      Optional<int> value;
//      if (value) { /*... */ }
//..
// In places where an implicit conversion takes place it is not desirable that
// the conversion is used.  When compiling with C++11 mode the conversion
// operator will not be used, e.g., the following code will result in an error:
//..
//  #if BSLS_COMPILERFEATURES_CPLUSPLUS < 201103L
//      bool flag = value;
//  #endif
//  }
//..
// The code will compile successfully when using C++03 mode; without the macro,
// when using C++11 or greater mode we get an error like this:
//..
//     error: cannot convert 'Optional<int>' to 'bool' in initialization
//..
//
// When defining conversion operators to 'bool' for code which needs to compile
// with C++03 mode the conversion operator should convert to a member pointer
// type instead: doing so has a similar effect to making the conversion
// operator 'explicit'.
//
// Some classes are not intended for use as a base class.  To clearly label
// these classes and enforce that they can't be derived from C++11 allows using
// the 'final' keyword after the class name in the class definition to label
// classes which are not intended to be derived from.  The macro
// 'BSLS_KEYWORD_FINAL' is replaced by 'final' when compiling with C++11
// causing the compiler to enforce that a class can't be further derived.  The
// code below defines a class which can't be derived from:
//..
//  class FinalClass BSLS_KEYWORD_FINAL
//  {
//      int d_value;
//  public:
//      explicit FinalClass(int value = 0): d_value(value) {}
//      int value() const { return d_value; }
//  };
//..
// An attempt to derive from this class will fail when compiling with C++11
// mode:
//..
//  #if BSLS_COMPILERFEATURES_CPLUSPLUS < 201103L
//  class FinalClassDerived : public FinalClass {
//      int d_anotherValue;
//  public:
//      explicit FinalClassDerived(int value)
//      : d_anotherValue(2 * value) {
//      }
//      int anotherValue() const { return d_anotherValue; }
//  };
//  #endif
//..
// The code will compile successfully when using C++03 mode; without the macro,
// when using C++11 or greater mode we get an error like this:
//..
//    error: cannot derive from 'final' base 'FinalClass' in derived type
//    'FinalClassDerived'
//..
//
// Sometime it is useful to declare that an overriding function is the final
// overriding function and further derived classes won't be allowed to further
// override the function.  One use of this feature could be informing the
// compiler that it won't need to use virtual dispatch when calling this
// function on a pointer or a reference of the corresponding type.  C++11
// allows marking functions as the final overrider using the keyword 'final'.
// The macro 'BSLS_KEYWORD_FINAL' can also be used for this purpose.  To
// demonstrate the use of this keyword first a base class with a 'virtual'
// function is defined:
//..
//  struct FinalFunctionBase
//  {
//      virtual int f() { return 0; }
//  };
//..
// When defining a derived class this function 'f' can be marked as the final
// overrider using 'BSLS_KEYWORD_FINAL':
//..
//  struct FinalFunctionDerived: FinalFunctionBase
//  {
//      int f() BSLS_KEYWORD_FINAL { return 1; }
//  };
//..
// The semantics of the overriding function aren't changed but a further
// derived class can't override the function 'f', i.e., the following code will
// result in an error when compiling with C++11 mode:
//..
//  #if BSLS_COMPILERFEATURES_CPLUSPLUS < 201103L
//  struct FinalFunctionFailure: FinalFunctionDerived
//  {
//      int f() { return 2; }
//  };
//  #endif
//..
// The code will compile successfully when using C++03 mode; without the macro,
// when using C++11 or greater mode we get an error like this:
//..
//     error: virtual function 'virtual int FinalFunctionFailure::f()'
//     error: overriding final function 'virtual int FinalFunctionDerived::f()'
//..
//
// The C++11 keyword 'override' is used to identify functions overriding a
// 'virtual' function from a base class.  If a function identified as
// 'override' does not override a 'virtual' function from a base class the
// compilation results in an error.  The macro 'BSLS_KEYWORD_OVERRIDE' is used
// to insert the 'override' keyword when compiling with C++11 mode.  When
// compiling with C++03 mode it has no effect but it both cases it documents
// that a function is overriding a 'virtual' function from a base class.  To
// demonstrate the use of the 'BSLS_KEYWORD_OVERRIDE' macro first a base class
// is defined:
//..
//  struct OverrideBase
//  {
//      virtual int f() const { return 0; }
//  };
//..
// When overriding 'OverrideBase::f' in a derived class the
// 'BSLS_KEYWORD_OVERRIDE' macro should be used to ascertain that the function
// in the derived class is indeed overriding a 'virtual' function:
//..
//  struct OverrideSuccess: OverrideBase
//  {
//      int f() const BSLS_KEYWORD_OVERRIDE { return 1; }
//  };
//..
// The above code compiles successfully with both C++03 mode and C++11.  When
// the function meant to be an override actually isn't overriding any function
// the compilation will fail when using C++11 mode as is demonstrated by the
// following example (note the missing 'const' in the function declaration):
//..
//  #if BSLS_COMPILERFEATURES_CPLUSPLUS < 201103L
//  struct OverrideFailure: OverrideBase
//  {
//      int f() BSLS_KEYWORD_OVERRIDE { return 2; }
//  };
//  #endif
//..
// The code will compile successfully when using C++03 mode (though it might
// produce a warning); without the macro, when using C++11 or greater mode we
// get an error like this:
//..
//    error: 'int OverrideFailure::f()' marked 'override', but does not
//    override
//..
//
///Example 2: Creating an extended 'constexpr' function
/// - - - - - - - - - - - - - - - - - - - - - - - - - -
// To use these macros, simply insert them where the corresponding C++14
// keyword would go.  When compiling with C++03 or C++11 mode there will be no
// effect but when compiling with C++14 mode additional restrictions will
// apply.  When compiling with C++14 mode the restriction will be checked
// providing some additional checking over what is done with C++11 or C++03.
//
// C++11 uses the 'constexpr' keyword to indicate that a (very simple) function
// may be evaluated compile-time if all its input is known compile time.  C++14
// allows more complex functions to be 'constexpr'.  Also, in C++14,
// 'constexpr' member functions are not implicitly 'const' as in C++11.
// Thefore we have a separate macro 'BSLS_KEYWORD_CONSTEXPR_CPP14' that can be
// used to mark functions 'constexpr' when compiling with C++14 mode:
//..
//  BSLS_KEYWORD_CONSTEXPR_CPP14
//  int complexConstexprFunc(bool b)
//  {
//      if (b) {
//          return 42;                                                // RETURN
//      }
//      else {
//          return 17;                                                // RETURN
//      }
//  }
//..
// When compiling with C++14 'constexpr' support it is possible to use the
// result of 'complexConstexprFunc' in compile-time constants:
//..
//  void useComplexConstexprFunc()
//  {
//  #ifdef BSLS_COMPILERFEATURES_SUPPORT_CONSTEXPR_CPP14
//      constexpr
//  #endif
//      int result = complexConstexprFunc(true);
//      ASSERT(42 == result);
//..
// The macro 'BSLS_KEYWORD_CONSTEXPR_CPP14' can also be used on variables to
// achieve an identical result:
//..
//      BSLS_KEYWORD_CONSTEXPR_CPP14 int result2 = complexConstexprFunc(true);
//      ASSERT(42 == result2);
//  }
//..
// C++17 made small but significant changes to what is allowed in a 'constexpr'
// function.  Notably, a lambda can now be defined in such a function (and, if
// not called at compile time, does not itself need to be 'constexpr').  To
// take advantage of this there is a separate macro
// 'BSLS_KEYWORD_CONSTEXPR_CPP14' that can be used to mark functions
// 'constexpr' when compiling with C++17 mode:
//..
//  BSLS_KEYWORD_CONSTEXPR_CPP17
//  int moreComplexConstexprFunc(bool b)
//  {
//      if (b) {
//          return 42;                                                // RETURN
//      }
//      else {
//  #if BSLS_COMPILERFEATURES_CPLUSPLUS >= 201103L
//          return []{
//                     static int b = 17;
//                     return b;
//                 }();                                               // RETURN
//  #else
//          return 17;
//  #endif
//      }
//  }
//..
// Then, just like 'useComplexConstexprFunc', we can invoke
// 'moreComplexConstexprFunc' to populate a compile-time constant when it is
// supported:
//..
//  void useMoreComplexConstexprFunc()
//  {
//      BSLS_KEYWORD_CONSTEXPR_CPP17 int result
//                                            = moreComplexConstexprFunc(true);
//      ASSERT(42 == result);
//  }
//..

#include <bsls_compilerfeatures.h>

#ifdef BSLS_COMPILERFEATURES_SUPPORT_CONSTEXPR
# define BSLS_KEYWORD_CONSTEXPR        constexpr
# define BSLS_KEYWORD_CONSTEXPR_MEMBER constexpr
#else
# define BSLS_KEYWORD_CONSTEXPR
# define BSLS_KEYWORD_CONSTEXPR_MEMBER const
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_CONSTEXPR_CPP14
# define BSLS_KEYWORD_CONSTEXPR_CPP14 constexpr
#else
# define BSLS_KEYWORD_CONSTEXPR_CPP14
#endif

# define BSLS_KEYWORD_CONSTEXPR_RELAXED BSLS_KEYWORD_CONSTEXPR_CPP14

#ifdef BSLS_COMPILERFEATURES_SUPPORT_CONSTEXPR_CPP17
# define BSLS_KEYWORD_CONSTEXPR_CPP17 constexpr
#else
# define BSLS_KEYWORD_CONSTEXPR_CPP17
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_DELETED_FUNCTIONS
# define BSLS_KEYWORD_DELETED = delete
#else
# define BSLS_KEYWORD_DELETED
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_INLINE_CONSTEXPR
# define BSLS_KEYWORD_INLINE_CONSTEXPR inline constexpr
#elif defined(BSLS_COMPILERFEATURES_SUPPORT_CONSTEXPR)
# define BSLS_KEYWORD_INLINE_CONSTEXPR constexpr
#else
# define BSLS_KEYWORD_INLINE_CONSTEXPR static const
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_INLINE_VARIABLES
# define BSLS_KEYWORD_INLINE_VARIABLE inline
#else
# define BSLS_KEYWORD_INLINE_VARIABLE
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT
# define BSLS_KEYWORD_NOEXCEPT noexcept
# define BSLS_KEYWORD_NOEXCEPT_AVAILABLE true
# define BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(...) noexcept(__VA_ARGS__)
# define BSLS_KEYWORD_NOEXCEPT_OPERATOR(...)      noexcept(__VA_ARGS__)
#else
# define BSLS_KEYWORD_NOEXCEPT
# define BSLS_KEYWORD_NOEXCEPT_AVAILABLE false
# define BSLS_KEYWORD_NOEXCEPT_SPECIFICATION(...)
# define BSLS_KEYWORD_NOEXCEPT_OPERATOR(...) false
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_OPERATOR_EXPLICIT
# define BSLS_KEYWORD_EXPLICIT   explicit
#else
# define BSLS_KEYWORD_EXPLICIT
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_FINAL
# define BSLS_KEYWORD_FINAL      final
#else
# define BSLS_KEYWORD_FINAL
#endif

#ifdef BSLS_COMPILERFEATURES_SUPPORT_OVERRIDE
# define BSLS_KEYWORD_OVERRIDE   override
#else
# define BSLS_KEYWORD_OVERRIDE
#endif

// ----------------------------------------------------------------------------

#endif

// ----------------------------------------------------------------------------
// Copyright 2018 Bloomberg Finance L.P.
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
