// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>


// --------------------------------------------------------------
// These macros provide an easy way to throw a std::runtime_error
// which includes file name, function name, and line number.  For
// example, the following call
// KOMP_THROW( "this is my message" );
// will throw a std::runtime_error with the message:
//     "error Throw.h (7) someFunctionName: 'this is my message'"
//
// If you want to check and throw if a pointer (or smart pointer)
// is null then use KOMP_THROW_IF_NULL( myPtr );
// --------------------------------------------------------------

#ifndef __FILENAME__

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

        #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

    #else

        #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

    #endif

#endif

#ifndef KOMP_THROW
#define KOMP_THROW(throw_error_message)                  \
throw std::runtime_error (                               \
std::string( "error " )                                  \
+ std::string( __FILENAME__ )                            \
+ std::string(" (")                                      \
+ std::string( std::to_string(__LINE__ ) )               \
+ std::string(") ")                                      \
+ std::string( __FUNCTION__ )                            \
+ std::string(": '")                                     \
+ std::string( throw_error_message )                     \
+ std::string( "'" ) );
#endif

// Define KOMP_ABORT_ON_FAILED_ASSERT if you want to abort the program when
// a KOMP_ASSERT statement fails.  Otherwise, if KOMP_ABORT_ON_FAILED_ASSERT
// is not defined then a std::runtime_error will be thrown instead of aborting.
// #define KOMP_ABORT_ON_FAILED_ASSERT

#ifdef KOMP_ABORT_ON_FAILED_ASSERT
    #define KOMP_ASSERT(expectedTrueStatement) assert(expectedTrueStatement);
#else

#ifndef KOMP_DEBUG
#define KOMP_DEBUG
#endif

#ifdef KOMP_DEBUG
    #define KOMP_ASSERT(expectedTrueStatement)                                       \
        if ( ! (expectedTrueStatement) )                                             \
        {                                                                            \
            KOMP_THROW("assertion failed '" #expectedTrueStatement "' is false");    \
        }
#else 
    // http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/
    #define KOMP_ASSERT(x) do { (void)sizeof(x);} while (0)
#endif

#endif

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
