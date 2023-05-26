//
// Created by kate on 5/25/23.
//

#include <cassert>

#ifndef KATE_ENGINE_ASSERT_HH
#define KATE_ENGINE_ASSERT_HH

#if defined(KT_ENABLE_ASSERTIONS)
    #define KT_ASSERT(__EXPR) assert(__EXPR)
#endif

#endif //KATE_ENGINE_ASSERT_HH
