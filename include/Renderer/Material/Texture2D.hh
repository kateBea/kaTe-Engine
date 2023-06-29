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
        enum class Type {
            NONE,
            DIFFUSE,
            SPECULAR,
            NORMAL,
            COUNT,
        };

        static auto CreateTexture(const Path_T& path) -> std::shared_ptr<Texture>;
        static auto CreateTextureRawPtr(const Path_T &path) -> Texture*;

        static auto LoadFromFile(const Path_T &path, Type type) -> std::shared_ptr<kaTe::Texture>;
    };
}


#endif//KATE_ENGINE_TEXTURE2D_HH
