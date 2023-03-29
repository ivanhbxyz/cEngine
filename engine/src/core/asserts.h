#pragma once
// Note that there IS an assert.h header in the std lib so this must be named asserts.

#include "defines.h"

#define KASSERTIONS_ENABLED

#ifdef KASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

KAPI void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);



// LMAOOOOO! THE BACKSLASHES \ ARE ACTUALLY REQUIRED?????! HOW? WHY??!

        // Note: That here we do something if expr is NOT True
        // Better to evalue the IF TRUE case first
#define KASSERT(expr)                                                   \
    {                                                                   \
        if(expr) {                                                      \
        } else {                                                        \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
            debugBreak();                                               \
        }                                                               \
    }                                                                   \


// Note: That here we do something if expr is NOT True
// Better to evalue the IF TRUE case first
#define KASSERT_MSG(expr, message)                                        \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }


// Note: That here we do something if expr is NOT True
        // Better to evalue the IF TRUE case first
#ifdef _DEBUG
#define KASSERT_DEBUG(expr)                                          \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }
#else
#define KASSERT_DEBUG(expr) // does nothing
#endif

#else
// What if they are not enabled...
#define KASSERT(expr)
#define KASSERT_MSG(expr, message)
#define KASSERT_DEBUG(expr)
#endif