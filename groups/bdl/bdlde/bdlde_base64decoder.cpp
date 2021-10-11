// bdlde_base64decoder.cpp                                            -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#include <bdlde_base64decoder.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlde_base64decoder_cpp,"$Id$ $CSID$")

#include <bdlde_base64encoder.h>  // for testing only

#include <bsls_assert.h>

namespace BloombergLP {

                // ======================
                // FILE-SCOPE STATIC DATA
                // ======================

// The following table identifies characters that can be ignored when
// d_isUnrecognizedAnErrorFlag 'true'.

static const bool charsThatCanBeIgnoredInStrictMode[256] = {
    // 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
       0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,  // 00  // whitespace
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 10
       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 20  // <space> char
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 30
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 50
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 60
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 70
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 80
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 90
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A0
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B0
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C0
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D0
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E0
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // F0
};

// The following table identifies characters that can be ignored in Basic64
// alphabet when d_isUnrecognizedAnErrorFlag 'false'.

static const bool basicCharsThatCanBeIgnoredInRelaxedMode[256] = {
    // 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 00
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 10
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0,  // 20  // '+', '/'
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1,  // 30  // '0'..'9', '='
       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40  // uppercase
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,  // 50  //      alphabet
       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 60  // lowercase
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,  // 70  //      alphabet
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 80
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 90
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // A0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // B0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // C0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // D0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // E0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // F0
};

// The following table identifies characters that can be ignored in Url and
// Filename Safe alphabet when d_isUnrecognizedAnErrorFlag 'false'.

static const bool urlCharsThatCanBeIgnoredInRelaxedMode[256] = {
    // 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 00
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 10
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,  // 20  // '-'
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1,  // 30  // '0'..'9', '='
       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40  // uppercase
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,  // 50  // alphabet, '_'
       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 60  // lowercase
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,  // 70  //      alphabet
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 80
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 90
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // A0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // B0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // C0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // D0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // E0
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // F0
};

// The following table is a map from numeric Base64 encoding characters to the
// corresponding 6-bit index.

static const char ff = static_cast<char>(-1);
static const char basicAlphabet[256] = {
    //  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 00
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 10
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, 62, ff, ff, ff, 63,  // 20
       52, 53, 54, 55, 56, 57, 58, 59, 60, 61, ff, ff, ff, ff, ff, ff,  // 30
       ff,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  // 40
       15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, ff, ff, ff, ff, ff,  // 50
       ff, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  // 60
       41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, ff, ff, ff, ff, ff,  // 70
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 80
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 90
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // A0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // B0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // C0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // D0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // E0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // F0
};

// The following table is a map from numeric Url and Filename Safe encoding
// characters to the corresponding 6-bit index.

static const char urlAlphabet[256] = {
    //  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
    // --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 00
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 10
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, 62, ff, ff,  // 20
       52, 53, 54, 55, 56, 57, 58, 59, 60, 61, ff, ff, ff, ff, ff, ff,  // 30
       ff,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,  // 40
       15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, ff, ff, ff, ff, 63,  // 50
       ff, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  // 60
       41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, ff, ff, ff, ff, ff,  // 70
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 80
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // 90
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // A0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // B0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // C0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // D0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // E0
       ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff, ff,  // F0
};

namespace bdlde {

                         // -------------------
                         // class Base64Decoder
                         // -------------------

// CLASS DATA
const bool *const Base64Decoder::s_ignorableStrict_p =
                                             charsThatCanBeIgnoredInStrictMode;
const bool *const Base64Decoder::s_basicIgnorableRelaxed_p =
                                       basicCharsThatCanBeIgnoredInRelaxedMode;
const bool *const Base64Decoder::s_urlIgnorableRelaxed_p =
                                         urlCharsThatCanBeIgnoredInRelaxedMode;
const char *const Base64Decoder::s_basicAlphabet_p = basicAlphabet;
const char *const Base64Decoder::s_urlAlphabet_p   = urlAlphabet;


// CREATORS

Base64Decoder::~Base64Decoder()
{
    BSLS_ASSERT(e_ERROR_STATE <= d_state);
    BSLS_ASSERT(d_state <= e_DONE_STATE);
    BSLS_ASSERT(0 <= d_outputLength);
}

}  // close package namespace
}  // close enterprise namespace

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
