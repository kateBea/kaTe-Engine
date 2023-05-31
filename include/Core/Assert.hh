/**
 * Assert.hh
 * Created by kate on 5/25/23.
 * */

#ifndef KATE_ENGINE_ASSERT_HH
#define KATE_ENGINE_ASSERT_HH

// C++ Standard Library
#include <cstdlib>

// Project Headers
#include <kaTe/Common.hh>

#if defined(NDEBUG) || defined(_DEBUG)
    #define KT_ENABLE_ASSERTIONS
#else
    #undef KT_ENABLE_ASSERTIONS
#endif

#if defined(KT_ENABLE_ASSERTIONS)
/**
 * Print __MESSAGE and abort program execution if __EXPR evaluates to false
 */
#define KT_ASSERT(__EXPR, __MESSAGE)                                       \
    do {                                                                   \
        if (!(__EXPR)) {                                                   \
            KT_COLOR_PRINT_FORMATTED(fmt::color::red, "MESSAGE: {}\n"      \
                                                      "FUNCTION: {}\n"     \
                                                      "SRC: {}\n"          \
                                                      "LINE: {}\n",        \
                                     __MESSAGE, __PRETTY_FUNCTION__,       \
                                     __FILE__, __LINE__);                  \
            std::abort();                                                  \
        }                                                                  \
    } while (false)

#else
    #define KT_ASSERT(__EXPR)
#endif


#endif //KATE_ENGINE_ASSERT_HH
