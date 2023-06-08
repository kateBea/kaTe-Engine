//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_TEXTURE_HH
#define KATE_ENGINE_TEXTURE_HH

#include <Tools/Common.hh>

namespace kaTe {
    class Texture {
    public:
        Texture() = default;
        virtual ~Texture() = default;
        virtual auto getWidth() -> UInt32_T = 0;
        virtual auto getHeight() -> UInt32_T = 0;

        virtual auto bind(UInt32_T slot = 0) -> void = 0;

    private:

    };
}

#endif//KATE_ENGINE_TEXTURE_HH
