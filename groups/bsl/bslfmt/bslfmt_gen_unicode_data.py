#!/usr/bin/env python3.9
#
# Script to generate
#
# This script requires at least version 3.9 of python
#
# To update the unicode static data download the latest unicode files:
# https://www.unicode.org/Public/UNIDATA/EastAsianWidth.txt
# https://www.unicode.org/Public/UNIDATA/DerivedCoreProperties.txt
# https://www.unicode.org/Public/UNIDATA/auxiliary/GraphemeBreakProperty.txt
# https://www.unicode.org/Public/UNIDATA/extracted/DerivedGeneralCategory.txt
# https://www.unicode.org/Public/UNIDATA/emoji/emoji-data.txt
#
# Then run this script and save the output to bslfmt_formatterunicodedata.cpp

import re
import os
import sys

from io import StringIO
from pathlib import Path
from dataclasses import dataclass
from typing import Optional

FILENAME = os.path.basename(__file__)

RANGE_DATA_TEMPLATE = """
{filename}
{timestamp}
const FormatterUnicodeData::{prop_name}
    FormatterUnicodeData::{var_name}[] = {{
        {values}
}};
"""

COUNT_TEMPLATE = """
const int FormatterUnicodeData::{var_name} = 
    sizeof({source_name}) / sizeof({prop_name});
"""

CATEGORY_DATA_TEMPLATE = """
{filename}
{timestamp}
const FormatterUnicodeData::{prop_name}
    FormatterUnicodeData::{var_name}[] = {{
        {values}
}};
"""


OUTER_TEMPLATE = """
// bslfmt_formatterunicodedata.cpp                                    -*-C++-*-
#include <bslfmt_formatterunicodedata.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bslfmt_formatterunicodedata_cpp, "$Id$ $CSID$")

namespace BloombergLP {{
namespace bslfmt {{

// ============================================================================
//                     MACHINE-GENERATED DATA GOES HERE
// ============================================================================

// WARNING, this file is generated by {FILENAME}

// DO NOT MODIFY!

// The following static data tables are generated from the Unicode character
// database.
//
// s_graphemeBreakCategoryRanges comes from
//                                     ucd/auxiliary/GraphemeBreakProperty.txt.
//
// s_extendedPictographicRanges comes from ucd/emoji/emoji-data.txt.
//
// s_printableRanges comes from ucd/extracted/DerivedGeneralCategory.txt.
//
// s_graphemeExtendRanges comes from ucd/DerivedCoreProperties.txt.
//
// s_doubleFieldWidthRanges comes from ucd/EastAsianWidth.txt.
//
// Codepoint ranges may not overlap, and, within one property, a codepoint may
// only appear once.  Furthermore, the codepoint lower bounds appear in sorted
// (ascending) order.

{content}

}}  // close package namespace
}}  // close enterprise namespace

// ----------------------------------------------------------------------------
//
// The data herein is generated from Unicode Data Files which are
// Copyright 1991-2024 Unicode, Inc. and subject to the
// following license agreement:
//
// UNICODE, INC. LICENSE AGREEMENT V3 - DATA FILES AND SOFTWARE
//
// See Terms of Use <https://www.unicode.org/copyright.html>
// for definitions of Unicode Inc.'s Data Files and Software.
//
// Disclaimer
//
// The Unicode Character Database is provided as is by Unicode, Inc. No
// claims are made as to fitness for any particular purpose. No warranties
// of any kind are expressed or implied. The recipient agrees to determine
// applicability of information provided. If this file has been purchased
// on magnetic or optical media from Unicode, Inc., the sole remedy for any
// claim will be exchange of defective media within 90 days of receipt.
//
// This disclaimer is applicable for all other data files accompanying the
// Unicode Character Database, some of which have been compiled by the
// Unicode Consortium, and some of which have been supplied by other sources.
//
// Limitations on Rights to Redistribute This Data
//
// Recipient is granted the right to make copies in any form for internal
// distribution and to freely use the information supplied in the creation
// of products supporting the UnicodeTM Standard. The files in the Unicode
// Character Database can be redistributed to third parties or other
// organizations (whether for profit or not) as long as this notice and the
// disclaimer notice are retained. Information can be extracted from these
// files and used in documentation or programs, as long as there is an
// accompanying notice indicating the source.
//
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

"""

@dataclass
class PropertyRange:
    lower: int = -1
    upper: int = -1
    prop: str = None


LINE_REGEX = re.compile(
    r"^(?P<lower>[0-9A-F]{4,6})"
    r"(?:\.\.(?P<upper>[0-9A-F]{4,6}))"
    r"?\s*;\s*"
    r"(?P<prop>\w+)")

TYPE_MAP = {
    "Prepend":            "e_PREPEND",
    "CR":                 "e_CR",
    "LF":                 "e_LF",
    "Control":            "e_CONTROL",
    "Extend":             "e_EXTEND",
    "Regional_Indicator": "e_REGIONAL_INDICATOR",
    "SpacingMark":        "e_SPACING_MARK",
    "L":                  "e_HANGUL_L",
    "V":                  "e_HANGUL_V",
    "T":                  "e_HANGUL_T",
    "LV":                 "e_HANGUL_LV",
    "LVT":                "e_HANGUL_LVT",
    "ZWJ":                "e_ZERO_WIDTH_JOINER"
}

def parse_property_line(inputLine: str) -> Optional[PropertyRange]:
    if (inputLine.startswith('#') or len(inputLine.strip()) == 0):
        return None
    result = PropertyRange()
    if m := LINE_REGEX.match(inputLine):
        lower_str, upper_str, result.prop = m.group("lower", "upper", "prop")
        result.lower = int(lower_str, base=16)
        result.upper = result.lower
        if upper_str is not None:
            result.upper = int(upper_str, base=16)
        return result
    else:
        print(len(inputLine))
        print(inputLine)
        assert False


def compact_property_ranges(input: list[PropertyRange]) -> list[PropertyRange]:
    """
    Merges consecutive ranges with the same property to one range.

    Merging the ranges results in fewer ranges in the output table,
    reducing binary size and improving lookup performance.
    """
    result = list()
    for x in input:
        if (
            len(result)
            and result[-1].prop == x.prop
            and result[-1].upper + 1 == x.lower
        ):
            result[-1].upper = x.upper
            continue
        result.append(x)
    return result


def generate_cpp_data(filename: str,
                      timestamp: str,
                      prop_name: str,
                      var_name: str,
                      ranges: list[PropertyRange]) -> str:
    result = StringIO(newline='\n')
    prop_values = sorted(set(x.prop for x in ranges))
    is_binary_property = len(prop_values) == 1
    sorted_ranges = sorted(ranges, key=lambda x: x.lower)
    if (is_binary_property):
        result.write(RANGE_DATA_TEMPLATE.lstrip().format(
                    filename=filename,
                    timestamp=timestamp,
                    prop_name=prop_name,
                    var_name=var_name,
                    values=",\n        ".join(
                        ["{{{:#8x},{:#8x}}}".format(x.lower, x.upper)
                             for x in sorted_ranges])))
    else:
        result.write(CATEGORY_DATA_TEMPLATE.lstrip().format(
                    filename=filename,
                    timestamp=timestamp,
                    prop_name=prop_name,
                    var_name=var_name,
                    values=",\n        ".join(
                        ["{{{:#8x},{:#8x}, {:s}}}".format(x.lower,
                                                          x.upper,
                                                          TYPE_MAP[x.prop])
                             for x in sorted_ranges])))
    return result.getvalue()

def generate_cpp_count(prop_name: str,
                       source_name: str,
                       var_name: str) -> str:
    result = StringIO(newline='\n')
    result.write(COUNT_TEMPLATE.lstrip().format(
                prop_name=prop_name,
                source_name=source_name,
                var_name=var_name))
    return result.getvalue()

def read_file(filename: str) -> list[PropertyRange]:
    data_path = Path(__file__).absolute().with_name(filename)
    with data_path.open(encoding='utf-8') as f:
        filename = f.readline().replace("#", "//").rstrip()
        timestamp = f.readline().replace("#", "//").rstrip()
        ranges = compact_property_ranges(
            [x for line in f if (x := parse_property_line(line))])
        return filename, timestamp, ranges


def generate_data_tables() -> str:
    """
    Generate Unicode data for inclusion into <format> from
    GraphemeBreakProperty.txt, emoji-data.txt, DerivedGeneralCategory.txt, DerivedCoreProperties.txt,
    and EastAsianWidth.txt.

    GraphemeBreakProperty.txt can be found at
    https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/GraphemeBreakProperty.txt

    emoji-data.txt can be found at
    https://www.unicode.org/Public/UCD/latest/ucd/emoji/emoji-data.txt

    DerivedGeneralCategory.txt can be found at
    https://www.unicode.org/Public/UCD/latest/ucd/extracted/DerivedGeneralCategory.txt

    DerivedCoreProperties.txt can be found at
    https://www.unicode.org/Public/UCD/latest/ucd/DerivedCoreProperties.txt

    EastAsianWidth.txt can be found at
    https://www.unicode.org/Public/UCD/latest/ucd/EastAsianWidth.txt

    All files are expected to be in the same directory as this script.
    """
    gbp_filename, gbp_timestamp, gbp_ranges = read_file("GraphemeBreakProperty.txt")
    emoji_filename, emoji_timestamp, emoji_ranges = read_file("emoji-data.txt")
    cat_filename, cat_timestamp, cat_ranges = read_file("DerivedGeneralCategory.txt")
    derived_filename, derived_timestamp, derived_ranges = read_file("DerivedCoreProperties.txt")
    eaw_filename, eaw_timestamp, eaw_ranges = read_file("EastAsianWidth.txt")

    printable_ranges = compact_property_ranges(sorted([
        PropertyRange(x.lower, x.upper, "Yes")
        for x in cat_ranges
        if x.prop not in ('Cc', 'Cf', 'Cs', 'Co', 'Cn', 'Zl', 'Zp', 'Zs') or chr(x.lower) == ' '
    ], key=lambda x: x.lower))

    # N4971 [format.string.std]/13
    std_wide_ranges = [
        range(0x4DC0, 0x4DFF),
        range(0x1F300, 0x1F5FF),
        range(0x1F900, 0x1F9FF),
    ]

    def has_width_2(prop_range):
        if prop_range.prop in ("F", "W"):
            return True

        for std_wide_range in std_wide_ranges:
            if prop_range.lower in std_wide_range:
                # Either prop_range is contained entirely within std_wide_range
                assert prop_range.upper <= std_wide_range.stop

                return True
            else:
                # Or they must be completely disjoint
                assert prop_range.upper not in std_wide_range
                assert not(prop_range.lower <= std_wide_range.start and
                           prop_range.upper >= std_wide_range.stop)

        return False

    width_2_ranges = compact_property_ranges(sorted([
        PropertyRange(x.lower, x.upper, "Yes") for x in eaw_ranges if has_width_2(x)
    ], key=lambda x: x.lower))

    gpb_cpp_data = generate_cpp_data(gbp_filename,
                                     gbp_timestamp,
                                     "GraphemeBreakCategoryRange",
                                     "s_graphemeBreakCategoryRanges",
                                     gbp_ranges)

    gpb_cpp_count = generate_cpp_count("GraphemeBreakCategoryRange",
                                       "s_graphemeBreakCategoryRanges",
                                       "s_graphemeBreakCategoryRangeCount")

    emoji_cpp_data = generate_cpp_data(emoji_filename, emoji_timestamp,
                                       "BooleanRange",
                                       "s_extendedPictographicRanges", [
        x for x in emoji_ranges if x.prop == "Extended_Pictographic"])

    emoji_cpp_count = generate_cpp_count("BooleanRange",
                                         "s_extendedPictographicRanges",
                                         "s_extendedPictographicRangeCount")

    printable_cpp_data = generate_cpp_data(cat_filename,
                                           cat_timestamp,
                                           "BooleanRange",
                                           "s_printableRanges",
                                           printable_ranges)

    printable_cpp_count = generate_cpp_count("BooleanRange",
                                            "s_printableRanges",
                                            "s_printableRangeCount")

    grapheme_extend_cpp_data = generate_cpp_data(derived_filename,
                                                 derived_timestamp,
                                                 "BooleanRange",
                                                 "s_graphemeExtendRanges", [
        x for x in derived_ranges if x.prop == "Grapheme_Extend"])

    grapheme_extend_cpp_count = generate_cpp_count("BooleanRange",
                                                   "s_graphemeExtendRanges",
                                                   "s_graphemeExtendRangeCount")

    width_estimate_intervals = generate_cpp_data(eaw_filename,
                                                 eaw_timestamp,
                                                 "BooleanRange",
                                                 "s_doubleFieldWidthRanges",
                                                 width_2_ranges)

    width_estimate_interval_count = generate_cpp_count("BooleanRange",
                                                       "s_doubleFieldWidthRanges",
                                                       "s_doubleFieldWidthRangeCount")

    return "\n\n".join(
        [gpb_cpp_data,
         gpb_cpp_count,
         emoji_cpp_data,
         emoji_cpp_count,
         printable_cpp_data,
         printable_cpp_count,
         grapheme_extend_cpp_data,
         grapheme_extend_cpp_count,
         width_estimate_intervals,
         width_estimate_interval_count])


if __name__ == "__main__":
    str_data = OUTER_TEMPLATE.lstrip().format(content=generate_data_tables(), FILENAME=FILENAME)
    byte_data = str_data.replace('\r\n', '\n').encode(encoding='ascii')
    os.write(sys.stdout.fileno(), byte_data)


# -----------------------------------------------------------------------------
#  Adapted for bde use from Microsoft MSVC library code, 2024
#  'https://github.com/microsoft/STL/blob/vs-2022-17.11/tools/unicode_properties_parse/unicode_properties_data_gen.py'
#
#  # Copyright (c) Microsoft Corporation.
#  # SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# -----------------------------------------------------------------------------

#  ----------------------------------------------------------------------------
#  Copyright 2024 Bloomberg Finance L.P.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#  ----------------------------- END-OF-FILE ----------------------------------
