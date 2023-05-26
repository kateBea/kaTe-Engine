//
// Created by kate on 5/25/23.
//

#include <cassert>
#include <cstdio>

#ifndef KATE_ENGINE_ASSERT_HH
#define KATE_ENGINE_ASSERT_HH

#if defined(NDEBUG) || defined(_DEBUG)
    #define KT_ENABLE_ASSERTIONS
#else
    #undef KT_ENABLE_ASSERTIONS
#endif

#if defined(KT_ENABLE_ASSERTIONS)
    /**
     * Abort program execution if __EXPR evaluates to false
     * */
    #define KT_ASSERT(__EXPR, __MESSAGE)                          \
        if (!(__EXPR)) {                                          \
            std::printf("MESSAGE: [ %s ]\n"                       \
                        "FUNCTION: [ %s ]\n"                      \
                        "SRC: [ %s ]\n"                           \
                        "LINE: %d\n",                             \
            #__MESSAGE, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
                                                                  \
            abort();                                              \
        }
#else
    #define KT_ASSERT(__EXPR)
#endif

#endif //KATE_ENGINE_ASSERT_HH
