//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_BASE_SHADER_HH
#define KATE_ENGINE_BASE_SHADER_HH


namespace kaTe {
    class BaseShader {
    public:
        BaseShader() = default;
        virtual ~BaseShader() = default;

        virtual auto bind() -> void = 0;
        virtual auto unbind() -> void = 0;
    };
}


#endif//KATE_ENGINE_BASE_SHADER_HH
