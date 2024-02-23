// bslmf_conjunction.t.cpp                                            -*-C++-*-

#include <bslmf_conjunction.h>
#include <bslmf_integralconstant.h>

#include <bsls_bsltestutil.h>

#include <stdio.h>
#include <stdlib.h>

using namespace BloombergLP;

//=============================================================================
//                                TEST PLAN
//-----------------------------------------------------------------------------
// [ 2] bsl::conjunction
// [ 2] bsl::conjunction_v
// [ 1] BREATHING TEST

// ============================================================================
//                     STANDARD BSL ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", line, message);

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

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

//=============================================================================
//                    GLOBAL TEST TYPES AND FUNCTIONS
//-----------------------------------------------------------------------------

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int                 test = argc > 1 ? atoi(argv[1]) : 0;
    bool             verbose = argc > 2;
    bool         veryVerbose = argc > 3;
    bool     veryVeryVerbose = argc > 4;
    bool veryVeryVeryVerbose = argc > 5;

    (void)veryVerbose;          // suppress warning
    (void)veryVeryVerbose;      // suppress warning
    (void)veryVeryVeryVerbose;  // suppress warning

    setbuf(stdout, NULL);       // Use unbuffered output

    printf("TEST " __FILE__ " CASE %d\n", test);

    switch (test) { case 0:  // Zero is always the leading case.
      case 2: {
        // --------------------------------------------------------------------
        // TESTING CLASS TEMPLATE 'bsl::conjunction'
        //
        // Concerns:
        //:  1 'conjunction<...>::value' is 'false' iff one of the arguments is
        //:    'Arg::value == false'.
        //:
        //:  2 'conjunction_v<...>' has the same value as
        //:    'conjunction<...>::value'.
        //
        // Plan:
        //:  1 Verify the result for all possible arguments permutations for 0,
        //:    1, 2, and 3 arguments.
        //:
        //:  2 Repeat the same tests with 'conjunction_v'.
        //
        // Testing:
        //   bsl::conjunction
        //   bsl::conjunction_v
        // --------------------------------------------------------------------

        if (verbose) printf("\nTESTING CLASS TEMPLATE 'bsl::conjunction'"
                            "\n=========================================\n");

        using bsl::conjunction;
        typedef bsl::false_type F;
        typedef bsl::true_type  T;

        ASSERT(( conjunction<>::value));

        ASSERT((!conjunction<F>::value));
        ASSERT(( conjunction<T>::value));

        ASSERT((!conjunction<F, F>::value));
        ASSERT((!conjunction<F, T>::value));
        ASSERT((!conjunction<T, F>::value));
        ASSERT(( conjunction<T, T>::value));

        ASSERT((!conjunction<F, F, F>::value));
        ASSERT((!conjunction<F, F, T>::value));
        ASSERT((!conjunction<F, T, F>::value));
        ASSERT((!conjunction<F, T, T>::value));
        ASSERT((!conjunction<T, F, F>::value));
        ASSERT((!conjunction<T, F, T>::value));
        ASSERT((!conjunction<T, T, F>::value));
        ASSERT(( conjunction<T, T, T>::value));

#ifdef BSLS_COMPILERFEATURES_SUPPORT_VARIABLE_TEMPLATES
        using bsl::conjunction_v;

        ASSERT(( conjunction_v<>));

        ASSERT((!conjunction_v<F>));
        ASSERT(( conjunction_v<T>));

        ASSERT((!conjunction_v<F, F>));
        ASSERT((!conjunction_v<F, T>));
        ASSERT((!conjunction_v<T, F>));
        ASSERT(( conjunction_v<T, T>));

        ASSERT((!conjunction_v<F, F, F>));
        ASSERT((!conjunction_v<F, F, T>));
        ASSERT((!conjunction_v<F, T, F>));
        ASSERT((!conjunction_v<F, T, T>));
        ASSERT((!conjunction_v<T, F, F>));
        ASSERT((!conjunction_v<T, F, T>));
        ASSERT((!conjunction_v<T, T, F>));
        ASSERT(( conjunction_v<T, T, T>));
#endif
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // BREATHING TEST
        //
        // Concerns:
        //: 1 The provided utilities are sufficiently functional.
        //
        // Plan:
        //: 1 Write any code with 'bsl::conjunction' here.
        //
        // Testing:
        //   BREATHING TEST
        // --------------------------------------------------------------------
        if (verbose) printf("\nBREATHING TEST"
                            "\n==============\n");
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

// ----------------------------------------------------------------------------
// Copyright 2024 Bloomberg Finance L.P.
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