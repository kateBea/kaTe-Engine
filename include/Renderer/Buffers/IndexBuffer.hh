//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_INDEX_BUFFER_HH
#define KATE_ENGINE_INDEX_BUFFER_HH

#include "Tools/Common.hh"

namespace kaTe {
    /**
     * General interface for Vertex index buffers
     * */
     class IndexBuffer {
     public:
         IndexBuffer() = default;
         virtual ~IndexBuffer() = default;

         virtual auto getRenderId() const -> UInt32_T { return m_Id; }
         virtual auto getCount() const -> UInt32_T { return m_Count; }

         // Temporary for OpenGL IndexBuffer
         virtual auto bindIndexBuffer() const -> void {}
         virtual auto unbindIndexBuffer() const -> void {}

         static auto createBuffer(const std::vector<UInt32_T>& data) -> IndexBuffer*;
     protected:
         UInt32_T m_Id{};
         UInt32_T m_Count{};
     };
}

#endif//KATE_ENGINE_INDEX_BUFFER_HH
