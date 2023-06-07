//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_RENDER_CONTEXT_HH
#define KATE_ENGINE_RENDER_CONTEXT_HH

#include <any>

namespace kaTe {
    class RenderContext {
    public:
        RenderContext() = default;

        virtual auto init(std::any windowHandle) -> void = 0;
        virtual auto shutDown() -> void = 0;
        virtual auto swapBuffers() -> void = 0;

        virtual ~RenderContext() = default;
    private:
        // Forbidden operations on Contexts
        RenderContext(const RenderContext&) = delete;
        auto operator=(const RenderContext&) -> RenderContext& = delete;

        RenderContext(RenderContext&&) = delete;
        auto operator=(RenderContext&&) -> RenderContext& = delete;
    };
}


#endif//KATE_ENGINE_RENDER_CONTEXT_HH
