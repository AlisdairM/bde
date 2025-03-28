// bslstl_function_invokerutil.t.cpp                                  -*-C++-*-
#include <bslstl_function_invokerutil.h>

#include <bsla_maybeunused.h>

#include <bslma_testallocator.h>

#include <bsls_bsltestutil.h>
#include <bsls_compilerfeatures.h>
#include <bsls_macrorepeat.h>
#include <bsls_objectbuffer.h>
#include <bsls_platform.h>

#include <stdio.h>   // `printf`
#include <stdlib.h>  // `atoi`, `size_t`

#if BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES
// clang-format off
// Include version that can be compiled with C++03
// Generated on Wed Jan 15 11:40:33 2025
// Command line: sim_cpp11_features.pl bslstl_function_invokerutil.t.cpp

# define COMPILING_BSLSTL_FUNCTION_INVOKERUTIL_T_CPP
# include <bslstl_function_invokerutil_cpp03.t.cpp>
# undef COMPILING_BSLSTL_FUNCTION_INVOKERUTIL_T_CPP

// clang-format on
#else

using namespace BloombergLP;

// ============================================================================
//                             TEST PLAN
// ----------------------------------------------------------------------------
// The component under test provides a utility class with only one public
// function, `invokerForFunc`.  The return value of `invokerForFunc` is itself
// a function pointer which is invoked with the address of a `Function_Rep`
// object and a set of arguments that are fowarded to the target object of the
// `Function_Rep`.  Testing `invokerForFunc` involves calling it with different
// kinds of targets and then calling the resulting function pointer to verify
// that the target is invoked.  Testing is performed with 0 to 13 arguments
// forwarded to the target.
// ----------------------------------------------------------------------------
// [ 2] invokerForFunc(const bsl::nullptr_t&);
// [ 2] invokerForFunc(const FUNC& f); // `FUNC` == ptr to function
// [ 3] invokerForFunc(const FUNC& f); // `FUNC` == ptr to member function
// [ 4] invokerForFunc(const FUNC& f); // `FUNC` == ptr to data member
// [ 5] invokerForFunc(const FUNC& f); // `FUNC` == user-defined functor
// [ 6] IsFuncInvocable<PROTOTYPE, FUNC>
//
// ----------------------------------------------------------------------------
// [ 1] BREATHING TEST
// ----------------------------------------------------------------------------

// ============================================================================
//                     STANDARD BSL ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", line, message);
        fflush(stdout);

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

// ============================================================================
//               STANDARD BSL TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLS_BSLTESTUTIL_ASSERT
#define ASSERTV      BSLS_BSLTESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLS_BSLTESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLS_BSLTESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLS_BSLTESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLS_BSLTESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLS_BSLTESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLS_BSLTESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLS_BSLTESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLS_BSLTESTUTIL_LOOP6_ASSERT

#define Q            BSLS_BSLTESTUTIL_Q   // Quote identifier literally.
#define P            BSLS_BSLTESTUTIL_P   // Print identifier and value.
#define P_           BSLS_BSLTESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLS_BSLTESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLS_BSLTESTUTIL_L_  // current Line number

// ============================================================================
//                                VERBOSITY
// ----------------------------------------------------------------------------

int verbose = 0;
int veryVerbose = 0;
int veryVeryVerbose = 0;
int veryVeryVeryVerbose = 0; // For test allocators

// ============================================================================
//                  GLOBAL DEFINITIONS FOR TESTING
// ----------------------------------------------------------------------------

#if BSLS_COMPILERFEATURES_CPLUSPLUS >= 202002L
#define CPP_20 1
#else
#define CPP_20 0
#endif

#if defined(BSLS_PLATFORM_CMP_MSVC)
#define MSVC 1
#else
#define MSVC 0
#endif

#if defined(BSLS_PLATFORM_CMP_MSVC) && BSLS_PLATFORM_CMP_VERSION == 1800
#define MSVC_2013 1
#else
#define MSVC_2013 0
#endif

#if defined(BSLS_PLATFORM_CMP_MSVC) && BSLS_PLATFORM_CMP_VERSION == 1900
#define MSVC_2015 1
#else
#define MSVC_2015 0
#endif

#if defined(BSLS_PLATFORM_CMP_MSVC)   \
 && BSLS_PLATFORM_CMP_VERSION >= 1910 \
 && BSLS_PLATFORM_CMP_VERSION <= 1916
#define MSVC_2017 1
#else
#define MSVC_2017 0
#endif

#if defined(BSLS_PLATFORM_CMP_MSVC)   \
 && BSLS_PLATFORM_CMP_VERSION >= 1920 \
 && BSLS_PLATFORM_CMP_VERSION <  1929
#define MSVC_2019 1
#else
#define MSVC_2019 0
#endif

#if defined(BSLS_PLATFORM_CMP_MSVC) &&                                        \
    (BSLS_COMPILERFEATURES_CPLUSPLUS <= 201703L ||                            \
     BSLS_PLATFORM_CMP_VERSION <= 1929)
    // MSVC displays inconsistent behavior with reference parameters. This was
    // fixed in MSVC 1930 (VS 2022) but only in C++20 mode.
#define MS_REF_BUG 1
#else
#define MS_REF_BUG 0
#endif

#if defined(BSLS_PLATFORM_CMP_CLANG)
#define CLANG 1
#else
#define CLANG 0
#endif

#if defined(BSLS_PLATFORM_CMP_GNU)      \
 && BSLS_PLATFORM_CMP_VERSION >= 100000
#define GNU_10PLUS 1
#else
#define GNU_10PLUS 0
#endif

typedef bslstl::Function_InvokerUtil             Util;
typedef bslstl::Function_Rep                     Rep;
typedef bslstl::Function_SmallObjectOptimization SmallObjectOptimization;

// Whitebox: Small object optimization buffer size
static const size_t k_SMALL_OBJECT_BUFFER_SIZE =
    sizeof(SmallObjectOptimization::InplaceBuffer);

#define NTWRAP(r)   bslalg::NothrowMovableUtil::wrap(r)
#define NTUNWRAP(r) bslalg::NothrowMovableUtil::unwrap(r)

/// A simple class with the interface of a smart pointer.
template <class TYPE>
class SmartPtr
{

    TYPE *d_obj_p;

  public:
    typedef TYPE value_type;

    SmartPtr(TYPE *p = 0) : d_obj_p(p) { }                          // IMPLICIT

    TYPE& operator*() const  { return *d_obj_p; }
    TYPE* operator->() const { return  d_obj_p; }
};

#define INT_ARGN(n) int arg ## n
#define ARGN(n) arg ## n

#define SUMMING_FUNC(n)                                                       \
    int sum ## n (BSLS_MACROREPEAT_COMMA(n, INT_ARGN)) {                      \
        return BSLS_MACROREPEAT_SEP(n, ARGN, +) + 0x4000;                     \
    }

// Create 14 functions with 0 to 13 integer arguments, returning the sum of the
// arguments + 0x4000.
SUMMING_FUNC(0)
BSLS_MACROREPEAT(13, SUMMING_FUNC)

#define RVALUE_INT_ARGN(n) bslmf::MovableRef<int> arg ## n
#define RVALUE_MOVE_INT_ARGN(n) \
    rvalueMoveInt(bslmf::MovableRefUtil::move(arg ## n))

int rvalueMoveInt(bslmf::MovableRef<int> i)
{
    int& li = bslmf::MovableRefUtil::access(i);
    int output = li;
    li = -1;
    return output;
}

#define RVALUE_SUMMING_FUNC(n)                                                \
    int rsum ## n (BSLS_MACROREPEAT_COMMA(n, RVALUE_INT_ARGN)) {              \
        return BSLS_MACROREPEAT_SEP(n, RVALUE_MOVE_INT_ARGN, +) + 0x8000;     \
    }

// Create 14 functions with 0 to 13 integer rvalue arguments, returning the sum
// of the arguments + 0x8000 and setting the arguments to -1.
RVALUE_SUMMING_FUNC(0)
BSLS_MACROREPEAT(13, RVALUE_SUMMING_FUNC)

// Increment `*val` by one.  Used to test a void return type.
void increment(int *val)
{
    ++*val;
}

/// a complete class type
class Base {
};

/// This is a complete class type that derives from `Base`, and which has a
/// single, private data member to ensure that it is not an aggregate.  A
/// constructor that initializes the data member and an accessor for it are
/// provided to silence warnings that it may be unused.
///
/// Note that it is important that this class is not an aggregate because it
/// is primarily used to test whether derived-to-base conversions are
/// allowed by machinery in this component, e.g., in checking whether the
/// return type of an invokable type is convertible to a desired return type
/// in the `bslstl::Function_InvokerUtil::IsFuncInvocable` type trait.
class Derived : public Base {

    // DATA
    int d_data;

  public:
    // CREATORS
    BSLA_MAYBE_UNUSED Derived()
    : Base()
    , d_data()
    {
    }

    // ACCESSORS
    BSLA_MAYBE_UNUSED int data() const
    {
        return d_data;
    }
};

/// a complete class type that is explicitly convertible from `Base`
class Explicit {

  public:
    // CREATORS

    /// declared but not defined
    explicit Explicit(Base);
};

/// a complete class type that is an aggregate
class BaseAggregate {
};

/// a complete class type that is an aggregate and derives from
/// `BaseAggregate`
class DerivedAggregate : public BaseAggregate {
};

#if !BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES // $var-args=13

/// forward declaration
template <class PROTOTYPE>
class F;

/// a complete class type that is invocable with the prototype
/// `RET (ARGS...)`
template <class RET, class... ARGS>
class F<RET(ARGS...)> {

  public:
    // ACCESSORS

    /// declared but not defined
    RET operator()(ARGS...) const;
};

#endif

/// Class of objects implicitly convertible to `int`
class ConvertibleToInt
{

    int d_value;

  public:
    explicit ConvertibleToInt(int v) : d_value(v) { }

    operator int() const { return d_value; }
};

/// Simple wrapper around an arbitrary object of type `T`, for testing
/// access through a pointer to data member.
template <typename TYPE>
struct DataWrapper
{

    TYPE d_t;  // public data member

    /// Initialize `d_t` with the specified `t`.
    DataWrapper(const TYPE& t) : d_t(t) {}
};

template <typename TYPE>
struct DerivedDataWrapper : DataWrapper<TYPE>
{
    DerivedDataWrapper(const TYPE& t) : DataWrapper<TYPE>(t) {}
};

/// Simple wrapper around an `int` that supplies member functions (whose
/// address can be taken) for testing `bsl::function`.  This wrapper also
/// provides a call operator that returns the value
class IntWrapper
{

    int d_value;

  public:
    IntWrapper(int i = 0) : d_value(i) { }                          // IMPLICIT
    IntWrapper(ConvertibleToInt i) : d_value(i) { }                 // IMPLICIT

#define ADD_FUNC(n)                                                           \
    int add ## n (BSLS_MACROREPEAT_COMMA(n, INT_ARGN)) const {                \
        return value() + BSLS_MACROREPEAT_SEP(n, ARGN, +);                    \
    }

#define INCREMENT_FUNC(n)                                                     \
    int increment ## n (BSLS_MACROREPEAT_COMMA(n, INT_ARGN)) {                \
        return d_value += BSLS_MACROREPEAT_SEP(n, ARGN, +);                   \
    }

#define VOID_INCREMENT_FUNC(n)                                                \
    void voidIncrement ## n (BSLS_MACROREPEAT_COMMA(n, INT_ARGN)) {           \
        d_value += BSLS_MACROREPEAT_SEP(n, ARGN, +);                          \
    }

    int add0() const { return d_value; }
    BSLS_MACROREPEAT(12, ADD_FUNC)
        // Const function with 0 to 12 arguments.  Return value() plus the sum
        // of all arguments.

    int increment0() { return d_value; }
    BSLS_MACROREPEAT(12, INCREMENT_FUNC)
        // Mutable function with 0 to 12 arguments.  Increment `d_value` by the
        // sum of all arguments and return the new value.  `increment0()` is a
        // no-op.

    void incrementBy1() { ++d_value; }

    /// Return `value()` + the integer at the specified `lv` modifiable
    /// reference and set the value at `lv` to -1.
    int lvadd1(int& lv) const { int ret = d_value + lv; lv = -1; return ret; }

    /// Return `value()` + the integer at the specified `rv` movable
    /// reference and set the value at `rv` to -1.
    int rvadd1(bslmf::MovableRef<int> rv) const { return lvadd1(rv); }

    int sub1(int arg) { return value() - arg; }

    int value() const { return d_value; }

    /// Return the current value added to the optionally specified
    /// `increment` (default 0).
    int operator()(int increment = 0) const { return d_value + increment; }

    void voidIncrement0() { }
    BSLS_MACROREPEAT(12, VOID_INCREMENT_FUNC)
        // Mutating function with 0 to 12 arguments.  Increment `d_value` by
        // the sum of all arguments.  `voidIncrement0()` is a no-op.
};

inline bool operator==(const IntWrapper& a, const IntWrapper& b)
{
    return a.value() == b.value();
}

BSLA_MAYBE_UNUSED
inline bool operator!=(const IntWrapper& a, const IntWrapper& b)
{
    return a.value() != b.value();
}

/// Derived class of `IntWrapper`
class IntWrapperDerived : public IntWrapper
{

  public:
    IntWrapperDerived(int v) : IntWrapper(v) { }                    // IMPLICIT
};

int *getAddress(int& r) { return &r; }
const int *getConstAddress(const int& r) { return &r; }

/// Dummy class.  Does nothing.
struct Dummy {
};

/// Dummy class.  Does nothing.
struct Dummy2 {
};

/// Functor object that fits in the small object buffer.
class SmallFunctor {

    // DATA
    int d_value;

  public:
    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(SmallFunctor,
                                   bsl::is_nothrow_move_constructible);

    // CREATORS
    explicit SmallFunctor(int value = 0) : d_value(value) { }

    //! SmallFunctor(const SmallFunctor&) = default;
    //! SmallFunctor(SmallFunctor&&) noexcept = default;
    //! ~SmallFunctor() = default;

    // MANIPULATORS
    //! SmallFunctor& operator=(const SmallFunctor&) = default;
    //! SmallFunctor& operator=(SmallFunctor&&) noexcept = default;

    // Generation macro for parenthesis operators
#define OP_PAREN(n)                                                           \
    int operator()(const IntWrapper& iw, BSLS_MACROREPEAT_COMMA(n, INT_ARGN)) \
    {                                                                         \
        int ret = (value() + iw.value() +                                     \
                   BSLS_MACROREPEAT_SEP(n, ARGN, +));                         \
        setValue(ret);                                                        \
        return ret;                                                           \
    }

    int operator()() { return value(); }
    int operator()(const IntWrapper& iw)
        { int ret = value() + iw.value(); setValue(ret); return ret; }
    BSLS_MACROREPEAT(12, OP_PAREN)
        // Increment this object's value by the sum of the specified `iw` (if
        // any) and `arg0` through `arg12`, then return the new value.  All of
        // the arguments to this call operator are optional, so this functor
        // can be called with 0 to 13 (integer) arguments.

#undef OP_PAREN

    /// Parse the specified `valstr` as an integer and use it to set this
    /// object's value.  This call operator is used to test invocations that
    /// return `void`.  The string argument prevents overload ambiguity.
    void operator()(const char* valstr)
        { setValue(atoi(valstr)); }

    void setValue(int value) { d_value = value; }

    // ACCESSORS

    /// Return the address of the specified `arg`.  This call operator is
    /// used to test invocations that take reference arguments.
    Dummy *operator()(Dummy& arg) const
        { return &arg; }

    /// Return the address of the specified `arg`.  This call operator is
    /// used to test invocations that take movable reference arguments.
    Dummy2 *operator()(bslmf::MovableRef<Dummy2> arg) const
        { return &bslmf::MovableRefUtil::access(arg); }

    int value() const { return d_value; }
};

/// Functor object that fits in the small object buffer but does not qualify
/// for the small object optimization because it has a throwing move
/// constructor.
class SmallThrowingFunctor : public SmallFunctor {

  public:
    // CREATORS
    explicit SmallThrowingFunctor(int value = 0) : SmallFunctor(value) { }

    SmallThrowingFunctor(const SmallThrowingFunctor& original)
        : SmallFunctor(original.value()) { }
    SmallThrowingFunctor(bslmf::MovableRef<SmallThrowingFunctor> original)
        : SmallFunctor(bslmf::MovableRefUtil::access(original).value()) { }

    //! ~SmallThrowingFunctor() = default;

    // MANIPULATORS
    SmallThrowingFunctor& operator=(const SmallThrowingFunctor& rhs)
    {
        setValue(rhs.value());
        return *this;
    }

    SmallThrowingFunctor&
    operator=(bslmf::MovableRef<SmallThrowingFunctor> rhs)
    {
        setValue(bslmf::MovableRefUtil::access(rhs).value());
        return *this;
    }
};

/// Functor object that does not fit in the small object buffer.
class LargeFunctor : public SmallFunctor {

    // Add enough padding so that it doesn't fit in the small object buffer.
    char d_padding[k_SMALL_OBJECT_BUFFER_SIZE - sizeof(int) + 1];

  public:
    // CREATORS
    explicit LargeFunctor(int value = 0) : SmallFunctor(value)
        { (void) d_padding; }

    //! LargeFunctor(const LargeFunctor&) = default;
    //! LargeFunctor(LargeFunctor&&) = default;
    //! ~LargeFunctor() = default;

    // MANIPULATORS
    //! LargeFunctor operator=(const LargeFunctor&) = default;
    //! LargeFunctor operator=(LargeFunctor&&) = default;
};

BSLMF_ASSERT(sizeof(SmallFunctor)         < k_SMALL_OBJECT_BUFFER_SIZE);
BSLMF_ASSERT(sizeof(SmallThrowingFunctor) < k_SMALL_OBJECT_BUFFER_SIZE);
BSLMF_ASSERT(sizeof(LargeFunctor)         > k_SMALL_OBJECT_BUFFER_SIZE);
BSLMF_ASSERT(SmallObjectOptimization::IsInplaceFunc<SmallFunctor>::value);
BSLMF_ASSERT(! SmallObjectOptimization::
               IsInplaceFunc<SmallThrowingFunctor>::value);
BSLMF_ASSERT(! SmallObjectOptimization::IsInplaceFunc<LargeFunctor>::value);

/// Wrap and make available an object of the specified `TYPE` for passing
/// into functions and constructors in the test driver.  `TYPE` is
/// constrained to be constructible from `int` and have a `value()` method
/// that returns an `int`.  (`IntWrapper` and the `Functor`s above meet
/// these criteria.)
template <class TYPE>
class ArgGeneratorBase {

    bsls::ObjectBuffer<TYPE> d_value;

  public:
    enum { INIT_VALUE = 0x2001, MOVED_FROM_VALUE = -1 };

    /// Create an object of wrapping a `TYPE` object with a known initial
    /// value.
    ArgGeneratorBase() { new (d_value.buffer()) TYPE(INIT_VALUE); }

    /// Reset the wrapped object to its initial value and return a
    /// modifiable reference to the wrapped object.
    TYPE& reset()
    {
        d_value.object().~TYPE();
        new (d_value.buffer()) TYPE(INIT_VALUE);
        return d_value.object();
    }

    /// Return the value of the wrapped object.
    int value() const { return d_value.object().value(); }

    /// Return a modifiable reference to the wrapped object.
    TYPE& targetObj() { return d_value.object(); }
};

/// Wrap and make available an object of the specified `TYPE` for passing
/// into functions and constructors in the test driver.  `TYPE` is
/// constrained to be constructible from `int` and have a `value()` method
/// that returns an `int`.  (`IntWrapper` and the `Functor`s above meet
/// these criteria.)  The expected usage is to construct an object of this
/// type, `x`, and pass `x.obj()` to a function or constructor.  After the
/// call, verify that `x.check(v)` returns true, where `v` is the value that
/// the call is expected to have written into `x`.
///
/// This primary template is used when `TYPE` is not an lvalue reference,
/// rvalue reference, pointer, or `SmartPtr` type.
template <class TYPE>
struct ArgGenerator : ArgGeneratorBase<TYPE> {

    /// Return true if the wrapped object has its initial value.  Normally,
    /// this function would compare the wrapped object to the passed
    /// argument, but since, for this specialization, `obj()` returns an
    /// rvalue, `somecall(x.obj())` cannot modify `x`, so `x` would be
    /// expected to retain its initial value.
    bool check(int /* exp */) const
        { return this->value() == ArgGeneratorBase<TYPE>::INIT_VALUE; }

    /// Reset the wrapped object to its initial value and return a copy of
    /// the wrapped object, typically for use as an argument in a function
    /// call.
    TYPE obj() { return this->reset(); }
};

/// Specialization of `ArgGenerator` for lvalues reference to the specified
/// `TYPE`.
template <class TYPE>
struct ArgGenerator<TYPE&> : ArgGeneratorBase<TYPE> {

    /// Return true if the wrapped object's value compares equal to the
    /// specified `exp`.
    bool check(int exp) const { return this->value() == exp; }

    /// Reset the wrapped object to its initial value and return a
    /// modifiable lvalue reference to the wrapped object, typically for use
    /// as an argument in a function call.
    TYPE& obj() { return this->reset(); }
};

/// Specialization of `ArgGenerator` for lvalues reference to the specified
/// `TYPE`.
template <class TYPE>
struct ArgGenerator<const TYPE&> : ArgGeneratorBase<TYPE> {

    /// Return true if the wrapped object's value compares equal to the
    /// specified `exp`.
    bool check(int exp) const { return this->value() == exp; }

    /// Reset the wrapped object to its initial value and return a `const`
    /// lvalue reference to the wrapped object, typically for use as an
    /// argument in a function call.
    const TYPE& obj() { return this->reset(); }
};

/// Specialization of `ArgGenerator` for pointers to the specified `TYPE`.
template <class TYPE>
struct ArgGenerator<TYPE *> : ArgGeneratorBase<TYPE> {

    /// Return true if the wrapped object's value compares equal to the
    /// specified `exp`.
    bool check(int exp) const { return this->value() == exp; }

    /// Reset the wrapped object to its initial value and return a
    /// modifiable pointer to the wrapped object, typically for use as an
    /// argument in a function call.
    TYPE *obj() { return &this->reset(); }
};

/// Specialization of `ArgGenerator` for pointers to the specified `TYPE`.
template <class TYPE>
struct ArgGenerator<const TYPE *> : ArgGeneratorBase<TYPE> {

    /// Return true if the wrapped object's value compares equal to the
    /// specified `exp`.
    bool check(int exp) const { return this->value() == exp; }

    /// Reset the wrapped object to its initial value and return a `const`
    /// pointer to the wrapped object, typically for use as an argument in a
    /// function call.
    const TYPE *obj() { return &this->reset(); }
};

/// Specialization of `ArgGenerator` for `SmartPtr` to the specified `TYPE`.
template <class TYPE>
struct ArgGenerator<SmartPtr<TYPE> > : ArgGeneratorBase<TYPE> {

    // Specialization for smart pointers to `TYPE`

    /// Return true if the wrapped object's value compares equal to the
    /// specified `exp`.
    bool check(int exp) const { return this->value() == exp; }

    /// Reset the wrapped object to its initial value and return a smart
    /// pointer to the wrapped object, typically for use as an argument in a
    /// function call.  The smart pointer is returned by value, but provides
    /// modifiable access to the wrapped object.
    SmartPtr<TYPE> obj() { return SmartPtr<TYPE>(&this->reset()); }
};

/// Specialization of `ArgGenerator` for `SmartPtr` to the specified `TYPE`.
template <class TYPE>
struct ArgGenerator<const SmartPtr<TYPE> > : ArgGeneratorBase<TYPE> {

    // Specialization for smart pointers to `TYPE`

    /// Return true if the wrapped object's value compares equal to the
    /// specified `exp`.
    bool check(int exp) const { return this->value() == exp; }

    /// Reset the wrapped object to its initial value and return a smart
    /// pointer to the wrapped object, typically for use as an argument in a
    /// function call.  The smart pointer is returned by value, but provides
    /// modifiable access to the wrapped object.
    const SmartPtr<TYPE> obj() { return SmartPtr<TYPE>(&this->reset()); }
};

// `bslmf::MovableRef` when C++11 is an alias for a member of a helper template
// explicitly for the purpose of preventing type deduction.  This, however,
// also prevents partial specialization on rvalue references, which we want to
// do here.  To get around that we use a different type alias on C++11, and a
// macro in C++03.   This should only be needed in the partial specialization
// declaration.
#if defined(BSLMF_MOVABLEREF_USES_RVALUE_REFERENCES)
template <class TYPE>
using DeducibleMovableRef = TYPE&&;
#else
#define DeducibleMovableRef bslmf::MovableRef
#endif

/// Wrap and make available an object of the specified `TYPE` for passing
/// into functions as an rvalue reference (or a `MovableRef<TYPE>` when
/// rvalue references are being simluated).
template <class TYPE>
struct ArgGenerator<DeducibleMovableRef<TYPE> > : ArgGeneratorBase<TYPE> {

    /// Return true if the wrapped object has been set to `MOVED_FROM_VALUE`
    bool check(int /*exp*/) const
        { return this->value() == ArgGeneratorBase<TYPE>::MOVED_FROM_VALUE; }

    /// Reset the wrapped object to its initial value and return a
    /// modifiable rvalue reference to the wrapped object, typically for
    /// use as an argument in a function call.
    bslmf::MovableRef<TYPE> obj()
        { return bslmf::MovableRefUtil::move(this->reset()); }

};

// ============================================================================
//                              TEST FUNCTIONS
// ----------------------------------------------------------------------------

#if !BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES // $var-args=13

/// Utilities to facilitate testing `invokerForFunc`.
template <class RET, class... ARGS>
struct TestUtil {

    // TYPES
    typedef Util::GenericInvoker
                GenericInvoker;
    typedef RET PROTOTYPE(ARGS...);
    typedef RET Invoker(const Rep                                     *,
                        typename bslmf::ForwardingType<ARGS>::Type... );
};

/// Empty the specified `rep` and install the specified `target` into `rep`,
/// then return the result of calling `Util::invokerForFunc`.  The `RET` and
/// `ARGS` paramters must be explicitly specified; the `FUNC` parameter is
/// deduced.
template <class RET, class... ARGS, class FUNC>
typename TestUtil<RET, ARGS...>::Invoker *
getInvoker(Rep *rep, FUNC target)
{
    typedef TestUtil<RET, ARGS...> TU;
    typedef typename TU::PROTOTYPE Prototype;
    typedef typename TU::Invoker   Invoker;

    rep->makeEmpty();

    Util::GenericInvoker *ginv_p = Util::invokerForFunc<Prototype>(target);
    rep->installFunc(bslmf::MovableRefUtil::move(target), ginv_p);

    return reinterpret_cast<Invoker *>(ginv_p);
}

#endif

/// Given `M` as `RemoveReference<RET>::type`, install the specified `pdm`
/// as the target and test invokers with variations of prototype `M(ARG)`.
template <class RET, class ARG, class PTR_TO_DATA_MEM>
void testPtrToDataMember(PTR_TO_DATA_MEM pdm)
{
    typedef typename
        bslmf::MovableRefUtil::RemoveReference<RET>::type M;

    bslma::TestAllocator                                  ta;
    Rep                                                   rep(&ta);
    ArgGenerator<ARG>                                     gen;

    // Prototype `M(ARG)`
    ASSERT((0x2001 == getInvoker<M, ARG>(&rep, pdm)(&rep, gen.obj())));

    // Prototype `int(ARG)`
    ASSERT((0x2001 == getInvoker<int, ARG>(&rep, pdm)(&rep, gen.obj())));

    // Prototype `long(ARG)`
    ASSERT((0x2001 == getInvoker<long, ARG>(&rep, pdm)(&rep, gen.obj())));
}

/// Given `M` as `RemoveReference<RET>::type`, install the specified `pdm`
/// as the target and test invokers with variations of prototype `M&(ARG)`.
template <class RET, class ARG, class PTR_TO_DATA_MEM>
void testPtrToDataMemberByRef(PTR_TO_DATA_MEM pdm)
{
    typedef typename
        bslmf::MovableRefUtil::RemoveReference<RET>::type M;

    bslma::TestAllocator                                  ta;
    Rep                                                   rep(&ta);
    ArgGenerator<ARG>                                     gen;

    testPtrToDataMember<M, ARG>(pdm);

    // Prototype `M&(ARG)`.  `M` may be const qualified.
    {
        M& r = getInvoker<M&, ARG>(&rep, pdm)(&rep, gen.obj());
        ASSERTV(&r, &(gen.targetObj().d_t), &r == &(gen.targetObj().d_t));
    }

    // Prototype `const M&(ARG)`.
    {
        const M& r = getInvoker<const M&, ARG>(&rep, pdm)(&rep, gen.obj());
        ASSERTV(&r, &(gen.targetObj().d_t), &r == &(gen.targetObj().d_t));
    }
}

/// Test invocation of pointer to member function wrapper.  The `TYPE`,
/// `RET`, and `ARG` type are selected by the caller so that the non-const
/// member functions `IntWrapper::increment[0-12]` can be invoked with the
/// correct number of arguments of type `ARG` on an object of type `TYPE`
/// and return a value of type `RET`.
template <class TYPE, class RET, class ARG>
void testPtrToMemFunc()
{
    bslma::TestAllocator ta;
    Rep                  rep(&ta);

    const ARG a1(0x0002);
    const ARG a2(0x0004);
    const ARG a3(0x0008);
    const ARG a4(0x0010);
    const ARG a5(0x0020);
    const ARG a6(0x0040);
    const ARG a7(0x0080);
    const ARG a8(0x0100);
    const ARG a9(0x0200);
    const ARG a10(0x0400);
    const ARG a11(0x0800);
    const ARG a12(0x1000);

    ArgGenerator<TYPE> gen;

    ASSERT(0x2001 == (getInvoker<RET, TYPE>
                      (&rep, &IntWrapper::increment0)
                      (&rep, gen.obj())));
    ASSERT(gen.check(0x2001));

    ASSERT(0x2003 == (getInvoker<RET, TYPE, ARG>
                      (&rep, &IntWrapper::increment1)
                      (&rep, gen.obj(), a1)));
    ASSERT(gen.check(0x2003));

    ASSERT(0x2007 == (getInvoker<RET, TYPE, ARG, ARG>
                      (&rep, &IntWrapper::increment2)
                      (&rep, gen.obj(), a1, a2)));
    ASSERT(gen.check(0x2007));

    ASSERT(0x200f == (getInvoker<RET, TYPE, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment3)
                      (&rep, gen.obj(), a1, a2, a3)));
    ASSERT(gen.check(0x200f));

    ASSERT(0x201f == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment4)
                      (&rep, gen.obj(), a1, a2, a3, a4)));
    ASSERT(gen.check(0x201f));

    ASSERT(0x203f == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment5)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5)));
    ASSERT(gen.check(0x203f));

    ASSERT(0x207f == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment6)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6)));
    ASSERT(gen.check(0x207f));

    ASSERT(0x20ff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment7)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7)));
    ASSERT(gen.check(0x20ff));

    ASSERT(0x21ff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG>
                      (&rep, &IntWrapper::increment8)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8)));
    ASSERT(gen.check(0x21ff));

    ASSERT(0x23ff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment9)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8, a9)));
    ASSERT(gen.check(0x23ff));

    ASSERT(0x27ff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment10)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8, a9,
                       a10)));
    ASSERT(gen.check(0x27ff));

    ASSERT(0x2fff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment11)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8, a9,
                       a10, a11)));
    ASSERT(gen.check(0x2fff));

    ASSERT(0x3fff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::increment12)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8, a9,
                       a10, a11, a12)));
    ASSERT(gen.check(0x3fff));

    // Test `const` member functions as well
    ASSERT(0x2001 == (getInvoker<RET, TYPE>
                      (&rep, &IntWrapper::add0)
                      (&rep, gen.obj())));
    ASSERT(gen.check(0x2001));

    ASSERT(0x2003 == (getInvoker<RET, TYPE, ARG>
                      (&rep, &IntWrapper::add1)
                      (&rep, gen.obj(), a1)));
    ASSERT(gen.check(0x2001));

    // No need to test 3 through 12 arguments.  That mechanism has already been
    // tested via the `testPtrToMemFunc` function.

    ASSERT(0x23ff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG, ARG>
                      (&rep, &IntWrapper::add9)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8, a9)));
    ASSERT(gen.check(0x2001));

    // Test void `RET`
    getInvoker<void, TYPE, ARG>
        (&rep, &IntWrapper::increment1)
        (&rep, gen.obj(), a1);     // No return type to test
    ASSERT(gen.check(0x2003));     // Check expected side-effect

    getInvoker<void, TYPE, ARG>
        (&rep, &IntWrapper::voidIncrement1)
        (&rep, gen.obj(), a1);     // No return type to test
    ASSERT(gen.check(0x2003));     // Check expected side-effect
}

/// Test invocation of pointer to const member function wrapper.  Tests use
/// const member functions `IntWrapper::add[0-12]`.  To save compile time,
/// since `testPtrToMemFunc` already tests every possible argument-list
/// length, we test only a small number of possible argument-list lengths
/// (specifically 0, 1, and 12 arguments) here.
template <class TYPE, class RET, class ARG>
void testPtrToConstMemFunc()
{
    bslma::TestAllocator ta;
    Rep                  rep(&ta);

    const ARG a1(0x0002);
    const ARG a2(0x0004);
    const ARG a3(0x0008);
    const ARG a4(0x0010);
    const ARG a5(0x0020);
    const ARG a6(0x0040);
    const ARG a7(0x0080);
    const ARG a8(0x0100);
    const ARG a9(0x0200);
    const ARG a10(0x0400);
    const ARG a11(0x0800);
    const ARG a12(0x1000);

    ArgGenerator<TYPE> gen;

    ASSERT(0x2001 == (getInvoker<RET, TYPE>
                      (&rep, &IntWrapper::add0)
                      (&rep, gen.obj())));
    ASSERT(gen.check(0x2001));

    ASSERT(0x2003 == (getInvoker<RET, TYPE, ARG>
                      (&rep, &IntWrapper::add1)
                      (&rep, gen.obj(), a1)));
    ASSERT(gen.check(0x2001));

    // No need to test 3 through 12 arguments.  That mechanism has already been
    // tested via the `testPtrToMemFunc` function.

    ASSERT(0x3fff == (getInvoker<RET, TYPE, ARG, ARG, ARG, ARG, ARG, ARG,
                                 ARG, ARG, ARG, ARG, ARG, ARG>
                      (&rep, &IntWrapper::add12)
                      (&rep, gen.obj(), a1, a2, a3, a4, a5, a6, a7, a8, a9,
                       a10, a11, a12)));
    ASSERT(gen.check(0x2001));
}

/// Test invocation of a stateful functor (function object)
template <class FUNC, class RET, class ARG>
void testFunctor()
{
    bslma::TestAllocator ta;
    Rep                  rep(&ta); const Rep& REP = rep;

    ArgGenerator<FUNC> gen;

    typedef IntWrapper IntW;

    const ARG a1(0x0001);
    const ARG a2(0x0002);
    const ARG a3(0x0004);
    const ARG a4(0x0008);
    const ARG a5(0x0010);
    const ARG a6(0x0020);
    const ARG a7(0x0040);
    const ARG a8(0x0080);
    const ARG a9(0x0100);
    const ARG a10(0x0200);
    const ARG a11(0x0400);
    const ARG a12(0x0800);
    const ARG a13(0x1000);

    ASSERT((0x2001 == getInvoker<RET>(&rep, gen.obj())(&REP)));
    ASSERT(0x2001 == rep.target<FUNC>()->value());
    ASSERT((0x2002 == getInvoker<RET, IntW>(&rep, gen.obj())(&REP, a1)));
    ASSERT(0x2002 == rep.target<FUNC>()->value());
    ASSERT((0x2004 == getInvoker<RET, IntW, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2)));
    ASSERT(0x2004 == rep.target<FUNC>()->value());
    ASSERT((0x2008 == getInvoker<RET, IntW, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3)));
    ASSERT(0x2008 == rep.target<FUNC>()->value());
    ASSERT((0x2010 == getInvoker<RET, IntW, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4)));
    ASSERT(0x2010 == rep.target<FUNC>()->value());
    ASSERT((0x2020 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5)));
    ASSERT(0x2020 == rep.target<FUNC>()->value());
    ASSERT((0x2040 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6)));
    ASSERT(0x2040 == rep.target<FUNC>()->value());
    ASSERT((0x2080 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7)));
    ASSERT(0x2080 == rep.target<FUNC>()->value());
    ASSERT((0x2100 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8)));
    ASSERT(0x2100 == rep.target<FUNC>()->value());
    ASSERT((0x2200 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG,
            ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8, a9)));
    ASSERT(0x2200 == rep.target<FUNC>()->value());
    ASSERT((0x2400 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG,
            ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)));
    ASSERT(0x2400 == rep.target<FUNC>()->value());
    ASSERT((0x2800 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG,
            ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)));
    ASSERT(0x2800 == rep.target<FUNC>()->value());
    ASSERT((0x3000 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG,
            ARG, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)));
    ASSERT(0x3000 == rep.target<FUNC>()->value());
    ASSERT((0x4000 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG,
            ARG, ARG, ARG, ARG, ARG>
            (&rep, gen.obj())
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)));
    ASSERT(0x4000 == rep.target<FUNC>()->value());

    // Test void return type with side effect.
    getInvoker<void, const char *>(&rep, gen.obj())(&REP, "7");
    ASSERT(7 == rep.target<FUNC>()->value());

    // Test discarding of non-void return value
    getInvoker<void, int>(&rep, gen.obj())(&REP, 9);
    ASSERT(0x200a == rep.target<FUNC>()->value());

    // Test pass-by-reference argument
    Dummy v;
    ASSERT((&v == getInvoker<Dummy *, Dummy&>(&rep, gen.obj())(&REP, v)));
    Dummy2 v2;
    ASSERT(&v2 == (getInvoker<Dummy2 *, bslmf::MovableRef<Dummy2> >
                   (&rep, gen.obj())
                   (&rep, bslmf::MovableRefUtil::move(v2))));

    // Test with `NothrowMovableWrapper`.
    ASSERT((0x2001 == getInvoker<RET>(&rep, NTWRAP(gen.obj()))(&REP)));
    ASSERT(0x2001 == rep.target<FUNC>()->value());
    ASSERT((0x2002 == getInvoker<RET, IntW>
            (&rep, NTWRAP(gen.obj()))
            (&REP, a1)));
    ASSERT(0x2002 == rep.target<FUNC>()->value());
    ASSERT((0x4000 == getInvoker<RET, IntW, ARG, ARG, ARG, ARG, ARG, ARG, ARG,
            ARG, ARG, ARG, ARG, ARG>
            (&rep, NTWRAP(gen.obj()))
            (&REP, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)));
    ASSERT(0x4000 == rep.target<FUNC>()->value());
}

#ifdef BSLSTL_FUNCTION_INVOKERUTIL_SUPPORT_IS_FUNC_INVOCABLE

class TestIsFuncInvocable {

  public:
    // CREATORS
    TestIsFuncInvocable()
    {
    }

    // ACCESSORS
    template <class PROTOTYPE, class FUNC>
    void run(int LINE, bool EXPECTED_VALUE) const
    {
        if (EXPECTED_VALUE) {
            ASSERTV(LINE, (Util::IsFuncInvocable<PROTOTYPE, FUNC>::value));
        }
        else {
            ASSERTV(LINE, (! Util::IsFuncInvocable<PROTOTYPE, FUNC>::value));
        }
    }
};

#endif // defined(BSLSTL_FUNCTION_INVOKERUTIL_SUPPORT_IS_FUNC_INVOCABLE)

}  // Close unnamed namespace

// ============================================================================
//                              MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int test = argc > 1 ? atoi(argv[1]) : 0;
    verbose = argc > 2;
    veryVerbose = argc > 3;
    veryVeryVerbose = argc > 4;
    veryVeryVeryVerbose = argc > 5;

    printf("TEST " __FILE__ " CASE %d\n", test);

    // `Function_Rep` requires an allocator at construction.  `Function_Rep` is
    // not the class under test, so there are no allocation tests.  Therefore,
    // all tests simply use the default allocator for constructing
    // `Function_Rep` objects.
    bslma::TestAllocator ta;

    switch (test) { case 0:  // Zero is always the leading case.
      case 6: {
        // --------------------------------------------------------------------
        // TESTING `IsFuncInvocable<PROTOTYPE, FUNC>`
        //   This case tests that `IsFuncInvocable` correctly detects whether
        //   objects of `FUNC` type are invocable with the
        //   `bslmf::ForwardingType`-forwarded argument types of the
        //   `PROTOTYPE`.  This case also tests that `IsFuncInvocable`
        //   correctly detects whether objects of `FUNC` type return an object
        //   of a type that is explicitly convertible to the return type of the
        //   `PROTOTYPE`.  If both of these criteria are met, `IsFuncInvocable`
        //   should declare a `true` `value`, and should declare a `false`
        //   `value` otherwise.
        //
        //   Note that `IsFuncInvocable` is qualitatively different than
        //   `std::is_invocable_r`, in that it makes concessions for supporting
        //   legacy behavior of `bsl::function`.  'std::is_invocable_r<RET,
        //   FUNC, ARGS...>' requires that the return type of the invocation of
        //   `FUNC` with `ARGS...` be implicitly convertible to `RET`, as
        //   opposed to explicitly convertible.  Further, the use of
        //   `bslmf::ForwardingType` to forward arguments in the invoker of a
        //   `bsl::function` creates qualitatively different behavior than
        //   the argument forwarding mechanism used by the standard `INVOKE`
        //   pseudo-expression.
        //
        //   In the concerns and plan, `PROTOTYPE` is a function prototype
        //   and `RET(ARGS...)` is the same function prototype, where `RET` is
        //   the return type and `ARGS...` are the argument types.
        //
        // Concerns:
        // 1. If `FUNC` is invocable with the arguments `ARGS...` forwarded
        //    through `bslmf::ForwardingType`, and the return type of invoking
        //    `FUNC` as such is explicitly convertible to `RET`,
        //    `IsFuncInvocable` declares a static, Boolean data member `value`
        //    that is `true`, and declares such a `value` that is `false`
        //    otherwise.
        //
        // Plan:
        // 1. Using a large combination of cvr-qualified fundamental and
        //    class types in argument and return positions of 'PROTOTYPE" and
        //    the call operator of `FUNC`,  verify that the results of
        //    `IsFuncInvocable<PROTOTYPE, FUNC>` meet the specification of
        //    concern 1.
        //
        // 2. Verify that explicit conversions are permitted between the
        //    return type of `FUNC` and `RET`.
        //
        // 3. Verify that, for backward compatibility,
        //    `IsFuncInvocable<PROTOTYPE, FUNC>` permits bindings rvalues
        //    of fundamental types in `ARGS...` to corresponding lvalues in the
        //    parameter list of `FUNC`.  For more information, see
        //    {DRQS 164834023}.
        //
        // Testing
        //  IsFuncInvocable<PROTOTYPE, FUNC>
        // --------------------------------------------------------------------

        if (verbose)
            printf("\nTESTING `IsFuncInvocable<PROTOTYPE, FUNC>`"
                   "\n==========================================\n");

#ifdef BSLSTL_FUNCTION_INVOKERUTIL_SUPPORT_IS_FUNC_INVOCABLE

          const TestIsFuncInvocable TEST;

          static const bool YES = true;
          static const bool NO  = false;

          typedef int                          I;
          typedef const int                    cI;
          typedef volatile int                 vI;
          typedef const volatile int           cvI;
          typedef int&                         rI;
          typedef const int&                   rcI;
          typedef volatile int&                rvI;
          typedef const volatile int&          rcvI;
          typedef bslmf::MovableRef<int>       rrI;
          typedef bslmf::MovableRef<const int> rrcI;

          typedef int                      *PI;
          typedef int *&                    rPI;
          typedef bslmf::MovableRef<int *>  rrPI;

          typedef const int                      *PcI;
          typedef const int *&                    rPcI;
          typedef bslmf::MovableRef<const int *>  rrPcI;

          typedef int       AI[10];
          typedef const int AcI[10];
          typedef int(&rAI)[10];
          typedef const int(&rAcI)[10];
          typedef int(&&rrAI)[10];
          typedef const int(&&rrAcI)[10];

          typedef Base                        B;
          typedef const B                     cB;
          typedef volatile B                  vB;
          typedef const volatile B            cvB;
          typedef B&                          rB;
          typedef const B&                    rcB;
          typedef volatile B&                 rvB;
          typedef const volatile B&           rcvB;
          typedef bslmf::MovableRef<B>        rrB;
          typedef bslmf::MovableRef<const B>  rrcB;

          typedef Derived                     D;
          typedef D&                          rD;
          typedef const D&                    rcD;
          typedef bslmf::MovableRef<D>        rrD;
          typedef bslmf::MovableRef<const D>  rrcD;

          typedef Explicit                    E;
          typedef E&                          rE;
          typedef const E&                    rcE;
          typedef bslmf::MovableRef<E>        rrE;
          typedef bslmf::MovableRef<const E>  rrcE;

          typedef BaseAggregate               BA;
          typedef BA&                         rBA;
          typedef const BA&                   rcBA;
          typedef bslmf::MovableRef<BA>       rrBA;
          typedef bslmf::MovableRef<const BA> rrcBA;

          typedef DerivedAggregate            DA;
          typedef DA&                         rDA;
          typedef const DA&                   rcDA;
          typedef bslmf::MovableRef<DA>       rrDA;
          typedef bslmf::MovableRef<const DA> rrcDA;


#if defined(BSLS_PLATFORM_CMP_GNU) && BSLS_PLATFORM_CMP_VERSION >= 100000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#endif

#ifdef BSLS_PLATFORM_CMP_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#endif

          //                                       LINE        IS INVOCABLE
          //                                       ----.      .------------
          //        PROTOTYPE       INVOCAND TYPE       \.   /
          //       ------------ ----------------------  -- ----
          // invocations with conversions of cvr-qualified `int` argument types
          TEST.run<void (    I),  void       (    I)  >(L_, YES);
          TEST.run<void (    I),  void       (   cI)  >(L_, YES);
          TEST.run<void (    I),  void       (   vI)  >(L_, YES);
          TEST.run<void (    I),  void       (  cvI)  >(L_, YES);
          TEST.run<void (    I),  void       (   rI)  >(L_, YES);
          TEST.run<void (    I),  void       (  rcI)  >(L_, YES);
          TEST.run<void (    I),  void       (  rvI)  >(L_, YES);
          TEST.run<void (    I),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (    I),  void       (  rrI)  >(L_, NO );
          TEST.run<void (    I),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (   cI),  void       (    I)  >(L_, YES);
          TEST.run<void (   cI),  void       (   cI)  >(L_, YES);
          TEST.run<void (   cI),  void       (   vI)  >(L_, YES);
          TEST.run<void (   cI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (   cI),  void       (   rI)  >(L_, YES);
          TEST.run<void (   cI),  void       (  rcI)  >(L_, YES);
          TEST.run<void (   cI),  void       (  rvI)  >(L_, YES);
          TEST.run<void (   cI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (   cI),  void       (  rrI)  >(L_, NO );
          TEST.run<void (   cI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (   vI),  void       (    I)  >(L_, YES);
          TEST.run<void (   vI),  void       (   cI)  >(L_, YES);
          TEST.run<void (   vI),  void       (   vI)  >(L_, YES);
          TEST.run<void (   vI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (   vI),  void       (   rI)  >(L_, YES);
          TEST.run<void (   vI),  void       (  rcI)  >(L_, YES);
          TEST.run<void (   vI),  void       (  rvI)  >(L_, YES);
          TEST.run<void (   vI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (   vI),  void       (  rrI)  >(L_, NO );
          TEST.run<void (   vI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (  cvI),  void       (    I)  >(L_, YES);
          TEST.run<void (  cvI),  void       (   cI)  >(L_, YES);
          TEST.run<void (  cvI),  void       (   vI)  >(L_, YES);
          TEST.run<void (  cvI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (  cvI),  void       (   rI)  >(L_, YES);
          TEST.run<void (  cvI),  void       (  rcI)  >(L_, YES);
          TEST.run<void (  cvI),  void       (  rvI)  >(L_, YES);
          TEST.run<void (  cvI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (  cvI),  void       (  rrI)  >(L_, NO );
          TEST.run<void (  cvI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (   rI),  void       (    I)  >(L_, YES);
          TEST.run<void (   rI),  void       (   cI)  >(L_, YES);
          TEST.run<void (   rI),  void       (   vI)  >(L_, YES);
          TEST.run<void (   rI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (   rI),  void       (   rI)  >(L_, YES);
          TEST.run<void (   rI),  void       (  rcI)  >(L_, YES);
          TEST.run<void (   rI),  void       (  rvI)  >(L_, YES);
          TEST.run<void (   rI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (   rI),  void       (  rrI)  >(L_, NO );
          TEST.run<void (   rI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (  rcI),  void       (    I)  >(L_, YES);
          TEST.run<void (  rcI),  void       (   cI)  >(L_, YES);
          TEST.run<void (  rcI),  void       (   vI)  >(L_, YES);
          TEST.run<void (  rcI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (  rcI),  void       (   rI)  >(L_, NO );
          TEST.run<void (  rcI),  void       (  rcI)  >(L_, YES);
          TEST.run<void (  rcI),  void       (  rvI)  >(L_, NO );
          TEST.run<void (  rcI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (  rcI),  void       (  rrI)  >(L_, NO );
          TEST.run<void (  rcI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (  rvI),  void       (    I)  >(L_, YES);
          TEST.run<void (  rvI),  void       (   cI)  >(L_, YES);
          TEST.run<void (  rvI),  void       (   vI)  >(L_, YES);
          TEST.run<void (  rvI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (  rvI),  void       (   rI)  >(L_, NO );
          TEST.run<void (  rvI),  void       (  rcI)  >(L_, NO );
          TEST.run<void (  rvI),  void       (  rvI)  >(L_, YES);
          TEST.run<void (  rvI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void (  rvI),  void       (  rrI)  >(L_, NO );
          TEST.run<void (  rvI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void ( rcvI),  void       (    I)  >(L_, YES);
          TEST.run<void ( rcvI),  void       (   cI)  >(L_, YES);
          TEST.run<void ( rcvI),  void       (   vI)  >(L_, YES);
          TEST.run<void ( rcvI),  void       (  cvI)  >(L_, YES);
          TEST.run<void ( rcvI),  void       (   rI)  >(L_, NO );
          TEST.run<void ( rcvI),  void       (  rcI)  >(L_, NO );
          TEST.run<void ( rcvI),  void       (  rvI)  >(L_, NO );
          TEST.run<void ( rcvI),  void       ( rcvI)  >(L_, YES);
          TEST.run<void ( rcvI),  void       (  rrI)  >(L_, NO );
          TEST.run<void ( rcvI),  void       ( rrcI)  >(L_, NO );
          TEST.run<void (  rrI),  void       (    I)  >(L_, YES);
          TEST.run<void (  rrI),  void       (   cI)  >(L_, YES);
          TEST.run<void (  rrI),  void       (   vI)  >(L_, YES);
          TEST.run<void (  rrI),  void       (  cvI)  >(L_, YES);
          TEST.run<void (  rrI),  void       (   rI)  >(L_, NO );
          TEST.run<void (  rrI),  void       (  rcI)  >(L_, YES);
          TEST.run<void (  rrI),  void       (  rvI)  >(L_, NO );
          TEST.run<void (  rrI),  void       ( rcvI)  >(L_, MS_REF_BUG);
          TEST.run<void (  rrI),  void       (  rrI)  >(L_, YES);
          TEST.run<void (  rrI),  void       ( rrcI)  >(L_, YES);
          TEST.run<void ( rrcI),  void       (    I)  >(L_, YES);
          TEST.run<void ( rrcI),  void       (   cI)  >(L_, YES);
          TEST.run<void ( rrcI),  void       (   vI)  >(L_, YES);
          TEST.run<void ( rrcI),  void       (  cvI)  >(L_, YES);
          TEST.run<void ( rrcI),  void       (   rI)  >(L_, NO );
          TEST.run<void ( rrcI),  void       (  rcI)  >(L_, YES);
          TEST.run<void ( rrcI),  void       (  rvI)  >(L_, NO );
          TEST.run<void ( rrcI),  void       ( rcvI)  >(L_, MS_REF_BUG);
          TEST.run<void ( rrcI),  void       (  rrI)  >(L_, NO );
          TEST.run<void ( rrcI),  void       ( rrcI)  >(L_, YES);

          // invocations with conversions of cvr-qualified complete class
          // argument types
          TEST.run<void (    B),  void       (    B)  >(L_, YES);
          TEST.run<void (    B),  void       (   cB)  >(L_, YES);
          TEST.run<void (    B),  void       (   vB)  >(L_, YES);
          TEST.run<void (    B),  void       (  cvB)  >(L_, YES);
          TEST.run<void (    B),  void       (   rB)  >(L_, MS_REF_BUG);
          TEST.run<void (    B),  void       (  rcB)  >(L_, YES);
          TEST.run<void (    B),  void       (  rvB)  >(L_, NO );
          TEST.run<void (    B),  void       ( rcvB)  >(L_, NO );
          TEST.run<void (    B),  void       (  rrB)  >(L_, YES);
          TEST.run<void (    B),  void       ( rrcB)  >(L_, YES);
          TEST.run<void (   cB),  void       (    B)  >(L_, YES);
          TEST.run<void (   cB),  void       (   cB)  >(L_, YES);
          TEST.run<void (   cB),  void       (   vB)  >(L_, YES);
          TEST.run<void (   cB),  void       (  cvB)  >(L_, YES);
          TEST.run<void (   cB),  void       (   rB)  >(L_, MS_REF_BUG);
          TEST.run<void (   cB),  void       (  rcB)  >(L_, YES);
          TEST.run<void (   cB),  void       (  rvB)  >(L_, NO );
          TEST.run<void (   cB),  void       ( rcvB)  >(L_, NO );
          TEST.run<void (   cB),  void       (  rrB)  >(L_, YES);
          TEST.run<void (   cB),  void       ( rrcB)  >(L_, YES);
          TEST.run<void (   vB),  void       (    B)  >(L_, YES);
          TEST.run<void (   vB),  void       (   cB)  >(L_, YES);
          TEST.run<void (   vB),  void       (   vB)  >(L_, YES);
          TEST.run<void (   vB),  void       (  cvB)  >(L_, YES);
          TEST.run<void (   vB),  void       (   rB)  >(L_, MS_REF_BUG);
          TEST.run<void (   vB),  void       (  rcB)  >(L_, YES);
          TEST.run<void (   vB),  void       (  rvB)  >(L_, NO );
          TEST.run<void (   vB),  void       ( rcvB)  >(L_, NO );
          TEST.run<void (   vB),  void       (  rrB)  >(L_, YES);
          TEST.run<void (   vB),  void       ( rrcB)  >(L_, YES);
          TEST.run<void (  cvB),  void       (    B)  >(L_, YES);
          TEST.run<void (  cvB),  void       (   cB)  >(L_, YES);
          TEST.run<void (  cvB),  void       (   vB)  >(L_, YES);
          TEST.run<void (  cvB),  void       (  cvB)  >(L_, YES);
          TEST.run<void (  cvB),  void       (   rB)  >(L_, MS_REF_BUG);
          TEST.run<void (  cvB),  void       (  rcB)  >(L_, YES);
          TEST.run<void (  cvB),  void       (  rvB)  >(L_, NO );
          TEST.run<void (  cvB),  void       ( rcvB)  >(L_, NO );
          TEST.run<void (  cvB),  void       (  rrB)  >(L_, YES);
          TEST.run<void (  cvB),  void       ( rrcB)  >(L_, YES);
          TEST.run<void (   rB),  void       (    B)  >(L_, YES);
          TEST.run<void (   rB),  void       (   cB)  >(L_, YES);
          TEST.run<void (   rB),  void       (   vB)  >(L_, YES);
          TEST.run<void (   rB),  void       (  cvB)  >(L_, YES);
          TEST.run<void (   rB),  void       (   rB)  >(L_, YES);
          TEST.run<void (   rB),  void       (  rcB)  >(L_, YES);
          TEST.run<void (   rB),  void       (  rvB)  >(L_, YES);
          TEST.run<void (   rB),  void       ( rcvB)  >(L_, YES);
          TEST.run<void (   rB),  void       (  rrB)  >(L_, NO );
          TEST.run<void (   rB),  void       ( rrcB)  >(L_, NO );
          TEST.run<void (  rcB),  void       (    B)  >(L_, YES);
          TEST.run<void (  rcB),  void       (   cB)  >(L_, YES);
          TEST.run<void (  rcB),  void       (   vB)  >(L_, YES);
          TEST.run<void (  rcB),  void       (  cvB)  >(L_, YES);
          TEST.run<void (  rcB),  void       (   rB)  >(L_, NO );
          TEST.run<void (  rcB),  void       (  rcB)  >(L_, YES);
          TEST.run<void (  rcB),  void       (  rvB)  >(L_, NO );
          TEST.run<void (  rcB),  void       ( rcvB)  >(L_, YES);
          TEST.run<void (  rcB),  void       (  rrB)  >(L_, NO );
          TEST.run<void (  rcB),  void       ( rrcB)  >(L_, NO );
          TEST.run<void (  rvB),  void       (    B)  >(L_, NO );
          TEST.run<void (  rvB),  void       (   cB)  >(L_, NO );
          TEST.run<void (  rvB),  void       (   vB)  >(L_, NO );
          TEST.run<void (  rvB),  void       (  cvB)  >(L_, NO );
          TEST.run<void (  rvB),  void       (   rB)  >(L_, NO );
          TEST.run<void (  rvB),  void       (  rcB)  >(L_, NO );
          TEST.run<void (  rvB),  void       (  rvB)  >(L_, YES);
          TEST.run<void (  rvB),  void       ( rcvB)  >(L_, YES);
          TEST.run<void (  rvB),  void       (  rrB)  >(L_, NO );
          TEST.run<void (  rvB),  void       ( rrcB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (    B)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (   cB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (   vB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (  cvB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (   rB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (  rcB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       (  rvB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       ( rcvB)  >(L_, YES);
          TEST.run<void ( rcvB),  void       (  rrB)  >(L_, NO );
          TEST.run<void ( rcvB),  void       ( rrcB)  >(L_, NO );
          TEST.run<void (  rrB),  void       (    B)  >(L_, YES);
          TEST.run<void (  rrB),  void       (   cB)  >(L_, YES);
          TEST.run<void (  rrB),  void       (   vB)  >(L_, YES);
          TEST.run<void (  rrB),  void       (  cvB)  >(L_, YES);
          TEST.run<void (  rrB),  void       (   rB)  >(L_, MS_REF_BUG);
          TEST.run<void (  rrB),  void       (  rcB)  >(L_, YES);
          TEST.run<void (  rrB),  void       (  rvB)  >(L_, NO );
          TEST.run<void (  rrB),  void       ( rcvB)  >(L_, NO );
          TEST.run<void (  rrB),  void       (  rrB)  >(L_, YES);
          TEST.run<void (  rrB),  void       ( rrcB)  >(L_, YES);
          TEST.run<void ( rrcB),  void       (    B)  >(L_, YES);
          TEST.run<void ( rrcB),  void       (   cB)  >(L_, YES);
          TEST.run<void ( rrcB),  void       (   vB)  >(L_, YES);
          TEST.run<void ( rrcB),  void       (  cvB)  >(L_, YES);
          TEST.run<void ( rrcB),  void       (   rB)  >(L_, NO );
          TEST.run<void ( rrcB),  void       (  rcB)  >(L_, YES);
          TEST.run<void ( rrcB),  void       (  rvB)  >(L_, NO );
          TEST.run<void ( rrcB),  void       ( rcvB)  >(L_, NO );
          TEST.run<void ( rrcB),  void       (  rrB)  >(L_, NO );
          TEST.run<void ( rrcB),  void       ( rrcB)  >(L_, YES);

          // invocations with conversions of cvr-qualified array argument types
          TEST.run<void (   AI),  void       (   AI)  >(L_, YES);
          TEST.run<void (   AI),  void       (  AcI)  >(L_, YES);
          TEST.run<void (   AI),  void       (  rAI)  >(L_, NO );
          TEST.run<void (   AI),  void       ( rAcI)  >(L_, NO );
          TEST.run<void (   AI),  void       ( rrAI)  >(L_, NO );
          TEST.run<void (   AI),  void       (rrAcI)  >(L_, NO );
          TEST.run<void (  AcI),  void       (   AI)  >(L_, NO );
          TEST.run<void (  AcI),  void       (  AcI)  >(L_, YES);
          TEST.run<void (  AcI),  void       (  rAI)  >(L_, NO );
          TEST.run<void (  AcI),  void       ( rAcI)  >(L_, NO );
          TEST.run<void (  AcI),  void       ( rrAI)  >(L_, NO );
          TEST.run<void (  AcI),  void       (rrAcI)  >(L_, NO );
          TEST.run<void (  rAI),  void       (   AI)  >(L_, YES);
          TEST.run<void (  rAI),  void       (  AcI)  >(L_, YES);
          TEST.run<void (  rAI),  void       (  rAI)  >(L_, YES);
          TEST.run<void (  rAI),  void       ( rAcI)  >(L_, YES);
          TEST.run<void (  rAI),  void       ( rrAI)  >(L_, MSVC_2017);
          TEST.run<void (  rAI),  void       (rrAcI)  >(L_, MSVC_2017);
          TEST.run<void ( rAcI),  void       (   AI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       (  AcI)  >(L_, YES);
          TEST.run<void ( rAcI),  void       (  rAI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       ( rAcI)  >(L_, YES);
          TEST.run<void ( rAcI),  void       ( rrAI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       (rrAcI)  >(L_, MSVC_2017);
          TEST.run<void ( rrAI),  void       (   AI)  >(L_, YES);
          TEST.run<void ( rrAI),  void       (  AcI)  >(L_, YES);
          TEST.run<void ( rrAI),  void       (  rAI)  >(L_, MSVC_2017
                                                         || MSVC_2015);
          TEST.run<void ( rrAI),  void       ( rAcI)  >(L_, YES);
          TEST.run<void ( rrAI),  void       ( rrAI)  >(L_, !MSVC_2015);
          TEST.run<void ( rrAI),  void       (rrAcI)  >(L_, !MSVC_2015);
          TEST.run<void (rrAcI),  void       (   AI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       (  AcI)  >(L_, YES);
          TEST.run<void (rrAcI),  void       (  rAI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       ( rAcI)  >(L_, YES);
          TEST.run<void (rrAcI),  void       ( rrAI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       (rrAcI)  >(L_, !MSVC_2015);

          // invocations with decay of cvr-qualified array argument types to
          // pointer argument types
          TEST.run<void (   AI),  void       (   PI)  >(L_, YES);
          TEST.run<void (   AI),  void       (  PcI)  >(L_, YES);
          TEST.run<void (   AI),  void       (  rPI)  >(L_, YES);
          TEST.run<void (   AI),  void       ( rPcI)  >(L_, NO );
          TEST.run<void (   AI),  void       ( rrPI)  >(L_, NO );
          TEST.run<void (   AI),  void       (rrPcI)  >(L_, !(MSVC  ||
                                                              CLANG ||
                                                              GNU_10PLUS));
          TEST.run<void (  AcI),  void       (   PI)  >(L_, NO );
          TEST.run<void (  AcI),  void       (  PcI)  >(L_, YES);
          TEST.run<void (  AcI),  void       (  rPI)  >(L_, NO );
          TEST.run<void (  AcI),  void       ( rPcI)  >(L_, YES);
          TEST.run<void (  AcI),  void       ( rrPI)  >(L_, NO );
          TEST.run<void (  AcI),  void       (rrPcI)  >(L_, NO );
          TEST.run<void (  rAI),  void       (   PI)  >(L_, YES);
          TEST.run<void (  rAI),  void       (  PcI)  >(L_, YES);
          TEST.run<void (  rAI),  void       (  rPI)  >(L_, NO );
          TEST.run<void (  rAI),  void       ( rPcI)  >(L_, NO );
          TEST.run<void (  rAI),  void       ( rrPI)  >(L_, !MSVC_2019
                                                         && !MSVC_2015);
          TEST.run<void (  rAI),  void       (rrPcI)  >(L_, !MSVC_2019
                                                         && !MSVC_2015);
          TEST.run<void ( rAcI),  void       (   PI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       (  PcI)  >(L_, YES);
          TEST.run<void ( rAcI),  void       (  rPI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       ( rPcI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       ( rrPI)  >(L_, NO );
          TEST.run<void ( rAcI),  void       (rrPcI)  >(L_, !MSVC_2019
                                                         && !MSVC_2015);
          TEST.run<void ( rrAI),  void       (   PI)  >(L_, YES);
          TEST.run<void ( rrAI),  void       (  PcI)  >(L_, YES);
          TEST.run<void ( rrAI),  void       (  rPI)  >(L_, NO );
          TEST.run<void ( rrAI),  void       ( rPcI)  >(L_, NO );
          TEST.run<void ( rrAI),  void       ( rrPI)  >(L_, !MSVC_2015);
          TEST.run<void ( rrAI),  void       (rrPcI)  >(L_, !MSVC_2015);
          TEST.run<void (rrAcI),  void       (   PI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       (  PcI)  >(L_, YES);
          TEST.run<void (rrAcI),  void       (  rPI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       ( rPcI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       ( rrPI)  >(L_, NO );
          TEST.run<void (rrAcI),  void       (rrPcI)  >(L_, !MSVC_2015);

          // invocations of various callable types without argument or return
          // type conversions
          TEST.run<int  (int  ),   int       (int  )  >(L_, YES);
          TEST.run<int  (int  ),   int (*   )(int  )  >(L_, YES);
          TEST.run<int  (int  ),   int (&   )(int  )  >(L_, YES);
          TEST.run<int  (int  ), F<int       (int  )> >(L_, YES);
          TEST.run<int  (B    ),   int  B::*          >(L_, YES);
          TEST.run<int  (B    ),   int (B::*)(     )  >(L_, YES);

          // invocations that perform derived-to-base and base-to-derived
          // conversions of argument types
          TEST.run<int  (B    ),   int        (B    )  >(L_, YES);
          TEST.run<int  (B    ),   int  B ::*          >(L_, YES);
          TEST.run<int  (B    ),   int (B ::*)(     )  >(L_, YES);
          TEST.run<int  (B    ),   int        (D    )  >(L_,  NO);
          TEST.run<int  (B    ),   int  D ::*          >(L_,  NO);
          TEST.run<int  (B    ),   int (D ::*)(     )  >(L_,  NO);
          TEST.run<int  (D    ),   int        (B    )  >(L_, YES);
          TEST.run<int  (D    ),   int  B ::*          >(L_, YES);
          TEST.run<int  (D    ),   int (B ::*)(     )  >(L_, YES);
          TEST.run<int  (D    ),   int        (D    )  >(L_, YES);
          TEST.run<int  (D    ),   int  D ::*          >(L_, YES);
          TEST.run<int  (D    ),   int (D ::*)(     )  >(L_, YES);

          TEST.run<int  (BA   ),   int        (BA   )  >(L_, YES);
          TEST.run<int  (BA   ),   int  BA::*          >(L_, YES);
          TEST.run<int  (BA   ),   int (BA::*)(     )  >(L_, YES);
          TEST.run<int  (BA   ),   int        (DA   )  >(L_,  NO);
          TEST.run<int  (BA   ),   int  DA::*          >(L_,  NO);
          TEST.run<int  (BA   ),   int (DA::*)(     )  >(L_,  NO);
          TEST.run<int  (DA   ),   int        (BA   )  >(L_, YES);
          TEST.run<int  (DA   ),   int  BA::*          >(L_, YES);
          TEST.run<int  (DA   ),   int (BA::*)(     )  >(L_, YES);
          TEST.run<int  (DA   ),   int        (DA   )  >(L_, YES);
          TEST.run<int  (DA   ),   int  DA::*          >(L_, YES);
          TEST.run<int  (DA   ),   int (DA::*)(     )  >(L_, YES);

#ifdef BSLS_PLATFORM_CMP_GNU
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif
          // invocations that drop the return type
          TEST.run<void (     ),      I      (     )  >(L_, YES);
          TEST.run<void (     ),     cI      (     )  >(L_, YES);
          TEST.run<void (     ),     vI      (     )  >(L_, YES);
          TEST.run<void (     ),    cvI      (     )  >(L_, YES);
          TEST.run<void (     ),     rI      (     )  >(L_, YES);
          TEST.run<void (     ),    rcI      (     )  >(L_, YES);
          TEST.run<void (     ),    rvI      (     )  >(L_, YES);
          TEST.run<void (     ),   rcvI      (     )  >(L_, YES);
          TEST.run<void (     ),    rrI      (     )  >(L_, YES);
          TEST.run<void (     ),   rrcI      (     )  >(L_, YES);

#ifdef BSLS_PLATFORM_CMP_GNU
#pragma GCC diagnostic pop
#endif

          // invocations that perform cvr-qualification conversions of
          // class return types
          TEST.run<   B (     ),      B      (     )  >(L_, YES);
          TEST.run<   B (     ),     rB      (     )  >(L_, YES);
          TEST.run<   B (     ),    rcB      (     )  >(L_, YES);
          TEST.run<   B (     ),    rrB      (     )  >(L_, YES);
          TEST.run<   B (     ),   rrcB      (     )  >(L_, YES);
          TEST.run<  rB (     ),      B      (     )  >(L_, MS_REF_BUG);
          TEST.run<  rB (     ),     rB      (     )  >(L_, YES);
          TEST.run<  rB (     ),    rcB      (     )  >(L_, NO );
          TEST.run<  rB (     ),    rrB      (     )  >(L_, MS_REF_BUG);
          TEST.run<  rB (     ),   rrcB      (     )  >(L_, NO );
          TEST.run< rcB (     ),      B      (     )  >(L_, YES);
          TEST.run< rcB (     ),     rB      (     )  >(L_, YES);
          TEST.run< rcB (     ),    rcB      (     )  >(L_, YES);
          TEST.run< rcB (     ),    rrB      (     )  >(L_, YES);
          TEST.run< rcB (     ),   rrcB      (     )  >(L_, YES);
          TEST.run< rrB (     ),      B      (     )  >(L_, YES);
          TEST.run< rrB (     ),     rB      (     )  >(L_, YES);
          TEST.run< rrB (     ),    rcB      (     )  >(L_, NO );
          TEST.run< rrB (     ),    rrB      (     )  >(L_, YES);
          TEST.run< rrB (     ),   rrcB      (     )  >(L_, NO );
          TEST.run<rrcB (     ),      B      (     )  >(L_, YES);
          TEST.run<rrcB (     ),     rB      (     )  >(L_, YES);
          TEST.run<rrcB (     ),    rcB      (     )  >(L_, YES);
          TEST.run<rrcB (     ),    rrB      (     )  >(L_, YES);
          TEST.run<rrcB (     ),   rrcB      (     )  >(L_, YES);

          // invocations that perform explicit conversions of cvr-qualified
          // class return types
          TEST.run<   E (     ),      B      (     )  >(L_, YES);
          TEST.run<   E (     ),     rB      (     )  >(L_, YES);
          TEST.run<   E (     ),    rcB      (     )  >(L_, YES);
          TEST.run<   E (     ),    rrB      (     )  >(L_, YES);
          TEST.run<   E (     ),   rrcB      (     )  >(L_, YES);
          TEST.run<  rE (     ),      B      (     )  >(L_, NO );
          TEST.run<  rE (     ),     rB      (     )  >(L_, NO );
          TEST.run<  rE (     ),    rcB      (     )  >(L_, NO );
          TEST.run<  rE (     ),    rrB      (     )  >(L_, NO );
          TEST.run<  rE (     ),   rrcB      (     )  >(L_, NO );
          TEST.run< rcE (     ),      B      (     )  >(L_, NO );
          TEST.run< rcE (     ),     rB      (     )  >(L_, NO );
          TEST.run< rcE (     ),    rcB      (     )  >(L_, NO );
          TEST.run< rcE (     ),    rrB      (     )  >(L_, NO );
          TEST.run< rcE (     ),   rrcB      (     )  >(L_, NO );
          TEST.run< rrE (     ),      B      (     )  >(L_, NO );
          TEST.run< rrE (     ),     rB      (     )  >(L_, NO );
          TEST.run< rrE (     ),    rcB      (     )  >(L_, NO );
          TEST.run< rrE (     ),    rrB      (     )  >(L_, NO );
          TEST.run< rrE (     ),   rrcB      (     )  >(L_, NO );
          TEST.run<rrcE (     ),      B      (     )  >(L_, NO );
          TEST.run<rrcE (     ),     rB      (     )  >(L_, NO );
          TEST.run<rrcE (     ),    rcB      (     )  >(L_, NO );
          TEST.run<rrcE (     ),    rrB      (     )  >(L_, NO );
          TEST.run<rrcE (     ),   rrcB      (     )  >(L_, NO );

          // invocations that perform derived-to-base conversions of
          // cvr-qualified class return types
          TEST.run<   B (     ),      D      (     )  >(L_, YES);
          TEST.run<   B (     ),     rD      (     )  >(L_, YES);
          TEST.run<   B (     ),    rcD      (     )  >(L_, YES);
          TEST.run<   B (     ),    rrD      (     )  >(L_, YES);
          TEST.run<   B (     ),   rrcD      (     )  >(L_, YES);
          TEST.run<  rB (     ),      D      (     )  >(L_, MS_REF_BUG);
          TEST.run<  rB (     ),     rD      (     )  >(L_, YES);
          TEST.run<  rB (     ),    rcD      (     )  >(L_, NO );
          TEST.run<  rB (     ),    rrD      (     )  >(L_, MS_REF_BUG);
          TEST.run<  rB (     ),   rrcD      (     )  >(L_, NO );
          TEST.run< rcB (     ),      D      (     )  >(L_, YES);
          TEST.run< rcB (     ),     rD      (     )  >(L_, YES);
          TEST.run< rcB (     ),    rcD      (     )  >(L_, YES);
          TEST.run< rcB (     ),    rrD      (     )  >(L_, YES);
          TEST.run< rcB (     ),   rrcD      (     )  >(L_, YES);
          TEST.run< rrB (     ),      D      (     )  >(L_, YES);
          TEST.run< rrB (     ),     rD      (     )  >(L_, YES);
          TEST.run< rrB (     ),    rcD      (     )  >(L_, NO );
          TEST.run< rrB (     ),    rrD      (     )  >(L_, YES);
          TEST.run< rrB (     ),   rrcD      (     )  >(L_, NO );
          TEST.run<rrcB (     ),      D      (     )  >(L_, YES);
          TEST.run<rrcB (     ),     rD      (     )  >(L_, YES);
          TEST.run<rrcB (     ),    rcD      (     )  >(L_, YES);
          TEST.run<rrcB (     ),    rrD      (     )  >(L_, YES);
          TEST.run<rrcB (     ),   rrcD      (     )  >(L_, YES);

          // invocations that perform base-to-derived conversions of
          // cvr-qualified class return types
          TEST.run<   D (     ),      B      (     )  >(L_, NO );
          TEST.run<   D (     ),     rB      (     )  >(L_, NO );
          TEST.run<   D (     ),    rcB      (     )  >(L_, NO );
          TEST.run<   D (     ),    rrB      (     )  >(L_, NO );
          TEST.run<   D (     ),   rrcB      (     )  >(L_, NO );
          TEST.run<  rD (     ),      B      (     )  >(L_, MS_REF_BUG);
          TEST.run<  rD (     ),     rB      (     )  >(L_, YES);
          TEST.run<  rD (     ),    rcB      (     )  >(L_, NO );
          TEST.run<  rD (     ),    rrB      (     )  >(L_, MS_REF_BUG);
          TEST.run<  rD (     ),   rrcB      (     )  >(L_, NO );
          TEST.run< rcD (     ),      B      (     )  >(L_, MSVC);
          TEST.run< rcD (     ),     rB      (     )  >(L_, YES);
          TEST.run< rcD (     ),    rcB      (     )  >(L_, YES);
          TEST.run< rcD (     ),    rrB      (     )  >(L_, MSVC);
          TEST.run< rcD (     ),   rrcB      (     )  >(L_, MSVC);
          TEST.run< rrD (     ),      B      (     )  >(L_, YES);
          TEST.run< rrD (     ),     rB      (     )  >(L_, YES);
          TEST.run< rrD (     ),    rcB      (     )  >(L_, NO );
          TEST.run< rrD (     ),    rrB      (     )  >(L_, YES);
          TEST.run< rrD (     ),   rrcB      (     )  >(L_, NO );
          TEST.run<rrcD (     ),      B      (     )  >(L_, YES);
          TEST.run<rrcD (     ),     rB      (     )  >(L_, YES);
          TEST.run<rrcD (     ),    rcB      (     )  >(L_, YES);
          TEST.run<rrcD (     ),    rrB      (     )  >(L_, YES);
          TEST.run<rrcD (     ),   rrcB      (     )  >(L_, YES);

          TEST.run<   DA(     ),      BA     (     )  >(L_, CPP_20 && !CLANG);
          TEST.run<   DA(     ),     rBA     (     )  >(L_, CPP_20 && !CLANG);
          TEST.run<   DA(     ),    rcBA     (     )  >(L_, CPP_20 && !CLANG);
          TEST.run<   DA(     ),    rrBA     (     )  >(L_, CPP_20 && !CLANG);
          TEST.run<   DA(     ),   rrcBA     (     )  >(L_, CPP_20 && !CLANG);
          TEST.run<  rDA(     ),      BA     (     )  >(L_, MS_REF_BUG);
          TEST.run<  rDA(     ),     rBA     (     )  >(L_, YES);
          TEST.run<  rDA(     ),    rcBA     (     )  >(L_, NO );
          TEST.run<  rDA(     ),    rrBA     (     )  >(L_, MS_REF_BUG);
          TEST.run<  rDA(     ),   rrcBA     (     )  >(L_, NO );
          TEST.run< rcDA(     ),      BA     (     )  >(L_, MSVC);
          TEST.run< rcDA(     ),     rBA     (     )  >(L_, YES);
          TEST.run< rcDA(     ),    rcBA     (     )  >(L_, YES);
          TEST.run< rcDA(     ),    rrBA     (     )  >(L_, MSVC);
          TEST.run< rcDA(     ),   rrcBA     (     )  >(L_, MSVC);
          TEST.run< rrDA(     ),      BA     (     )  >(L_, YES);
          TEST.run< rrDA(     ),     rBA     (     )  >(L_, YES);
          TEST.run< rrDA(     ),    rcBA     (     )  >(L_, NO );
          TEST.run< rrDA(     ),    rrBA     (     )  >(L_, YES);
          TEST.run< rrDA(     ),   rrcBA     (     )  >(L_, NO );
          TEST.run<rrcDA(     ),      BA     (     )  >(L_, YES);
          TEST.run<rrcDA(     ),     rBA     (     )  >(L_, YES);
          TEST.run<rrcDA(     ),    rcBA     (     )  >(L_, YES);
          TEST.run<rrcDA(     ),    rrBA     (     )  >(L_, YES);
          TEST.run<rrcDA(     ),   rrcBA     (     )  >(L_, YES);


#ifdef BSLS_PLATFORM_CMP_CLANG
#pragma clang diagnostic pop
#endif

#if defined(BSLS_PLATFORM_CMP_GNU) && BSLS_PLATFORM_CMP_VERSION >= 100000
#pragma GCC diagnostic pop
#endif

#endif // defined(BSLSTL_FUNCTION_INVOKERUTIL_SUPPORT_IS_FUNC_INVOCABLE)

      } break;
      case 5: {
        // --------------------------------------------------------------------
        // FUNCTION OBJECT TARGET
        //  This case tests that `invokerForFunc` returns the correct invoker
        //  for functor-class targets.  In the concerns and plan, `func` is a
        //  functor object, `rep` is a `bslstl::Function_Rep` that holds a copy
        //  of `func`, `REP` is a const reference to `rep` `PROTOTYPE` is a
        //  function prototype, `RET(ARGS...)`  is the same function prototype,
        //  where `RET` is the return type and `ARGS...` is the argument types,
        //  and `args...` is the list of invocation arguments of type
        //  `ARGS...`.
        //
        // Concerns:
        // 1. Casting the return value of `invokerForFunc<PROTOTYPE>(func)` to
        //    `RET(*)(Function_Rep *, ARGS...)` yields a pointer to function,
        //    `inv_p` such that `inv_p(&rep, args...)` returns `func(args...)`.
        //
        // 2. Invocation works for zero to ten arguments and yields the
        //    expected return value.
        //
        // 3. The prototype for `func` need not be an exact match for the
        //    PROTOTYPE parameter of `invokerForFunc`; so long as each type in
        //    `ARGS...` is implicitly convertible to the corresponding argument
        //    to `func` and the return type of invocation through `func` is
        //    implicitly convertible to `RET`.
        //
        // 4. If `RET` is `void`, then the return value of invocation is
        //    discarded, even if the return type of the `func` is non-void.
        //
        // 5. Arguments that are supposed to be passed by (lvalue or rvalue)
        //    reference *are* passed by reference all the way through the
        //    invocation interface.
        //
        // 6. Invocation works correctly whether the functor fits within the
        //    small-object optimization or is allocated on the heap.
        //
        // 7. Side effects are observed even if the `bsl::function` is const.
        //    This surprising fact comes from the idea that a `function` object
        //    is an abstraction of a pointer to a function.  Moreover, type
        //    erasure means that, at compile time, it is not possible to
        //    determine whether the callable object even cares whether or not
        //    it is const.
        //
        // 8. When the functor argument is wrapped using a
        //    `bslalg::NothrowMovableWrapper`, invocation procedes normally.
        //    The return value of `invokerForFunc` will change if wrapping
        //    `func` causes it to qualify for the small object optimization
        //    when the unwrapped `func` did not; otherwise the call to
        //    `invokerForFunc` returns the same pointer for both the wrapped
        //    and unwrapped `func`.
        //
        // Plan:
        // 1. Create a functor, `func` and pass it to
        //    `invokerForFunc<PROTOTYPE>`, where `PROTOTYPE` matches the
        //    prototype of a call operator in
        //    `func`'s interface, yielding invoker `inv_p` after casting to the
        //    specific invoker type.  Set `func` as the target of a
        //    `Function_Rep` object, `rep`.  Call `inv_p(&REP, args...)`, where
        //    the `args` have the correct type for the function signature.
        //    Verify that this invocation returns the correct value and has the
        //    correct side-effect on `rep->target()`.  (C-1)
        //
        // 2. Repeat the previous step for `PROTOTYPE` having 0 to 13
        //    arguments.  (C-2)
        //
        // 3. Instantiate `invokerForFunc` with a `PROTOTYPE` having arguments
        //    of type `ConvertibleToInt` and return type `IntWrapper` instead
        //    of `int`.  Verify that invocation works the same way as before,
        //    with conversions occuring automatically.  (C-3)
        //
        // 4. Instantiate `invokerForFunc` with a `RET` of `void`.  Verify that
        //    a call operator that returns void and has side effects can be
        //    invoked, discarding the return value but having all expected side
        //    effects.  Verify that a call operator that returns non-void can
        //    be invoked, discarding the result.  (C-4)
        //
        // 5. Invoke a call operator that returns the address of its argument,
        //    which is passed by lvalue reference.  Test `invokerForFunc` with
        //    a `PROTOTYPE` that unambiguosly selects this call operator and
        //    verify that invocation returns the address of its argument.
        //    Repeat with a movable reference.  (C-5)
        //
        // 6. Perform the previous steps with a small functor that fits in the
        //    small-object buffer, a small functor that is disqualified from
        //    the small-object optimization because of a throwing move
        //    constructor, and a large functor that does not fit in the
        //    small-object buffer.  Verify that these functors produce
        //    identical results when the return value of `invokerForFunc` is
        //    called.  (C-6)
        //
        // 7. When calling `inv_p`, the first argument should be a const
        //    reference to `rep` (i.e., `REP`), proving that the constness of
        //    the `Function_Rep` does not affect side effects on the functor.
        //    (C-7)
        //
        // 8. Call `invokerForFunc` twice: with both `func` and with
        //    `bslalg::NothrowMovableUtil::wrap(func)`.  Verify that the
        //    results of calling the returned invoker are the same.  Verify
        //    that the returned invoker type and value compare of both calls
        //    return equal for the small and large functors but unequal for the
        //    small functor with throwing move constructor.  (C-8)
        //
        // Testing:
        //  invokerForFunc(const FUNC& f); // `FUNC` == user-defined functor
        // --------------------------------------------------------------------

        if (verbose) printf("\nFUNCTION OBJECT TARGET"
                            "\n======================\n");

        if (veryVerbose) printf("Plan step 1-5\n");
        testFunctor<SmallFunctor        , int, int>();
        testFunctor<SmallFunctor        , IntWrapper, ConvertibleToInt>();

        if (veryVerbose) printf("Plan step 7\n");
        testFunctor<SmallThrowingFunctor, int, int>();
        testFunctor<LargeFunctor        , int, int>();
        testFunctor<SmallThrowingFunctor, IntWrapper, ConvertibleToInt>();
        testFunctor<LargeFunctor        , IntWrapper, ConvertibleToInt>();

        if (veryVerbose) printf("Plan step 8\n");
        SmallFunctor         sf(1);
        SmallThrowingFunctor stf(2);
        LargeFunctor         lf(3);

        ASSERT(Util::invokerForFunc<int()>(sf) ==
               Util::invokerForFunc<int()>(NTWRAP(sf)));
        ASSERT(Util::invokerForFunc<int()>(stf) !=
               Util::invokerForFunc<int()>(NTWRAP(stf)));
        ASSERT(Util::invokerForFunc<int()>(lf) ==
               Util::invokerForFunc<int()>(NTWRAP(lf)));

      } break;

      case 4: {
        // --------------------------------------------------------------------
        // POINTER TO DATA MEMBER TARGET
        //  This case tests that `invokerForFunc` returns the correct invoker
        //  for pointer-to-data-member targets.  In the concerns and plan,
        //  `pdm` is a pointer to a public data member of type `M` of class
        //  `FT` (of type `M FT::*`), `rep` is a `bslstl::Function_Rep` that
        //  holds a copy of `pdm`, `PROTOTYPE` is a function prototype,
        //  `RET(T)` is the same function prototype, where `RET` is the return
        //  type, `T` is a class type, reference-to-class type, or
        //  pointer-to-class type, and `obj` is an invocation argument of type
        //  `T`.
        //
        // Concerns:
        // 1. If `T` is the same as `FT` calling the invoker with arguments
        //    `(&rep, obj)` yields the specified member of `obj`, by-value
        //    converting to alternate types or by reference if `RET` is
        //    compatible reference type.
        //
        // 2. Verify the same works if `M` is a `const` member.
        //
        // 3. Arguments of type pointer to `T`, reference to `T`, and
        //    smart-pointer to `T`, should all be valid invoker arguments.
        //
        // 4. Concern 3 should apply with a target type that is
        //    compatible with `const T`.
        //
        // 5. Concerns 1-4 should apply for invoker arguments that refer to a
        //    type derived from `T`.
        //
        // 6. When the `pdm` is wrapped using `bslalg::NothrowMovableWrapper`
        //    the invocation procedes as though the wrapper were not present.
        //
        // Plan:
        // 1. Create a class `DataWrapper<TYPE>` that has a public data member
        //    of the specified `TYPE`.
        //
        // 2. Test the invoker returned by `invokerForFunc` using a `PROTOTYPE`
        //    where `T` is `DataWrapper<int>` and the `pdm` is a pointer to the
        //    `d_t` member of that type.  Verify that the return value is
        //    produced properly where `RET` is `int`, `long`, `int&`, and
        //    `const int&`.  (C-1)
        //
        // 3. Test the same invocation for `DataWrapper<const int>` where `RET`
        //    is `int`, `long`, and `const int&`.  (C-2)
        //
        // 4. Steps 2 and 3 also apply if `T` is a reference to, object of,
        //    pointer to or smart-pointer to a `DataWrapper`.  (C-3)
        //
        // 5. Step 3 also applies if `T` is a reference to, object of,
        //    pointer to or smart-pointer to a `const DataWrapper`.  (C-4)
        //
        // 6. Steps 2-5 also apply if `T` is refers to a class derived from
        //    `DataWrapper<TYPE>`.  (C-5)
        //
        // 7. Call `invokerForFunc` twice: once with 'pdm and once with
        //    `bslalg::NothrowMovableUtil::wrap(pdm)`.  Verify that the return
        //    type and value of both calls compare equal.  (C-6)
        //
        // Testing:
        //  invokerForFunc(const FUNC& f); // `FUNC` == ptr to data member
        // --------------------------------------------------------------------

        if (verbose) printf("\nPOINTER TO DATA MEMBER TARGET"
                            "\n=============================\n");

        typedef DataWrapper<      int>        DW;
        typedef DataWrapper<const int>        DWC;
        typedef DerivedDataWrapper<      int> DDW;
        typedef DerivedDataWrapper<const int> DDWC;

        int                        DW::*const pdm  = &DW::d_t;
        const int                 DWC::*const cpdm = &DWC::d_t;

        if (veryVerbose) printf("Plan step 2: DataWrapper<int>\n");
        testPtrToDataMember<           int ,       DW >( pdm);
        testPtrToDataMember<     const int ,       DW >( pdm);
        testPtrToDataMemberByRef<      int&,       DW&>( pdm);
        testPtrToDataMemberByRef<const int&, const DW&>( pdm);

        if (veryVerbose) printf("Plan step 3: DataWrapper<const int>\n");
        testPtrToDataMember<           int ,       DWC >(cpdm);
        testPtrToDataMember<     const int ,       DWC >(cpdm);
        testPtrToDataMemberByRef<const int&,       DWC&>(cpdm);
        testPtrToDataMemberByRef<const int&, const DWC&>(cpdm);

        if (veryVerbose) printf("Plan step 4: ptr to DataWrapper\n");
        testPtrToDataMemberByRef<      int&,                DW  *>( pdm);
        testPtrToDataMemberByRef<const int&,                DWC *>(cpdm);
        testPtrToDataMemberByRef<      int&, SmartPtr<      DW > >( pdm);
        testPtrToDataMemberByRef<const int&, SmartPtr<      DWC> >(cpdm);

        if (veryVerbose) printf("Plan step 5: ptr to const DataWrapper\n");
        testPtrToDataMemberByRef<const int&,          const DW  *>( pdm);
        testPtrToDataMemberByRef<const int&,          const DWC *>(cpdm);
        testPtrToDataMemberByRef<const int&, SmartPtr<const DW > >( pdm);
        testPtrToDataMemberByRef<const int&, SmartPtr<const DWC> >(cpdm);

        if (veryVerbose) printf("Plan step 6: DataWrapperDerived\n");
        testPtrToDataMember<           int ,       DDW >( pdm);
        testPtrToDataMember<     const int ,       DDW >( pdm);
        testPtrToDataMemberByRef<      int&,       DDW&>( pdm);
        testPtrToDataMemberByRef<const int&, const DDW&>( pdm);
        testPtrToDataMember<           int ,       DDWC >(cpdm);
        testPtrToDataMember<     const int ,       DDWC >(cpdm);
        testPtrToDataMemberByRef<const int&,       DDWC&>(cpdm);
        testPtrToDataMemberByRef<const int&, const DDWC&>(cpdm);
        testPtrToDataMemberByRef<      int&,                DDW  *>( pdm);
        testPtrToDataMemberByRef<const int&,                DDWC *>(cpdm);
        testPtrToDataMemberByRef<      int&, SmartPtr<      DDW > >( pdm);
        testPtrToDataMemberByRef<const int&, SmartPtr<      DDWC> >(cpdm);
        testPtrToDataMemberByRef<const int&,          const DDW  *>( pdm);
        testPtrToDataMemberByRef<const int&,          const DDWC *>(cpdm);
        testPtrToDataMemberByRef<const int&, SmartPtr<const DDW > >( pdm);
        testPtrToDataMemberByRef<const int&, SmartPtr<const DDWC> >(cpdm);

        if (veryVerbose) printf("Plan step 7: NothrowMovableWrapper\n");
        ASSERT(Util::invokerForFunc<int(DW&)>(&DW::d_t) ==
               Util::invokerForFunc<int(DW&)>(NTWRAP(&DW::d_t)));

      } break;

      case 3: {
        // --------------------------------------------------------------------
        // POINTER TO MEMBER FUNCTION TARGET
        //  This case tests that `invokerForFunc` returns the correct invoker
        //  for pointer-to-member-function targets.  In the concerns and plan,
        //  `pmf` is a pointer to a non-static function member of class `FT`,
        //  `rep` is a `bslstl::Function_Rep` that holds a copy of `pmf`,
        //  `PROTOTYPE` is a function prototype, `RET(T, ARGS...)`  is the same
        //  function prototype, where `RET` is the return type, `T` is a class
        //  type, reference-to-class type, or pointer-to-class type, `ARGS...`
        //  is the argument types, `obj` is an invocation argument of type `T`,
        //  and `args...` is the list of invocation arguments of type
        //  `ARGS...`.
        //
        // Concerns:
        // 1. If `T` is the same as `FT` (i.e., `obj` is passed by value),
        //    calling the invoker with arguments `(&rep, obj, args...)` yields
        //    the same return value as invoking `(obj.*pmf)(args...)` and will
        //    have no side effect on `obj`.
        //
        // 2. If `T` is the same as `FT&`, calling the invoker with arguments
        //    `(&rep, obj, args...)` yields the same return value and
        //    side-effect as invoking `(obj.*pmf)(args...)`.
        //
        // 3. If `T` is a pointer to `FT` or smart pointer to `FT`, calling the
        //    invoker with arguments `(&rep, obj, args...)` yields the same
        //    results and side effects as invoking `((*obj).*pmf)(args...)`.
        //
        // 4. Invocation works for zero to nine arguments, `args...` in
        //    addition to the `obj` argument and yields the expected return
        //    value.
        //
        // 5. The template argument types `ARGS...` need not match the
        //    member-function arguments exactly, so long as the argument lists
        //    are the same length and each type in `ARGS` is implicitly
        //    convertible to the corresponding argument in the call to `pmf`.
        //    Similarly, `RET` need not match the member-function return type
        //    exactly so long as it is implicitly convertible to
        //    member-function's return type.
        //
        // 6. If `pmf` is a pointer to const member function, then `T` can be
        //    an rvalue of, reference to, pointer to, or smart-pointer to
        //    either a const or non-const type compatible with `T`.
        //
        // 7. Concerns 2, 3, and 4 also apply if `T` is a reference to, object
        //    of, pointer to, or smart-pointer to, a type derived from `FT`.
        //
        // 8. If `RET` is `void`, then the return value of `pmf` is discarded,
        //    even if `pmf` returns non-void.
        //
        // 9. When the `pmf` is wrapped using `bslalg::NothrowMovableWrapper`,
        //    the invocation procedes as though the wrapper were not present.
        //
        // 10. Arguments that are passed by lvalue or rvalue reference are
        //    passed by reference all the way down to the invoked member
        //    function.
        //
        // Plan:
        // 1. Create a class `IntWrapper` that holds an `int` value and has
        //    const member functions `add0` to `add12` and non-const member
        //    functions `increment0` to `increment12`, each taking 0 to 12
        //    `int` arguments.
        //
        // 2. Test the invoker returned by `invokerForFunc` using a `PROTOTYPE`
        //    where `T` is `IntWrapper` and the `pmf` is a pointer to one of
        //    the `increment` methods.  Verify that, when the invoker is
        //    called, the expected value is returned and no side effect is
        //    observed on the (pass-by-value) `IntWrapper` argument.  (C-1)
        //
        // 3. Test the invoker returned by `invokerForFunc` using a `PROTOTYPE`
        //    where `T` is `IntWrapper&` and the `pmf` is a pointer to one of
        //    the non-const methods.  Verify that, when the invoker is called,
        //    the expected value is returned and the expected side effect is
        //    observed on the (pass-by-reference) `IntWrapper` argument.  (C-1)
        //
        // 4. Repeat step 3, except with `T` being `IntWrapper *` and the first
        //    argument being the address if an `IntWrapper`.  Repeat again with
        //    `T` being `SmartPtr<IntWrapper>`.  (C-3)
        //
        // 5. Repeat the above tests with 0 to 12 arguments in addition to the
        //    initial `IntWrapper` argument.  (C-4)
        //
        // 6. Repeat the above tests, using a prototype of
        //    `IntWrapper(IntWrapper&, ConvertibleToInt...)` while maintaining
        //    a target with prototype `int(IntWrapper&, int...)`.  Verify that
        //    the behavior is unchanged from the exact-match prototype.  (C-5)
        //
        // 7. Test selected invocations (it is not necessary to test all 14
        //    argument lists) with `T` being `const IntWrapper`,
        //    `const IntWrapper&`, `const IntWrapper *`, and
        //    `SmartPtr<const IntWrapper>` and using const member functions.
        //    (C-6)
        //
        // 8. Derive a class, `IntWrapperDerived`, from `IntWrapper`.  Repeat
        //    selected cases using `IntWrapperDerived` instead of `IntWrapper`
        //    for the first argument of `PROTOTYPE` but using a pointer to
        //    member of `IntWrapper` as the target callable object.  (C-7)
        //
        // 9. Repeat selected cases using a `RET` type of `void`.  Verify that,
        //    although the return value is discarded, side effects (if any)
        //    still take effect.  (C-8)
        //
        // 10. Call `invokerForFunc` twice: once with `pmf` and once with
        //    `bslalg::NothrowMovableUtil::wrap(pmf)`.  Verify that the return
        //    type and value compare of both calls return equal.  (C-9)
        //
        // 11. Call the result of `invokerForFunc` with `pmf` being a pointer
        //    to member function that takes an argument by lvalue reference and
        //    test that the argument value is modified by the call.  Repeat
        //    with an rvalue reference.  It is not necessary to test more than
        //    one argument  (C-10)
        //
        // Testing:
        //  invokerForFunc(const FUNC& f); // `FUNC` == ptr to member function
        // --------------------------------------------------------------------

        if (verbose) printf("\nPOINTER TO MEMBER FUNCTION TARGET"
                            "\n=================================\n");

        bslma::TestAllocator ta;
        Rep                  rep(&ta);

        if (veryVerbose) printf("Plan step 2: IntWrapper\n");
        testPtrToMemFunc<IntWrapper, int, int>();

        if (veryVerbose) printf("Plan step 3: IntWrapper&\n");
        testPtrToMemFunc<IntWrapper&, int, int>();

        if (veryVerbose) printf("Plan step 4: IntWrapper* and SmartPtr\n");
        testPtrToMemFunc<IntWrapper*, int, int>();
        testPtrToMemFunc<SmartPtr<IntWrapper>, int, int>();

        if (veryVerbose) printf("Plan step 6: non-exact prototype match\n");
        testPtrToMemFunc<IntWrapper, IntWrapper, ConvertibleToInt>();
        testPtrToMemFunc<IntWrapper&, IntWrapper, ConvertibleToInt>();
        testPtrToMemFunc<IntWrapper*, IntWrapper, ConvertibleToInt>();
        testPtrToMemFunc<SmartPtr<IntWrapper>, IntWrapper, ConvertibleToInt>();

        if (veryVerbose) printf("Plan step 7: const class object\n");
        testPtrToConstMemFunc<IntWrapper, int, int>();
        testPtrToConstMemFunc<IntWrapper&, int, int>();
        testPtrToConstMemFunc<IntWrapper*, int, int>();
        testPtrToConstMemFunc<SmartPtr<IntWrapper>, int, int>();
        testPtrToConstMemFunc<const IntWrapper, int, int>();
        testPtrToConstMemFunc<const IntWrapper&, int, int>();
        testPtrToConstMemFunc<const IntWrapper*, int, int>();
        testPtrToConstMemFunc<SmartPtr<const IntWrapper>, int, int>();

        if (veryVerbose) printf("Plan step 8: derived class\n");
        testPtrToConstMemFunc<IntWrapperDerived, int, int>();
        testPtrToConstMemFunc<IntWrapperDerived&, int, int>();
        testPtrToConstMemFunc<IntWrapperDerived*, int, int>();
        testPtrToConstMemFunc<SmartPtr<IntWrapperDerived>, int, int>();
        testPtrToConstMemFunc<const IntWrapperDerived, int, int>();
        testPtrToConstMemFunc<const IntWrapperDerived&, int, int>();
        testPtrToConstMemFunc<const IntWrapperDerived*, int, int>();
        testPtrToConstMemFunc<SmartPtr<const IntWrapperDerived>, int, int>();

        if (veryVerbose) printf("Plan step 10: NothrowMovableWrapper\n");
        ASSERT(Util::invokerForFunc<int(IntWrapper&,int)>(&IntWrapper::add1) ==
               Util::invokerForFunc<int(IntWrapper&,int)>(
                   NTWRAP(&IntWrapper::add1)));
        ArgGenerator<IntWrapper&> gen;
        ASSERT((getInvoker<int, IntWrapper&, int>
                (&rep, &IntWrapper::add1)(&rep, gen.obj(), 99)) ==
               (getInvoker<int, IntWrapper&, int>
                (&rep, NTWRAP(&IntWrapper::add1))(&rep, gen.obj(), 99)));

        typedef IntWrapper       Ret;
        typedef ConvertibleToInt Arg;

        ASSERT(Util::invokerForFunc<Ret(IntWrapper&, Arg, Arg)>
               (&IntWrapper::add2) ==
               Util::invokerForFunc<Ret(IntWrapper&, Arg, Arg)>
               (NTWRAP(&IntWrapper::add2)));

        if (veryVerbose) printf("Plan step 11: reference arguments\n");
        IntWrapper classArg(0x2000);
        int        lvArg = 1;
        ASSERT(0x2001 == (getInvoker<int, const IntWrapper&, int&>
                          (&rep, &IntWrapper::lvadd1)
                          (&rep, classArg, lvArg)));
        ASSERT(-1 == lvArg);

        lvArg = 2;
        ASSERT(0x2002 == (getInvoker<int, const IntWrapper&,
                          bslmf::MovableRef<int> >
                          (&rep, &IntWrapper::rvadd1)
                          (&rep, classArg,
                           bslmf::MovableRefUtil::move(lvArg))));
        ASSERT(-1 == lvArg);

      } break;

      case 2: {
        // --------------------------------------------------------------------
        // POINTER TO FUNCTION TARGET
        //  This case tests that `invokerForFunc` returns the correct invoker
        //  for pointer-to-function targets.  In the concerns and plan, `pf` is
        //  a pointer to function, `rep` is a `bslstl::Function_Rep` that holds
        //  a copy of `pf`, `PROTOTYPE` is a function prototype, `RET(ARGS...)`
        //  is the same function prototype, where `RET` is the return type and
        //  `ARGS...` is the argument types, and `args...` is the list of
        //  invocation arguments of type `ARGS...`.
        //
        // Concerns:
        // 1. `bslstl::Function_InvokerUtil::invokerForFunc<PROTOTYPE>(pf)`
        //    returns a null pointer if `pf` is `nullptr` or a null function
        //    pointer.
        //
        // 2. Casting the return value of `invokerForFunc<PROTOTYPE>(pf)` to
        //    `RET(*)(Function_Rep *, ARGS...)` yields a pointer to function,
        //    `inv_p` such that `inv_p(&rep, args...)` returns `pf(args...)`.
        //
        // 3. Invocation works for zero to ten arguments and yields the
        //    expected return value.
        //
        // 4. The prototype for `pf` need not be an exact match for the
        //    PROTOTYPE parameter of `invokerForFunc`; so long as each type in
        //    `ARGS...` is implicitly convertible to the corresponding argument
        //    to `pf` and the return type of invocation through `pf` is
        //    implicitly convertible to `RET`.
        //
        // 5. If `RET` is `void`, then the return value of invocation is
        //    discarded, even if the return type of the `pf` is non-void.
        //
        // 6. Arguments that are supposed to be passed by reference *are*
        //    passed by reference all the way through the invocation interface.
        //
        // 7. Arguments that are supposed to be passed by rvalue reference
        //    *are passed by rvalue reference all the way through the
        //    invocation interface.
        //
        // 8. When the function pointer argument is wrapped using a
        //    `bslalg::NothrowMovableWrapper`, invocation procedes as though
        //    the wrapper were not present.
        //
        // Plan:
        // 1. Create a null pointer to function.  Invoke `invokerForFunc` on
        //    that null pointer.  Verify that the return value is null.  (C-1)
        //
        // 2. Set `pf` to the address of function `f` and pass it to
        //    `invokerForFunc<PROTOTYPE>`, where `PROTOTYPE` matches the
        //    `f`'s prototype, yielding invoker `inv_p` after casting to the
        //    specific invoker type.  Set the address of `f` as the target of a
        //    `Function_Rep` object, `rep`.  Call `inv_p(&rep, args...)`, where
        //    the `args` have the correct type for the function signature.
        //    Verify that this invocation returns the correct value.  Repeat
        //    this step for `f` having 0 to 13 arguments.  (C-2, 3)
        //
        // 3. Instantiate `invokerForFunc` with a `PROTOTYPE` having arguments
        //    of type `ConvertibleToInt` and return type `IntWrapper` instead
        //    of `int`.  Verify that invocation works the same way as before,
        //    with conversions occuring automatically.  It is not necessary to
        //    try every combination of 0 to 13 arguments.  (C-4)
        //
        // 4. Instantiate `invokerForFunc` with a `RET` of `void`.  Verify that
        //    a function that returns void and has side effects can be invoked,
        //    discarding the return value but having all expected side effects.
        //    Verify that a function that returns non-void can be invoked,
        //    discarding the result.  (C-5)
        //
        // 5. Implement a set of functions, `getAddress` and `getConstAddress`
        //    that return the address of their argument, which is passed by
        //    reference and passed by const reference, respectively.  Test
        //    `invokerForFunc` with pointers to these functions and verify that
        //    invocation returns the address of their arguments.  (C-6)
        //
        // 6. Instantiate `invokeForFunc` with a `PROTOTPYE` having arguments
        //    of type `bslmf::MovableRef<int>` and call a function with that
        //    signature which will reset each argument to `-1` and return the
        //    sum of their values.  Verify that this invocation returns the
        //    correct value and resets all parameters. (C-7)
        //
        // 7. Call `invokerForFunc` twice: once with `pf` and once with
        //    `bslalg::NothrowMovableUtil::wrap(pf)`.  Verify that the return
        //    type and value compare of both calls return equal.  (C-8)
        //
        // Testing:
        //  invokerForFunc(const bsl::nullptr_t&);
        //  invokerForFunc(const FUNC& f); // `FUNC` == ptr-to-function
        // --------------------------------------------------------------------

        if (verbose) printf("\nPOINTER TO FUNCTION TARGET"
                            "\n==========================\n");

        bslstl::Function_Rep rep(&ta);

        if (veryVerbose) printf("Plan step 1: Null invoker\n");
        ASSERT((0 == getInvoker<int, int, int>(&rep, bsl::nullptr_t())));
        int (*nullFuncPtr)(int, int) = 0;
        ASSERT((0 == getInvoker<int, int, int>(&rep, nullFuncPtr)));

        if (veryVerbose) printf("Plan step 2: normal arguments\n");
        ASSERT((0x4000 == getInvoker<int>(&rep, &sum0)(&rep)));
        ASSERT((0x4001 == getInvoker<int, int>(&rep, &sum1)(&rep, 1)));
        ASSERT((0x4003 == getInvoker<int, int, int>(&rep, &sum2)(&rep, 1, 2)));
        ASSERT((0x4007 == getInvoker<int, int, int, int>(&rep, &sum3)
                (&rep, 1, 2, 4)));
        ASSERT((0x400f == getInvoker<int, int, int, int, int>(&rep, &sum4)
                (&rep, 1, 2, 4, 8)));
        ASSERT((0x401f == getInvoker<int, int, int, int, int, int>(&rep, &sum5)
                (&rep, 1, 2, 4, 8, 0x10)));
        ASSERT((0x403f == getInvoker<int, int, int, int, int, int, int>
                (&rep, &sum6)
                (&rep, 1, 2, 4, 8, 0x10, 0x20)));
        ASSERT((0x407f == getInvoker<int, int, int, int, int, int, int, int>
                (&rep, &sum7)
                (&rep, 1, 2, 4, 8, 0x10, 0x20, 0x40)));
        ASSERT((0x40ff == getInvoker<int, int, int, int, int, int, int, int,
                                     int>
                (&rep, &sum8)
                (&rep, 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80)));
        ASSERT((0x41ff == getInvoker<int, int, int, int, int, int, int, int,
                                     int, int>
                (&rep, &sum9)
                (&rep, 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100)));
        ASSERT((0x43ff == getInvoker<int, int, int, int, int, int, int, int,
                                     int, int, int>
                (&rep, &sum10)
                (&rep, 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200)));

        if (veryVerbose) printf("Plan step 3: converting arguments\n");
        {
            typedef IntWrapper                       Ret;
            typedef ConvertibleToInt                 Arg;

            const Arg a1(0x0001);
            const Arg a2(0x0002);
            const Arg a3(0x0004);
            const Arg a4(0x0008);
            const Arg a5(0x0010);
            const Arg a6(0x0020);
            const Arg a7(0x0040);
            const Arg a8(0x0080);
            const Arg a9(0x0100);
            const Arg a10(0x0200);
            const Arg a11(0x0400);
            const Arg a12(0x0800);
            const Arg a13(0x1000);

            ASSERT((0x4000 == getInvoker<Ret>(&rep, &sum0)(&rep)));
            ASSERT((0x4001 == getInvoker<Ret, Arg>(&rep, &sum1)(&rep, a1)));
            ASSERT((0x4003 == getInvoker<Ret, Arg, Arg>(&rep, &sum2)
                    (&rep, a1, a2)));
            ASSERT((0x4007 == getInvoker<Ret, Arg, Arg, Arg>(&rep, &sum3)
                    (&rep, a1, a2, a3)));
            ASSERT((0x400f == getInvoker<Ret, Arg, Arg, Arg, Arg>(&rep, &sum4)
                    (&rep, a1, a2, a3, a4)));
            ASSERT((0x401f == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &sum5)
                    (&rep, a1, a2, a3, a4, a5)));
            ASSERT((0x403f == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &sum6)
                    (&rep, a1, a2, a3, a4, a5, a6)));
            ASSERT((0x407f == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg>
                    (&rep, &sum7)
                    (&rep, a1, a2, a3, a4, a5, a6, a7)));
            ASSERT((0x40ff == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg>
                    (&rep, &sum8)
                    (&rep, a1, a2, a3, a4, a5, a6, a7, a8)));
            ASSERT((0x41ff == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg>
                    (&rep, &sum9)
                    (&rep, a1, a2, a3, a4, a5, a6, a7, a8, a9)));
            ASSERT((0x43ff == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg>
                    (&rep, &sum10)
                    (&rep, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)));
            ASSERT((0x47ff == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &sum11)
                    (&rep, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)));
            ASSERT((0x4fff == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &sum12)
                    (&rep, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11,
                     a12)));
            ASSERT((0x5fff == getInvoker<Ret, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &sum13)
                    (&rep, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11,
                     a12, a13)));
        }

        if (veryVerbose) printf("Plan step 4: void return\n");
        // Test void return type with side effect.
        int v = 1;
        getInvoker<void, int *>(&rep, &increment)(&rep, &v);
        ASSERT(2 == v);

        // Test discarding of non-void return value
        getInvoker<void, int>(&rep, &sum1)(&rep, 9);

        if (veryVerbose) printf("Plan step 5: Pass by reference\n");
        ASSERT((&v == getInvoker<int *, int&>(&rep, &getAddress)(&rep, v)));
        ASSERT((&v == getInvoker<const int *, const int&>
                (&rep, &getConstAddress)(&rep, v)));

        if (veryVerbose) printf("Plan step 6: Pass by rvalue reference\n");
        {
            typedef bslmf::MovableRef<int> Arg;

            int a1(0x0001);
            int a2(0x0002);
            int a3(0x0004);
            int a4(0x0008);
            int a5(0x0010);
            int a6(0x0020);
            int a7(0x0040);
            int a8(0x0080);
            int a9(0x0100);
            int a10(0x0200);
            int a11(0x0400);
            int a12(0x0800);
            int a13(0x1000);

            ASSERT((0x8000 == getInvoker<int>(&rep, &rsum0)(&rep)));

            ASSERT((0x8001 == getInvoker<int, Arg>(&rep, &rsum1)
                    (&rep, bslmf::MovableRefUtil::move(a1))));
            ASSERT(a1 == -1);  a1 = 0x0001;

            ASSERT((0x8003 == getInvoker<int, Arg, Arg>(&rep, &rsum2)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;

            ASSERT((0x8007 == getInvoker<int, Arg, Arg, Arg>(&rep, &rsum3)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;

            ASSERT((0x800f == getInvoker<int, Arg, Arg, Arg, Arg>(&rep, &rsum4)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;
            ASSERT(a4 == -1);  a4 = 0x0008;

            ASSERT((0x801f == getInvoker<int, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &rsum5)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;
            ASSERT(a4 == -1);  a4 = 0x0008;
            ASSERT(a5 == -1);  a5 = 0x0010;

            ASSERT((0x803f == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &rsum6)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;
            ASSERT(a4 == -1);  a4 = 0x0008;
            ASSERT(a5 == -1);  a5 = 0x0010;
            ASSERT(a6 == -1);  a6 = 0x0020;

            ASSERT((0x807f == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg>
                    (&rep, &rsum7)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;
            ASSERT(a4 == -1);  a4 = 0x0008;
            ASSERT(a5 == -1);  a5 = 0x0010;
            ASSERT(a6 == -1);  a6 = 0x0020;
            ASSERT(a7 == -1);  a7 = 0x0040;

            ASSERT((0x80ff == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg>
                    (&rep, &rsum8)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7),
                     bslmf::MovableRefUtil::move(a8))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;
            ASSERT(a4 == -1);  a4 = 0x0008;
            ASSERT(a5 == -1);  a5 = 0x0010;
            ASSERT(a6 == -1);  a6 = 0x0020;
            ASSERT(a7 == -1);  a7 = 0x0040;
            ASSERT(a8 == -1);  a8 = 0x0080;

            ASSERT((0x81ff == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg>
                    (&rep, &rsum9)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7),
                     bslmf::MovableRefUtil::move(a8),
                     bslmf::MovableRefUtil::move(a9))));
            ASSERT(a1 == -1);  a1 = 0x0001;
            ASSERT(a2 == -1);  a2 = 0x0002;
            ASSERT(a3 == -1);  a3 = 0x0004;
            ASSERT(a4 == -1);  a4 = 0x0008;
            ASSERT(a5 == -1);  a5 = 0x0010;
            ASSERT(a6 == -1);  a6 = 0x0020;
            ASSERT(a7 == -1);  a7 = 0x0040;
            ASSERT(a8 == -1);  a8 = 0x0080;
            ASSERT(a9 == -1);  a9 = 0x0100;

            ASSERT((0x83ff == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg>
                    (&rep, &rsum10)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7),
                     bslmf::MovableRefUtil::move(a8),
                     bslmf::MovableRefUtil::move(a9),
                     bslmf::MovableRefUtil::move(a10))));
            ASSERT(a1 == -1);  a1  = 0x0001;
            ASSERT(a2 == -1);  a2  = 0x0002;
            ASSERT(a3 == -1);  a3  = 0x0004;
            ASSERT(a4 == -1);  a4  = 0x0008;
            ASSERT(a5 == -1);  a5  = 0x0010;
            ASSERT(a6 == -1);  a6  = 0x0020;
            ASSERT(a7 == -1);  a7  = 0x0040;
            ASSERT(a8 == -1);  a8  = 0x0080;
            ASSERT(a9 == -1);  a9  = 0x0100;
            ASSERT(a10 == -1); a10 = 0x0200;

            ASSERT((0x87ff == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg>
                    (&rep, &rsum11)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7),
                     bslmf::MovableRefUtil::move(a8),
                     bslmf::MovableRefUtil::move(a9),
                     bslmf::MovableRefUtil::move(a10),
                     bslmf::MovableRefUtil::move(a11))));
            ASSERT(a1 == -1);  a1  = 0x0001;
            ASSERT(a2 == -1);  a2  = 0x0002;
            ASSERT(a3 == -1);  a3  = 0x0004;
            ASSERT(a4 == -1);  a4  = 0x0008;
            ASSERT(a5 == -1);  a5  = 0x0010;
            ASSERT(a6 == -1);  a6  = 0x0020;
            ASSERT(a7 == -1);  a7  = 0x0040;
            ASSERT(a8 == -1);  a8  = 0x0080;
            ASSERT(a9 == -1);  a9  = 0x0100;
            ASSERT(a10 == -1); a10 = 0x0200;
            ASSERT(a11 == -1); a11 = 0x0400;

            ASSERT((0x8fff == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &rsum12)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7),
                     bslmf::MovableRefUtil::move(a8),
                     bslmf::MovableRefUtil::move(a9),
                     bslmf::MovableRefUtil::move(a10),
                     bslmf::MovableRefUtil::move(a11),
                     bslmf::MovableRefUtil::move(a12))));
            ASSERT(a1 == -1);  a1  = 0x0001;
            ASSERT(a2 == -1);  a2  = 0x0002;
            ASSERT(a3 == -1);  a3  = 0x0004;
            ASSERT(a4 == -1);  a4  = 0x0008;
            ASSERT(a5 == -1);  a5  = 0x0010;
            ASSERT(a6 == -1);  a6  = 0x0020;
            ASSERT(a7 == -1);  a7  = 0x0040;
            ASSERT(a8 == -1);  a8  = 0x0080;
            ASSERT(a9 == -1);  a9  = 0x0100;
            ASSERT(a10 == -1); a10 = 0x0200;
            ASSERT(a11 == -1); a11 = 0x0400;
            ASSERT(a12 == -1); a12 = 0x0800;

            ASSERT((0x9fff == getInvoker<int, Arg, Arg, Arg, Arg, Arg, Arg,
                                         Arg, Arg, Arg, Arg, Arg, Arg, Arg>
                    (&rep, &rsum13)
                    (&rep,
                     bslmf::MovableRefUtil::move(a1),
                     bslmf::MovableRefUtil::move(a2),
                     bslmf::MovableRefUtil::move(a3),
                     bslmf::MovableRefUtil::move(a4),
                     bslmf::MovableRefUtil::move(a5),
                     bslmf::MovableRefUtil::move(a6),
                     bslmf::MovableRefUtil::move(a7),
                     bslmf::MovableRefUtil::move(a8),
                     bslmf::MovableRefUtil::move(a9),
                     bslmf::MovableRefUtil::move(a10),
                     bslmf::MovableRefUtil::move(a11),
                     bslmf::MovableRefUtil::move(a12),
                     bslmf::MovableRefUtil::move(a13))));
            ASSERT(a1 == -1);  a1  = 0x0001;
            ASSERT(a2 == -1);  a2  = 0x0002;
            ASSERT(a3 == -1);  a3  = 0x0004;
            ASSERT(a4 == -1);  a4  = 0x0008;
            ASSERT(a5 == -1);  a5  = 0x0010;
            ASSERT(a6 == -1);  a6  = 0x0020;
            ASSERT(a7 == -1);  a7  = 0x0040;
            ASSERT(a8 == -1);  a8  = 0x0080;
            ASSERT(a9 == -1);  a9  = 0x0100;
            ASSERT(a10 == -1); a10 = 0x0200;
            ASSERT(a11 == -1); a11 = 0x0400;
            ASSERT(a12 == -1); a12 = 0x0800;
            ASSERT(a13 == -1); a13 = 0x1000;
        }

        if (veryVerbose) printf("Plan step 7: NothrowMovableWrapper\n");

        ASSERT(Util::invokerForFunc<int(int)>(&sum1) ==
               Util::invokerForFunc<int(int)>(NTWRAP(&sum1)));
        ASSERT((getInvoker<int, int>(&rep, &sum1)(&rep, 99) ==
                getInvoker<int, int>(&rep, NTWRAP(&sum1))(&rep, 99)));

        ASSERT(Util::invokerForFunc<void(int *)>(&increment) ==
               Util::invokerForFunc<void(int *)>(NTWRAP(&increment)));

        typedef IntWrapper       Ret;
        typedef ConvertibleToInt Arg;

        ASSERT(Util::invokerForFunc<Ret(Arg, Arg)>(&sum2) ==
               Util::invokerForFunc<Ret(Arg, Arg)>(NTWRAP(&sum2)));
      } break;

      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST
        //
        // Concerns:
        // 1. The this component is sufficiently functional to enable
        //    comprehensive testing in subsequent test cases.
        //
        // Plan:
        // 1. Call `bslstl::Function_InvokerUtil::invokerForFunc` with a
        //    `nullptr` argument.  Verify that it returns null function
        //    pointer.
        //
        // 2. Create `bslstl::Function_Rep` object and install a
        //    pointer-to-function as its target.  Verify that
        //    `bslstl::Function_InvokerUtil::invokerForFunc` returns a pointer
        //    to function that, when cast to the appropriate type, can be
        //    called to invoke the target function.
        //
        // Testing:
        //  BREATHING TEST
        // --------------------------------------------------------------------

        if (verbose) printf("\nBREATHING TEST"
                            "\n==============\n");

        ASSERT(0 == Util::invokerForFunc<int(void *, char)>(bsl::nullptr_t()));

        int (*pf)(int, int) = &sum2;
        Util::GenericInvoker *ginv_p = Util::invokerForFunc<int(int, int)>(pf);
        ASSERT(0 != ginv_p);
        Rep rep(&ta);
        rep.installFunc(pf, ginv_p);
        int (*inv_p)(Rep*, int, int) =
            reinterpret_cast<int (*)(Rep*, int, int)>(ginv_p);
        ASSERT(0x4003 == inv_p(&rep, 1, 2));

      } break;

      default: {
        fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        fprintf(stderr, "Error, non-zero test status = %d.\n", testStatus);
    }

    return testStatus;
}

#endif // End C++11 code

// ----------------------------------------------------------------------------
// Copyright 2020 Bloomberg Finance L.P.
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
