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
        virtual auto GetWidth() -> UInt32_T = 0;
        virtual auto GetHeight() -> UInt32_T = 0;

        virtual auto Bind(UInt32_T slot = 0) -> void = 0;

    private:

    };
}

#endif//KATE_ENGINE_TEXTURE_HH
