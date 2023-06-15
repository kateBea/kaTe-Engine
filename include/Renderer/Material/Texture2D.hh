//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_TEXTURE2D_HH
#define KATE_ENGINE_TEXTURE2D_HH

#include <memory>

#include <Tools/Common.hh>
#include <Renderer/Material/Texture.hh>

namespace kaTe {
    class Texture2D : public Texture {
    public:
        static auto CreateTexture(const Path_T& path) -> std::shared_ptr<Texture>;
    };
}


#endif//KATE_ENGINE_TEXTURE2D_HH
