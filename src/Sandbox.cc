/**
 * Snadbox.cc
 * Created by kate on 6/7/23.
 * */

#include <Sandbox.hh>

#include <Core/Logger.hh>
#include <Core/EngineManager.hh>

namespace kaTe {
    auto Sandbox::run() -> kaTe::Int32_T {
        EngineManager& engine{ EngineManager::get() };

        try {
            engine.init();
            setupData();

            while (engine.isRunning()) {
                updateCamera();
                updateSceneData();
                renderScene();
                swapBuffers();
            }

        }
        catch(const std::exception& exception) {
            KATE_APP_LOGGER_CRITICAL("Engine exception thrown.\n Message: {}", exception.what());
            return 1;
        }
        catch(...) {
            KATE_APP_LOGGER_CRITICAL("Engine exception thrown.\n Unknown type of exception");
            return 1;
        }

        engine.shutDown();
        return 0;
    }

    auto Sandbox::updateCamera() -> void {
        auto camera{ EngineManager::get().getOrthographicCamera() };

        static float posX{ 0.0f };
        static float posY{ 0.0f };
        static float rota{ 0.0f };

        // Try using the event system instead
        if (InputManager::isKeyPressed(KT_KEY_LEFT)) rota -= 1.0f;
        if (InputManager::isKeyPressed(KT_KEY_RIGHT)) rota += 1.0f;

        if (InputManager::isKeyPressed(KT_KEY_A)) posX += 0.01f;
        if (InputManager::isKeyPressed(KT_KEY_D)) posX -= 0.01f;
        if (InputManager::isKeyPressed(KT_KEY_W)) posY -= 0.01f;
        if (InputManager::isKeyPressed(KT_KEY_S)) posY += 0.01f;

        camera->setPosition(posX, posY);
        camera->setRotation(rota);
    }

    auto Sandbox::updateSceneData() -> void {
        auto engine{ EngineManager::getPtr() };
        float time = glfwGetTime();
        float blue = 0.5f;
        float red = (std::sin(time) + blue * 2) * blue;
        float green = (std::cos(time) + blue * 2) * blue;
        engine->getRenderer()->getRenderCommand().setClearColor(red, green, blue, 1.0f);
        engine->getRenderer()->getRenderCommand().clear((RendererAPI::BufferBit)(RendererAPI::OPEN_GL_COLOR_BUFFER_BIT |
                                                      RendererAPI::OPEN_GL_DEPTH_BUFFER_BIT));
        m_VertexBuffer->setBufferLayout(BufferLayout{ { ShaderDataType::FLOAT3_TYPE, "a_Position" }, { ShaderDataType::FLOAT4_TYPE, "a_Color" } });
    }

    auto Sandbox::renderScene() -> void {
        auto engine{ EngineManager::getPtr() };
        engine->getRenderer()->beginScene(engine->getOrthographicCamera());
        engine->getRenderer()->submit(m_Shader, m_VertexBuffer, m_IndexBuffer);
        engine->getRenderer()->endScene();
        engine->updateLayers();
    }

    void Sandbox::setupData() {
        std::vector<float> data {
                // Positions                // Colors
                -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.0f,          0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 0.5f, 0.0f,           1.0f, 1.0f, 0.0f, 1.0f,
        };

        m_VertexBuffer.reset(VertexBuffer::createBuffer(data));
        m_IndexBuffer.reset(IndexBuffer::createBuffer({ 0, 1, 2} ));
        m_Shader.reset(new Shader("../assets/shaders/debugShaderVert.glsl", "../assets/shaders/debugShaderFrag.glsl"));
    }

    void Sandbox::swapBuffers() {
        auto engine{ EngineManager::getPtr() };
        engine->getMainWindow().onUpdate();
    }
}