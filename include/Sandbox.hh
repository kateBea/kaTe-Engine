/**
 * Sandbox.hh
 * Created by kate on 6/7/23.
 * */
#ifndef KATE_ENGINE_SANDBOX_HH
#define KATE_ENGINE_SANDBOX_HH

#include <memory>

#include <Tools/Common.hh>

namespace kaTe {
    class Sandbox {
    public:
        auto run() -> kaTe::Int32_T;
    };
}


#endif//KATE_ENGINE_SANDBOX_HH
