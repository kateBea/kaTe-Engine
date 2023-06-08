//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_TEXTURE2D_HH
#define KATE_ENGINE_TEXTURE2D_HH

#include <Tools/Common.hh>
#include <Renderer/Material/Texture.hh>

namespace kaTe {
    class Texture2D : public Texture {
    public:
        static auto createTexture(const Path_T& path) -> Texture*;
    };
}


#endif//KATE_ENGINE_TEXTURE2D_HH
