// bdlt_iso8601util.cpp                                               -*-C++-*-
#include <bdlt_iso8601util.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlt_iso8601util_cpp,"$Id$ $CSID$")

#include <bdlt_date.h>
#include <bdlt_datetime.h>
#include <bdlt_datetimeinterval.h>
#include <bdlt_datetimetz.h>
#include <bdlt_datetz.h>
#include <bdlt_time.h>
#include <bdlt_timetz.h>

#include <bsl_algorithm.h>
#include <bsl_cctype.h>
#include <bsl_cstdlib.h>
#include <bsl_cstring.h>

namespace BloombergLP {
namespace bdlt {

namespace {

// STATIC HELPER FUNCTIONS

static
int parseUint(const char **nextPos,
              int         *result,
              const char  *begin,
              const char  *end)
    // Parse the unsigned integer described by the string starting at specified
    // 'begin' and ending before the specified 'end', then load the integer
    // value into the specified 'result' and load '*nextPos' with the address
    // one past the last character parsed.  Return 0 on success and a non-zero
    // value otherwise.  Failure will occur if the range [begin, end) does not
    // begin with at least one digit or if the string of digits is more than 10
    // chars long.  Characters beginning with the first non-digit char in the
    // input string are ignored.  The behavior is undefined unless 'begin' and
    // 'end' describe a contiguous range of memory or if the string of digits
    // represent a value that cannot be represented in a signed integer.  Note
    // that if a non-digit char is encountered at least one char after 'begin'
    // but before 'end', it is not an error but merely terminates parsing.
{
    BSLS_ASSERT(nextPos);
    BSLS_ASSERT(result);
    BSLS_ASSERT(begin);
    BSLS_ASSERT(end);

    static const int maxDigits = 10;

    // Stop parsing at 'maxDigits'.

    if (end - begin > maxDigits) {
        end = begin + maxDigits + 1;  // Parse one extra digit.
    }

    char buf[maxDigits + 2];  // Make room for one extra digit.
    char *bufPtr = buf;
    const char *inPtr = begin;
    while (inPtr < end && isdigit(*inPtr)) {
        *bufPtr++ = *inPtr++;
    }

    const int parsedLen = static_cast<int>(inPtr - begin);
    if (0 == parsedLen || parsedLen > maxDigits) {
        // Parsed too few or too many digits.

        return -1;                                                    // RETURN
    }

    *nextPos = inPtr;
    *bufPtr = '\0';
    *result = bsl::atoi(buf);

    return 0;
}

static
int parseDate(int         *year,
              int         *month,
              int         *day,
              const char **begin,
              const char  *end)
    // Parse a date, represented in "YYYY-MM-DD" format, from the string
    // starting at the specified '*begin' and ending before the specified 'end'
    // then load the specified 'year', 'month' and 'day' with their respective
    // parsed values, and set '*begin' to the location one past the last parsed
    // character.  In the "YYYY-MM-DD" format accepted by this function,
    // 'YYYY', 'MM', and 'DD' are strings representing positive integers, '-'
    // is literally a dash character, 'YYYY' is 4 chars long, and 'MM' and 'DD'
    // are both 2 chars long.  Return 0 on success and a non-zero value if the
    // string being parsed does not match the specified format (including
    // partial representations).  Note that if the pattern is successfully
    // completed before 'end' is reached, that is not an error and 0 is
    // returned.
{
    BSLS_ASSERT(year);
    BSLS_ASSERT(month);
    BSLS_ASSERT(day);
    BSLS_ASSERT(begin);
    BSLS_ASSERT(end);

    const char *p = *begin;

    // Parse year.

    const char *expectedEnd = p + 4;
    if (0   != parseUint(&p, year, p, end - 1)
     || p   != expectedEnd
     || '-' != *p) {

        return -1;                                                    // RETURN
    }
    ++p;  // skip hyphen

    // Parse month.

    expectedEnd = p + 2;
    if (0   != parseUint(&p, month, p, end - 1)
     || p   != expectedEnd
     || '-' != *p) {

        return -1;                                                    // RETURN
    }
    ++p;  // skip hyphen

    // Parse day.

    expectedEnd = p + 2;
    if (0 != parseUint(&p, day, p, end) || p != expectedEnd) {

        return -1;                                                    // RETURN
    }

    *begin = p;

    return 0;
}

static
int parseTimeFraction(int         *millisecond,
                      const char **begin,
                      const char  *end)
    // Parse the fractional part of a second after '.' in a time, returning the
    // value in the specified '*millisecond'.  The specified '*begin', on
    // input, points to the beginning of the input, starting after the '.'.
    // The specified 'end' points to the end of input, and may contain trailing
    // data after the fractional part of a second.  Upon returning, '*begin'
    // points after the parsed fractional part of a second, which may be of
    // unbounded length.  Return 0 on success and a non-zero value otherwise.
    // Note that if the string passed is of 0 length or if it doesn't begin
    // with at least one digit, an error code is returned.
{
    BSLS_ASSERT(millisecond);
    BSLS_ASSERT(begin);
    BSLS_ASSERT(end);

    const char *const start = *begin;
    BSLS_ASSERT(start);

    // there must be at least one digit

    if (start >= end || !isdigit(*start)) {
        return -1;                                                    // RETURN
    }

    // Get 'buf' containing, in string form, the fractional time in tenths of
    // microseconds.  Note we already know that the first char is good.

    char buf[5] = { "0000" };    // must not be static
    const char *p = start;
    const char *const end2 = bsl::min(end, start + 4);
    do {
        ++p;
    } while (p < end2 && isdigit(*p));
    bsl::memcpy(buf, start, p - start);

    // round to nearest millisecond

    *millisecond = (bsl::atoi(buf) + 5) / 10;

    // ignore any low order digits after first 4, skipping them

    while (p < end && isdigit(*p)) {
        ++p;
    }
    *begin = p;

    return 0;
}

static
int parseTime(int         *hour,
              int         *minute,
              int         *second,
              int         *millisecond,
              bool        *hasLeapSecond,
              const char **begin,
              const char  *end)
    // Parse a time, represented in "hh:mm:ss[.d+]" format, from the string
    // starting at the specified '*begin' and ending before the specified
    // 'end', then load the specified 'hour', 'minute', 'second', and
    // 'millisecond' with their respective parsed values, and set '*begin' to
    // the location one past the last parsed character.  In the "hh:mm:ss[.d+]"
    // format accepted by this function, 'hh', 'mm', 'ss' are all 2 digit
    // integers (left padded with 0's if necessary) denoting hours, minutes,
    // and seconds, ':' is literally a colon character, and '[.d+]' is the
    // optional fraction of a second, consisting of a '.' followed by one or
    // more decimal digits.  If '60 == ss', interpret it as a leap second, and
    // set '*second' to 59 and set the specified '*hasLeapSecond' to 'true';
    // otherwise set '*hasLeapSecond' to 'false'.  If '[.d+]' contains more
    // than 3 digits, the value will be rounded to the nearest value in
    // milliseconds.  Return 0 on success and a non-zero value if the string
    // being parsed does not match the specified format (including partial
    // representations).  Note that a fractional second value of '.9995' or
    // more will be rounded to 1000 milliseconds (and higher level functions
    // are responsible for incrementing the represented value of 'seconds' if
    // necessary).  Also note that if the pattern is successfully completed
    // before 'end' is reached, that is not an error.
{
    BSLS_ASSERT(hour);
    BSLS_ASSERT(minute);
    BSLS_ASSERT(second);
    BSLS_ASSERT(millisecond);
    BSLS_ASSERT(hasLeapSecond);
    BSLS_ASSERT(begin);
    BSLS_ASSERT(end);

    const char *p = *begin;

    // Parse hour.

    const char *expectedEnd = p + 2;
    if (0   != parseUint(&p, hour, p, end - 1)
     || p   != expectedEnd
     || ':' != *p) {

        return -1;                                                    // RETURN
    }
    ++p;  // skip colon

    // Parse minute.

    expectedEnd = p + 2;
    if (0   != parseUint(&p, minute, p, end - 1)
     || p   != expectedEnd
     || ':' != *p) {

        return -1;                                                    // RETURN
    }
    ++p;  // skip colon

    // Parse second.

    expectedEnd = p + 2;
    if (0 != parseUint(&p, second, p, end) || p != expectedEnd) {

        return -1;                                                    // RETURN
    }

    // Parse millisecond.

    if (p < end && '.' == *p) {
        // We have a fraction of a second.

        ++p;  // skip dot
        if (0 != parseTimeFraction(millisecond, &p, end)) {
            return -1;                                                // RETURN
        }
    }
    else {
        *millisecond = 0;
    }

    // Parse hasLeapSecond

    if (60 == *second) {
        *hasLeapSecond = true;
        *second = 59;
    }
    else {
        *hasLeapSecond = false;
    }

    *begin = p;

    return 0;
}

static
int parseTimezoneOffset(int         *minuteOffset,
                        const char **begin,
                        const char  *end)
    // Parse a time zone offset, represented in either "([Zz]|[+-]hh:mm"
    // format, from the string starting at the specified '*begin' and ending
    // before the specified 'end' then load the specified 'minuteOffset' with
    // the parsed time zone offset (in minutes), and set '*begin' to the
    // location one past the last parsed character.  If the timezone begins
    // with 'z' or 'Z', it is one char long, and equivalent to "+00:00".
    // Otherwise it starts with '+' or '-' followed by "hh:mm" where 'hh'
    // represents hours and 'mm' represents minutes.  Note 'hh' and 'mm' are 2
    // digit integers (left padded with '0's if necessary).  'hh' must be in
    // the range '[ 00, 24 )' and 'mm' must be in the range '[ 0, 60 )'.
    // Return 0 on success and a non-zero value if the string being parsed does
    // not match the specified format.  Note that if the pattern is
    // successfully completed before 'end' is reached, that is not an error.
{
    BSLS_ASSERT(minuteOffset);
    BSLS_ASSERT(begin);
    BSLS_ASSERT(end);

    const char *p = *begin;

    if (p >= end) {
        return -1;                                                    // RETURN
    }

    char sign = *p++;

    if ('Z' == sign || 'z' == sign) {
        *minuteOffset = 0;
        *begin = p;

        return 0;                                                     // RETURN
    }
    else if ('+' != sign && '-' != sign) {
        return -1;                                                    // RETURN
    }

    // We have parsed a '+' or '-'.  Make sure it is followed by at least 5
    // more chars ('hh:mm').

    // read hour

    int hourVal;
    const char *expectedEnd = p + 2;
    if (0       != parseUint(&p, &hourVal, p, end - 1)
     || p       != expectedEnd
     || hourVal >= 24  // Max TZ offset is 23:59
     || ':'     != *p) {

        return -1;                                                    // RETURN
    }
    ++p;  // skip ':'

    // read minute

    int minuteVal;
    expectedEnd = p + 2;
    if (0 != parseUint(&p, &minuteVal, p, end)
     || p != expectedEnd
     || minuteVal > 59) {

        return -1;                                                    // RETURN
    }

    *minuteOffset = hourVal * 60 + minuteVal;
    if ('-' == sign) {
        *minuteOffset = -*minuteOffset;
    }
    *begin = p;

    return 0;
}

static
char *generateInt(char *buffer, int val, int len)
    // Write into the specified 'buffer', the decimal value of the specified
    // 'val', left-padded with zeros to the specified 'len' and return
    // 'buffer + len'.  The buffer is NOT null-terminated.  Note that if the
    // decimal string representation of 'val' is more than 'len' digits, only
    // the low order digits are printed.
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= len);

    buffer += len;
    for (char *p = buffer; len != 0; --len) {
        *--p = static_cast<char>('0' + val % 10);
        val /= 10;
    }

    return buffer;
}

static inline
char *generateInt(char *buffer, int val, int len, char separator)
    // Write into the specified 'buffer', the decimal value of the specified
    // 'val', left-padded with zeros to the specified 'len' and followed by
    // the specified 'separator' character then return 'buffer + len + 1'.
    // The buffer is NOT null-terminated.
{
    BSLS_ASSERT_SAFE(buffer);
    BSLS_ASSERT_SAFE(0 <= len);

    buffer = generateInt(buffer, val, len);
    *buffer++ = separator;

    return buffer;
}

static
char *generateTimeZoneOffset(char *buffer,
                             int   tzOffset,
                             bool  useZAbbreviationForUtc)
    // Write into the specified 'buffer', the formatted time zone offset
    // indicated by the specified 'tzOffset', which is the character 'Z' if
    // 'tzOffset' is 0 and the specified 'useZAbbreviationForUtc' is 'true',
    // and a numeric offset otherwise.
{
    if (0 == tzOffset && useZAbbreviationForUtc) {
        *buffer++ = 'Z';
    }
    else {
        char timezoneSign;

        if (0 > tzOffset) {
            tzOffset     = -tzOffset;
            timezoneSign = '-';
        }
        else {
            timezoneSign = '+';
        }

        // TZ offset cannot be more than 24 hours.

        BSLS_ASSERT(tzOffset <= 24 * 60);

        *buffer++ = timezoneSign;
        buffer = generateInt(buffer, tzOffset / 60        , 2, ':');
        buffer = generateInt(buffer, tzOffset % 60        , 2     );
    }

    return buffer;
}

static
int copyBuf(char *dest, int destLen, const char *src, int srcLen)
    // Copy the contents of the specified 'src' string having the specified
    // 'srcLen' into the specified 'dest' buffer having the specified
    // 'destLen' and return 'srcLen'.  If 'destLen > srcLen', add a null
    // terminator.  If 'srcLen >= destLen', then copy only 'destLen' bytes and
    // add do not add a null terminator.
{
    BSLS_ASSERT(dest);
    BSLS_ASSERT(0 <= destLen);
    BSLS_ASSERT(src);
    BSLS_ASSERT(0 <= srcLen);

    if (destLen > srcLen) {
        bsl::memcpy(dest, src, srcLen);
        dest[srcLen] = '\0';
    }
    else {
        bsl::memcpy(dest, src, destLen);
    }

    return srcLen;
}

}  // close unnamed namespace

                            // ------------------
                            // struct Iso8601Util
                            // ------------------

// CLASS METHODS
int Iso8601Util::generate(char *buffer, const Date& object, int bufferLength)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= bufferLength);

    char outBuf[k_DATE_STRLEN];
    const int outLen = generateRaw(outBuf, object);
    BSLS_ASSERT(outLen == sizeof(outBuf));

    return copyBuf(buffer, bufferLength, outBuf, outLen);
}

int
Iso8601Util::generate(char *buffer, const Datetime& object, int bufferLength)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= bufferLength);

    char outBuf[k_DATETIME_STRLEN];
    const int outLen = generateRaw(outBuf, object);
    BSLS_ASSERT(outLen == sizeof(outBuf));

    return copyBuf(buffer, bufferLength, outBuf, outLen);
}

int Iso8601Util::generate(char *buffer, const Time& object, int bufferLength)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= bufferLength);

    char outBuf[k_TIME_STRLEN];
    const int outLen = generateRaw(outBuf, object);
    BSLS_ASSERT(outLen == sizeof(outBuf));

    return copyBuf(buffer, bufferLength, outBuf, outLen);
}

int Iso8601Util::generate(char          *buffer,
                          const DateTz&  object,
                          int            bufferLength,
                          bool           useZAbbreviationForUtc)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= bufferLength);

    char outBuf[k_DATETZ_STRLEN];
    const int outLen = generateRaw(outBuf, object, useZAbbreviationForUtc);

    BSLS_ASSERT(outLen == sizeof(outBuf) ||
                (0 == object.offset() && useZAbbreviationForUtc &&
                 outLen <= static_cast<int>(sizeof(outBuf))));

    return copyBuf(buffer, bufferLength, outBuf, outLen);
}

int Iso8601Util::generate(char              *buffer,
                          const DatetimeTz&  object,
                          int                bufferLength,
                          bool               useZAbbreviationForUtc)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= bufferLength);

    char outBuf[k_DATETIMETZ_STRLEN];
    const int outLen = generateRaw(outBuf, object, useZAbbreviationForUtc);

    BSLS_ASSERT(outLen == sizeof(outBuf) ||
                (0 == object.offset() && useZAbbreviationForUtc &&
                 outLen <= static_cast<int>(sizeof(outBuf))));

    return copyBuf(buffer, bufferLength, outBuf, outLen);
}

int Iso8601Util::generate(char          *buffer,
                          const TimeTz&  object,
                          int            bufferLength,
                          bool           useZAbbreviationForUtc)
{
    BSLS_ASSERT(buffer);
    BSLS_ASSERT(0 <= bufferLength);

    char outBuf[k_TIMETZ_STRLEN];
    const int outLen = generateRaw(outBuf, object, useZAbbreviationForUtc);

    BSLS_ASSERT(outLen == sizeof(outBuf) ||
                (0 == object.offset() && useZAbbreviationForUtc &&
                 outLen <= static_cast<int>(sizeof(outBuf))));

    return copyBuf(buffer, bufferLength, outBuf, outLen);
}

int Iso8601Util::generateRaw(char *buffer, const Date& object)
{
    BSLS_ASSERT(buffer);

    char *outp = buffer;
    outp = generateInt(outp, object.year() , 4, '-');
    outp = generateInt(outp, object.month(), 2, '-');
    outp = generateInt(outp, object.day()  , 2     );

    return static_cast<int>(outp - buffer);
}

int Iso8601Util::generateRaw(char *buffer, const Datetime& object)
{
    BSLS_ASSERT(buffer);

    char *outp = buffer;
    outp = generateInt(outp, object.year()       , 4, '-');
    outp = generateInt(outp, object.month()      , 2, '-');
    outp = generateInt(outp, object.day()        , 2, 'T');
    outp = generateInt(outp, object.hour()       , 2, ':');
    outp = generateInt(outp, object.minute()     , 2, ':');
    outp = generateInt(outp, object.second()     , 2, '.');
    outp = generateInt(outp, object.millisecond(), 3     );

    return static_cast<int>(outp - buffer);
}

int Iso8601Util::generateRaw(char *buffer, const Time& object)
{
    BSLS_ASSERT(buffer);

    char *outp = buffer;
    outp = generateInt(outp, object.hour()       , 2, ':');
    outp = generateInt(outp, object.minute()     , 2, ':');
    outp = generateInt(outp, object.second()     , 2, '.');
    outp = generateInt(outp, object.millisecond(), 3     );

    return static_cast<int>(outp - buffer);
}

int Iso8601Util::generateRaw(char          *buffer,
                             const DateTz&  object,
                             bool           useZAbbreviationForUtc)
{
    BSLS_ASSERT(buffer);

    Date localDate = object.localDate();

    char *outp = buffer;
    outp = generateInt(outp, localDate.year() , 4, '-');
    outp = generateInt(outp, localDate.month(), 2, '-');
    outp = generateInt(outp, localDate.day()  , 2);
    outp = generateTimeZoneOffset(outp,
                                  object.offset(),
                                  useZAbbreviationForUtc);

    return static_cast<int>(outp - buffer);
}

int Iso8601Util::generateRaw(char              *buffer,
                             const DatetimeTz&  object,
                             bool               useZAbbreviationForUtc)
{
    BSLS_ASSERT(buffer);

    Datetime localDatetime = object.localDatetime();

    char *outp = buffer;
    outp = generateInt(outp, localDatetime.year()       , 4, '-');
    outp = generateInt(outp, localDatetime.month()      , 2, '-');
    outp = generateInt(outp, localDatetime.day()        , 2, 'T');
    outp = generateInt(outp, localDatetime.hour()       , 2, ':');
    outp = generateInt(outp, localDatetime.minute()     , 2, ':');
    outp = generateInt(outp, localDatetime.second()     , 2, '.');
    outp = generateInt(outp, localDatetime.millisecond(), 3);
    outp = generateTimeZoneOffset(outp,
                                  object.offset(),
                                  useZAbbreviationForUtc);

    return static_cast<int>(outp - buffer);
}

int Iso8601Util::generateRaw(char          *buffer,
                             const TimeTz&  object,
                             bool           useZAbbreviationForUtc)
{
    BSLS_ASSERT(buffer);

    char *outp = buffer;

    Time localTime = object.localTime();

    outp = generateInt(outp, localTime.hour()       , 2, ':');
    outp = generateInt(outp, localTime.minute()     , 2, ':');
    outp = generateInt(outp, localTime.second()     , 2, '.');
    outp = generateInt(outp, localTime.millisecond(), 3);
    outp = generateTimeZoneOffset(outp,
                                  object.offset(),
                                  useZAbbreviationForUtc);

    return static_cast<int>(outp - buffer);
}

int Iso8601Util::parse(Date *result, const char *input, int inputLength)
{
    BSLS_ASSERT(result);
    BSLS_ASSERT(input);
    BSLS_ASSERT(0 <= inputLength);

    // Sample XML date: "2005-01-31" having a minimum length of 10.

    enum { k_MINIMUM_LENGTH = 10 };

    if (inputLength < k_MINIMUM_LENGTH) {
        return -1;                                                    // RETURN
    }

    const char *begin = input;
    const char *end   = input + inputLength;

    // Parse date.

    int year, month, day;
    if (0 != parseDate(&year, &month, &day, &begin, end)) {
        return -1;                                                    // RETURN
    }
    if (! Date::isValid(year, month, day)) {
        return -1;                                                    // RETURN
    }

    // parse and ignore timezone, if any

    if (end != begin) {
        int tzOffset;
        if (0 != parseTimezoneOffset(&tzOffset, &begin, end) || end != begin) {
            return -1;                                                // RETURN
        }
    }

    result->setYearMonthDay(year, month, day);

    return 0;
}

int Iso8601Util::parse(Datetime *result, const char *input, int inputLength)
{
    BSLS_ASSERT(result);
    BSLS_ASSERT(input);
    BSLS_ASSERT(0 <= inputLength);

    DatetimeTz datetimeTz;

    const int rc = parse(&datetimeTz, input, inputLength);

    if (0 != rc) {
        return rc;                                                    // RETURN
    }

    if (datetimeTz.offset() > 0) {
        Datetime min(0001, 01, 01, 00, 00, 00, 000);

        min.addMinutes(datetimeTz.offset());
        if (min > datetimeTz.localDatetime()) {
            return -1;                                                // RETURN
        }
    }
    else if (datetimeTz.offset() < 0) {
        Datetime max(9999, 12, 31, 23, 59, 59, 999);

        max.addMinutes(datetimeTz.offset());
        if (max < datetimeTz.localDatetime()) {
            return -1;                                                // RETURN
        }
    }

    *result = datetimeTz.utcDatetime();

    return 0;
}

int Iso8601Util::parse(Time *result, const char *input, int inputLength)
{
    BSLS_ASSERT(result);
    BSLS_ASSERT(input);
    BSLS_ASSERT(0 <= inputLength);

    // Sample XML time: "08:59:59.999" having a minimum length of 8.  Also,
    // there might be more than 3 decimal places for the fraction of a second.
    // But when storing in Datetime, we only take the 3 most significant
    // digits.

    enum { k_MINIMUM_LENGTH = 8 };

    if (inputLength < k_MINIMUM_LENGTH) {
        return -1;                                                    // RETURN
    }

    const char *begin = input;
    const char *end   = input + inputLength;

    // Parse time.  Milliseconds could be 1000 (if fraction is .9995 or
    // greater).  Thus, we have to add it after setting the time else it might
    // not validate.

    Time localTime;
    int hour, minute, second, millisecond;
    bool hasLeapSecond;
    if (0 != parseTime(&hour,
                       &minute,
                       &second,
                       &millisecond,
                       &hasLeapSecond,
                       &begin,
                       end)
     || 0 != localTime.setTimeIfValid(hour, minute, second)) {

        return -1;                                                    // RETURN
    }
    if (hasLeapSecond) {
        localTime.addSeconds(1);
    }
    if (millisecond) {
        localTime.addMilliseconds(millisecond);
    }

    // Parse timezone, if any

    int timezoneOffset = 0;  // minutes from GMT
    if (end != begin) {
        if (  0   != parseTimezoneOffset(&timezoneOffset, &begin, end)
           || end != begin) {

            return -1;                                                // RETURN
        }
        if (timezoneOffset) {
            localTime.addMinutes(-timezoneOffset);  // convert to GMT
        }
    }

    // 'addTime' and/or 'addMinutes' will reset '24:00:00' to '00:00:00' (even
    // if the quantities added are 0), which we don't want to happen.
    // 'hours == 24' is only allowed for the value '24:00:00.000' with timezone
    // UTC.

    if (24 == hour && (millisecond || timezoneOffset)) {
        return -1;                                                    // RETURN
    }

    *result = localTime;

    return 0;
}

int Iso8601Util::parse(DateTz *result, const char *input, int inputLength)
{
    BSLS_ASSERT(result);
    BSLS_ASSERT(input);
    BSLS_ASSERT(0 <= inputLength);

    // Sample XML datetime: "2005-01-31" having a minimum length of 10.
    // Timezone may be optionally specified: "2005-01-31-04:00".  Also, there
    // might be more than 3 decimal places for the fraction of a second.  But
    // when storing in Datetime, we only take the 3 most significant digits.

    enum { k_MINIMUM_LENGTH = 10 };

    if (inputLength < k_MINIMUM_LENGTH) {
        return -1;                                                    // RETURN
    }

    const char *begin = input;
    const char *end   = input + inputLength;

    // Parse date.

    Date localDate;
    int year, month, day;
    if (0 != parseDate(&year, &month, &day, &begin, end)
     || 0 != localDate.setYearMonthDayIfValid(year, month, day)) {

        return -1;                                                    // RETURN
    }

    // Parse timezone, if any

    int timezoneOffset = 0;  // minutes from GMT
    if (end != begin) {
        if (0   != parseTimezoneOffset(&timezoneOffset, &begin, end)
         || end != begin) {

            return -1;                                                // RETURN
        }
    }

    result->setDateTz(localDate, timezoneOffset);

    return 0;
}

int Iso8601Util::parse(DatetimeTz *result, const char *input, int inputLength)
{
    BSLS_ASSERT(result);
    BSLS_ASSERT(input);
    BSLS_ASSERT(0 <= inputLength);

    // Sample XML datetime: "2005-01-31T08:59:59.999" having a minimum length
    // of 19.  Timezone may be optionally specified:
    // "2005-01-31T08:59:59.999-04:00".  Also, there might be more than 3
    // decimal places for the fraction of a second.  But when storing in
    // Datetime, we only take the 3 most significant digits.

    enum { k_MINIMUM_LENGTH = 19 };

    if (inputLength < k_MINIMUM_LENGTH) {
        return -1;                                                    // RETURN
    }

    const char *begin = input;
    const char *end   = input + inputLength;

    // 1. Parse Date
    // =============

    int year, month, day;

    if (0   != parseDate(&year, &month, &day, &begin, end)
     || end <= begin
     || 'T' != *begin) {

        return -1;                                                    // RETURN
    }
    ++begin;  // skip 'T'

    // 2. Parse Time
    // =============

    Datetime localDatetime;
    int  hour, minute, second, millisecond;
    bool hasLeapSecond;

    if (0 != parseTime(&hour,
                       &minute,
                       &second,
                       &millisecond,
                       &hasLeapSecond,
                       &begin,
                       end)) {

        return -1;                                                    // RETURN
    }

    // 3. Parse Timezone (if any)
    // ==========================

    int timezoneOffset = 0;  // minutes from GMT

    if (end != begin) {
        if (0   != parseTimezoneOffset(&timezoneOffset, &begin, end)
         || end != begin) {

            return -1;                                                // RETURN
        }
    }

    // 4. Adjust Special ISO-8601 Values
    // =================================

    ///Leap Seconds
    /// - - - - - -
    // Note that leap seconds, milliseconds == 1000, cannot be directly
    // represented with a 'Datetime'.  Instead we create the simplest related
    // 'Datetime' value and then adjust it (forward a second).
    //
    // Also note milliseconds == 1000 is the result of rounding up a
    // fractional number of seconds.
    //
    ///24:00
    ///- - -
    // The hour value 24 is not valid unless minutes, seconds, and milliseconds
    // are all 0.  Further, note that supplying an hour of 24 results in a
    // different interpretation of the hour value than that provided by the
    // standard (see 'Note Regarding the Time 24:00' in the .h file).

    DatetimeInterval resultAdjustment;  // for leap-seconds, midnight, etc.

    if (hour == 24) {
        // 'hours == 24' is only allowed for the value '24:00:00.000'.
        if (0 != minute || 0 != second || 0 != millisecond) {
            return -1;                                                // RETURN
        }
    }

    if (hasLeapSecond) {
        resultAdjustment.addSeconds(1);
    }

    if (1000 == millisecond) {
        millisecond = 0;
        resultAdjustment.addSeconds(1);
    }


    // 5. Load a Datetime
    // ==================

    if (0 != localDatetime.setDatetimeIfValid(
                        year, month, day, hour, minute, second, millisecond)) {

        return -1;                                                    // RETURN
    }

    // 6. Apply Adjustments for Special ISO-8601 Values
    // ================================================

    if (DatetimeInterval() != resultAdjustment) {

        BSLS_ASSERT(resultAdjustment > DatetimeInterval());
        // We assert 'resultAdjustment > DatetimeInterval()' to prevent future
        // developers from accidentally introducing negative adjustments (which
        // are not handled by the following logic).

        const Datetime maxDatetime(9999, 12, 31, 23, 59, 59, 999);

        if (maxDatetime - resultAdjustment < localDatetime) {
            return -1;                                                // RETURN
        }
        localDatetime += resultAdjustment;
    }

    result->setDatetimeTz(localDatetime, timezoneOffset);

    return 0;
}

int Iso8601Util::parse(TimeTz *result, const char *input, int inputLength)
{
    BSLS_ASSERT(result);
    BSLS_ASSERT(input);
    BSLS_ASSERT(0 <= inputLength);

    // Sample XML time: "08:59:59.999" having a minimum length of 8.  Also,
    // there might be more than 3 decimal places for the fraction of a second.

    enum { k_MINIMUM_LENGTH = 8 };

    if (inputLength < k_MINIMUM_LENGTH) {
        return -1;                                                    // RETURN
    }
    const char *begin = input;
    const char *end   = input + inputLength;

    // Parse time.  Milliseconds could be rounded to 1000 (if fraction is .9995
    // or greater).  Thus, we have to add it after setting the time else it
    // might not validate.

    Time localTime;
    int hour, minute, second, millisecond;
    bool hasLeapSecond;

    if (0 != parseTime(&hour,
                       &minute,
                       &second,
                       &millisecond,
                       &hasLeapSecond,
                       &begin,
                       end)
     || 0 != localTime.setTimeIfValid(hour, minute, second)) {

        return -1;                                                    // RETURN
    }
    if (hasLeapSecond) {
        localTime.addSeconds(1);
    }
    if (millisecond) {
        localTime.addMilliseconds(millisecond);
    }

    // Parse timezone, if any.

    int timezoneOffset = 0;  // minutes from GMT

    if (end != begin) {
        // Parse timezone.

        if (0   != parseTimezoneOffset(&timezoneOffset, &begin, end)
         || end != begin) {

            return -1;                                                // RETURN
        }
    }

    // 'addMilliseconds' will reset '24:00:00' to '00:00:00' (even if the
    // quantity added is 0), which we don't want to happen.  'hours == 24' is
    // only allowed for the value '24:00:00.000' and timezone GMT.

    if (24 == hour && (millisecond || timezoneOffset)) {
        return -1;                                                    // RETURN
    }

    result->setTimeTz(localTime, timezoneOffset);

    return 0;
}

                        // -------------------------------
                        // struct Iso8601UtilConfiguration
                        // -------------------------------

// CLASS DATA
bool Iso8601UtilConfiguration::s_useZAbbreviationForUtc = false;

// CLASS METHODS
void Iso8601UtilConfiguration::setUseZAbbreviationForUtc(
                                               bool useZAbbreviationForUtcFlag)
{
    s_useZAbbreviationForUtc = useZAbbreviationForUtcFlag;
}

bool Iso8601UtilConfiguration::useZAbbreviationForUtc()
{
    return s_useZAbbreviationForUtc;
}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2014 Bloomberg Finance L.P.
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
