//
// Created by kate on 6/7/23.
//

#ifndef KATE_ENGINE_SANDBOX_HH
#define KATE_ENGINE_SANDBOX_HH

#include <memory>

#include <Tools/Common.hh>
#include <Renderer/Shader.hh>
#include <Renderer/VertexBuffer.hh>
#include <Renderer/IndexBuffer.hh>

namespace kaTe {
    class Sandbox {
    public:
        auto run() -> kaTe::Int32_T;

    private:
        auto updateCamera() -> void;
        auto updateSceneData() -> void;
        auto renderScene() -> void;
    private:
        void setupData();

    private:
        std::shared_ptr<Shader> m_Shader{};
        std::shared_ptr<VertexBuffer> m_VertexBuffer{};
        std::shared_ptr<IndexBuffer> m_IndexBuffer{};

        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer{};
        std::shared_ptr<IndexBuffer> m_SquareIndexBuffer{};
        void swapBuffers();
    };
}


#endif//KATE_ENGINE_SANDBOX_HH
