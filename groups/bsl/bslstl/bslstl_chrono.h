// bslstl_chrono.h                                                    -*-C++-*-
#ifndef INCLUDED_BSLSTL_CHRONO
#define INCLUDED_BSLSTL_CHRONO

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide functionality of the corresponding C++ Standard header.
//
//@CANONICAL_HEADER: bsl_chrono.h
//
//@DESCRIPTION: This component is for internal use only. Please include
// '<bsl_chrono.h>' directly.  This component imports symbols declared in the
// <chrono> header file implemented in the standard library provided by the
// compiler vendor.
//
///User-defined literals
///---------------------
// This component provides a set of user-defined literals (UDL) to form
// 'bsl::chrono::duration' objects with various duration periods such as hours,
// minutes, seconds, milliseconds, microseconds and nanoseconds.  The
// ud-suffixes are preceded with the '_' symbol to distinguish between the
// 'bsl'-chrono's UDLs and the 'std'-chrono's UDLs introduced in the C++14
// standard and implemented in the standard library.  Note that 'bsl'-chrono's
// UDLs, unlike the 'std'-chrono's UDLs, can be used in a client's code if the
// current compiler supports the C++11 standard.
//
// Also note that 'bsl'-chrono's UDL operators are declared in the
// 'bsl::literals::chrono_literals' namespace, where 'literals' and
// 'chrono_literals' are inline namespaces.  Access to these operators can be
// gained with either 'using namespace bsl::literals',
// 'using namespace bsl::chrono_literals' or
// 'using namespace bsl::literals::chrono_literals'.  But we recommend
// 'using namespace bsl::chrono_literals' to minimize the scope of the using
// declaration.
//
///Usage
///-----
// In this section we show intended use of this component.
//
///Example 1: Basic 'bsl'-chrono's UDLs Usage
/// - - - - - - - - - - - - - - - - - - - - -
// This example demonstrates basic use of user-defined literal operators.
//
// First, we provide an access to 'bsl'-chrono's UDLs.
//..
//  using namespace bsl::chrono_literals;
//..
// Then, we construct two duration objects that represent a 24-hours and a half
// an hour time intervals using 'operator "" _h'.
//..
//  auto hours_in_a_day = 24_h;
//  auto halfhour       = 0.5_h;
//..
// Finally, stream the two objects to 'stdout':
//..
//  printf("one day is %ld hours\n", hours_in_a_day.count());
//  printf("half an hour is %.1f hours\n",
//         static_cast<double>(halfhour.count()));
//..
// The streaming operator produces output in the following format on 'stdout':
//..
//  one day is 24 hours
//  half an hour is 0.5 hours
//..

#include <bslscm_version.h>

#include <bslstl_ratio.h>

#include <bsls_compilerfeatures.h>
#include <bsls_keyword.h>
#include <bsls_libraryfeatures.h>
#include <bsls_nativestd.h>

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY

#include <chrono>

namespace bsl {

    namespace chrono {
        using native_std::chrono::duration;
        using native_std::chrono::time_point;
        using native_std::chrono::system_clock;
        using native_std::chrono::steady_clock;
        using native_std::chrono::high_resolution_clock;
        using native_std::chrono::treat_as_floating_point;
        using native_std::chrono::duration_values;
        using native_std::chrono::duration_cast;
        using native_std::chrono::time_point_cast;
        using native_std::chrono::hours;
        using native_std::chrono::minutes;
        using native_std::chrono::seconds;
        using native_std::chrono::milliseconds;
        using native_std::chrono::microseconds;
        using native_std::chrono::nanoseconds;

#if defined(BSLS_LIBRARYFEATURES_HAS_CPP14_BASELINE_LIBRARY)
        template <class TYPE>
        constexpr bool treat_as_floating_point_v =
                      native_std::chrono::treat_as_floating_point<TYPE>::value;
            // This template variable represents the result value of the
            // 'native_std::chrono::treat_as_floating_point' meta-function.
#endif

#if defined(BSLS_LIBRARYFEATURES_HAS_CPP17_BASELINE_LIBRARY)
        using native_std::chrono::abs;
        using native_std::chrono::ceil;
        using native_std::chrono::floor;
        using native_std::chrono::round;
#endif
    }  // close namespace chrono

#ifdef BSLS_LIBRARYFEATURES_HAS_CPP14_BASELINE_LIBRARY
inline namespace literals {
inline namespace chrono_literals {
    using namespace native_std::literals::chrono_literals;
}  // close chrono_literals namespace
}  // close literals namespace
#endif

#if defined (BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY) && \
    defined (BSLS_COMPILERFEATURES_SUPPORT_INLINE_NAMESPACE)

inline namespace literals {
inline namespace chrono_literals {

    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::hours operator "" _h(unsigned long long hrs);
    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::ratio<3600, 1>> operator "" _h(
                                                              long double hrs);
       // Create a 'bsl::chrono::duration' object having the
       // 'bsl::ratio<3600, 1>' duration period and initialized with the
       // specified 'hrs' number of hours.

    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::minutes operator "" _min(unsigned long long mins);
    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::ratio<60, 1>> operator "" _min(
                                                             long double mins);
       // Create a 'bsl::chrono::duration' object having the
       // 'bsl::ratio<60, 1>' duration period and initialized with the
       // specified 'mins' number of minutes.

    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::seconds operator "" _s(unsigned long long secs);
    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double> operator "" _s(long double secs);
        // Create a 'bsl::chrono::duration' object having the 'bsl::ratio<1>'
        // duration period and initialized with the specified 'secs' number of
        // seconds.

    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::milliseconds operator "" _ms(unsigned long long ms);
    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::milli> operator "" _ms(
                                                               long double ms);
       // Create a 'bsl::chrono::duration' object having the
       // 'bsl::milli' duration period and initialized with the specified 'ms'
       // number of milliseconds.

    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::microseconds operator "" _us(unsigned long long us);
    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::micro> operator "" _us(
                                                               long double us);
       // Create a 'bsl::chrono::duration' object having the
       // 'bsl::micro' duration period and initialized with the specified 'us'
       // number of microseconds.

    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::nanoseconds operator "" _ns(unsigned long long ns);
    BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::nano> operator "" _ns(
                                                               long double ns);
       // Create a 'bsl::chrono::duration' object having the 'bsl::nano'
       // duration period and initialized with the specified 'ns' number of
       // microseconds.

}  // close chrono_literals namespace
}  // close literals namespace
#endif

// ============================================================================
//                   INLINE AND TEMPLATE FUNCTION DEFINITIONS
// ============================================================================

#if defined (BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY) && \
    defined (BSLS_COMPILERFEATURES_SUPPORT_INLINE_NAMESPACE)

inline namespace literals {
inline namespace chrono_literals {
    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::hours operator "" _h(unsigned long long hrs)
    {
        return bsl::chrono::hours(hrs);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::ratio<3600, 1>> operator "" _h(
                                                               long double hrs)
    {
        return bsl::chrono::duration<long double, bsl::ratio<3600, 1>>(hrs);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::minutes operator "" _min(unsigned long long mins)
    {
        return bsl::chrono::minutes(mins);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::ratio<60, 1>>
        operator "" _min(long double mins)
    {
        return bsl::chrono::duration<long double, bsl::ratio<60, 1>>(mins);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::seconds operator "" _s(unsigned long long secs)
    {
        return bsl::chrono::seconds(secs);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double> operator "" _s(long double secs)
    {
        return bsl::chrono::duration<long double>(secs);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::milliseconds operator "" _ms(unsigned long long ms)
    {
        return bsl::chrono::milliseconds(ms);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::milli> operator "" _ms(
                                                                long double ms)
    {
        return bsl::chrono::duration<long double, bsl::milli>(ms);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::microseconds operator "" _us(unsigned long long us)
    {
        return bsl::chrono::microseconds(us);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::micro> operator "" _us(
                                                                long double us)
    {
        return bsl::chrono::duration<long double, bsl::micro>(us);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::nanoseconds operator "" _ns(unsigned long long ns)
    {
        return bsl::chrono::nanoseconds(ns);
    }

    inline BSLS_KEYWORD_CONSTEXPR
    bsl::chrono::duration<long double, bsl::nano> operator "" _ns(
                                                                long double ns)
    {
        return bsl::chrono::duration<long double, bsl::nano>(ns);
    }

}  // close chrono_literals namespace
}  // close literals namespace
#endif
}  // close package namespace

#endif  // BSLS_LIBRARYFEATURES_HAS_CPP11_BASELINE_LIBRARY
#endif  // INCLUDED_BSLSTL_CHRONO

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
