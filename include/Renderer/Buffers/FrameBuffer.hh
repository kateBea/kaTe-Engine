//
// Created by kate on 6/23/23.
//

#ifndef KATE_ENGINE_FRAMEBUFFER_HH
#define KATE_ENGINE_FRAMEBUFFER_HH

#include <memory>

#include <Tools/Common.hh>

namespace kaTe {
    struct FrameBufferCreateInfo {
        Int32_T width{};
        Int32_T height{};
        UInt32_T samples{};

        // If it's true, we render to the screen and not to a specific Viewport
        bool swapChainTarget{};
    };

    class FrameBuffer {
    public:
        explicit FrameBuffer() = default;
        virtual ~FrameBuffer() = default;

        virtual auto Bind() -> void = 0;
        virtual auto Unbind() -> void = 0;
        virtual auto Resize(UInt32_T width, UInt32_T height) -> void = 0;

        KT_NODISCARD virtual auto GetId() -> UInt32_T = 0;
        KT_NODISCARD virtual auto GetFrameBufferProperties() const -> const FrameBufferCreateInfo& = 0;

        KT_NODISCARD virtual auto GetColorAttachmentId() -> UInt32_T = 0;
        KT_NODISCARD virtual auto GetDepthAttachmentId() -> UInt32_T = 0;

        KT_NODISCARD static auto CreatFrameBuffer(const FrameBufferCreateInfo& properties) -> std::shared_ptr<FrameBuffer>;
        KT_NODISCARD static auto CreatFrameBufferRawPtr(const FrameBufferCreateInfo& properties) -> FrameBuffer*;
    private:

    };
}



#endif//KATE_ENGINE_FRAMEBUFFER_HH
