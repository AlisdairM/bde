// bdls_pipeutil.t.cpp                                                -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#include <bdls_pipeutil.h>

#include <bslim_testutil.h>

#include <bsls_platform.h>
#include <bdls_pathutil.h>
#include <bdls_filesystemutil.h>

#include <bsl_algorithm.h>
#include <bsl_cstdlib.h>
#include <bsl_c_stdlib.h>
#include <bsl_iostream.h>

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

// ============================================================================
//                     STANDARD BDE ASSERT TEST FUNCTION
// ----------------------------------------------------------------------------

namespace {

int testStatus = 0;

void aSsErT(bool condition, const char *message, int line)
{
    if (condition) {
        cout << "Error " __FILE__ "(" << line << "): " << message
             << "    (failed)" << endl;

        if (0 <= testStatus && testStatus <= 100) {
            ++testStatus;
        }
    }
}

}  // close unnamed namespace

// ============================================================================
//               STANDARD BDE TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLIM_TESTUTIL_ASSERT
#define ASSERTV      BSLIM_TESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLIM_TESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLIM_TESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLIM_TESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLIM_TESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLIM_TESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLIM_TESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLIM_TESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLIM_TESTUTIL_LOOP6_ASSERT

#define Q            BSLIM_TESTUTIL_Q   // Quote identifier literally.
#define P            BSLIM_TESTUTIL_P   // Print identifier and value.
#define P_           BSLIM_TESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLIM_TESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLIM_TESTUTIL_L_  // current Line number

template <class STRING_TYPE>
void testCase1_makeCanonicalName(const STRING_TYPE& typeName,
                                 int                test,
                                 int                verbose)
{
    (void) verbose; (void) test;

    if (verbose) cout << "\n\t+++++++++++++++ Testing " << typeName << endl;

#ifdef BSLS_PLATFORM_OS_WINDOWS
    STRING_TYPE compare("\\\\.\\pipe\\foo.bar");
    STRING_TYPE name;
    ASSERT(0 == bdls::PipeUtil::makeCanonicalName(&name, "FOO.Bar"));
    LOOP2_ASSERT(name, compare, name == compare);
    if (verbose) { P(name) }
#else
    STRING_TYPE compare;
    const char  *tmpdirPtr;
    if (0 != (tmpdirPtr = bsl::getenv("SOCKDIR"))) {
        compare = tmpdirPtr;
    }
    else if (0 != (tmpdirPtr = bsl::getenv("TMPDIR"))) {
        compare = tmpdirPtr;
    }
    else {
        bdls::FilesystemUtil::getWorkingDirectory(&compare);
    }

    ASSERT(0 == bdls::PathUtil::appendIfValid(&compare, "foo.bar"));

    STRING_TYPE name;
    ASSERT(0 == bdls::PipeUtil::makeCanonicalName(&name, "FOO.Bar"));
    LOOP2_ASSERT(name, compare, name == compare);
    if (verbose) { P(name) }

    if (0 == bsl::getenv("SOCKDIR")) {
        // 'SOCKDIR' is not set.  Set it and retest.
        static char sockdir[] = "SOCKDIR=bozonono";
        ::putenv(sockdir);
        compare = bsl::getenv("SOCKDIR");
        ASSERT(0 == bdls::PathUtil::appendIfValid(&compare, "xy.ab"));
        ASSERT(0 == bdls::PipeUtil::makeCanonicalName(&name, "XY.Ab"));
        LOOP2_ASSERT(name, compare, name == compare);
        if (verbose) { P(name) }
    }
    else if (0 != bsl::getenv("TMPDIR")) {
        // Both 'SOCKDIR' and 'TMPDIR' are set.  If 'SOCKDIR' can be
        // removed, retest 'TMPDIR'.
        static char sockdir[] = "SOCKDIR";
        ::putenv(sockdir);
        if (0 == bsl::getenv("SOCKDIR")) {
            compare = bsl::getenv("TMPDIR");
            ASSERT(0 == bdls::PathUtil::appendIfValid(&compare, "xy.ab"));
            ASSERT(0 == bdls::PipeUtil::makeCanonicalName(&name, "XY.Ab"));
            LOOP2_ASSERT(name, compare, name == compare);
            if (verbose) { P(name) }
        }
    }
#endif
}

int main(int argc, char *argv[]) {
    int test = argc > 1 ? bsl::atoi(argv[1]) : 0;
    bool             verbose = argc > 2;
//  bool         veryVerbose = argc > 3;
//  bool     veryVeryVerbose = argc > 4;
//  bool veryVeryVeryVerbose = argc > 5;

    switch(test) { case 0:
      case 1: {
        ///////////////////////////////////////////////////////////////////////
        // makeCanonicalName test
        //
        // makeCanonicalName() is, well, canonical.  Here we re-implement its
        // canonical behavior and ensure that the function behaves that way.
        ///////////////////////////////////////////////////////////////////////

        if (verbose) {
           cout << "makeCanonicalName test" << endl;
        }

        testCase1_makeCanonicalName<bsl::string>(
            "bsl::string", test, verbose);
        testCase1_makeCanonicalName<std::string>(
            "std::string", test, verbose);
#ifdef BSLS_LIBRARYFEATURES_HAS_CPP17_PMR
        testCase1_makeCanonicalName<std::pmr::string>(
            "std::pmr::string", test, verbose);
#endif
      } break;
      default: {
        cerr << "WARNING: CASE `" << test << "' NOT FOUND." << endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        cerr << "Error, non-zero test status = " << testStatus << "."
             << endl;
    }
    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
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
