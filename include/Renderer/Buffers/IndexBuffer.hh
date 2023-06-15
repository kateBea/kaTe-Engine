//
// Created by kate on 6/4/23.
//

#ifndef KATE_ENGINE_INDEX_BUFFER_HH
#define KATE_ENGINE_INDEX_BUFFER_HH

#include <memory>

#include "Tools/Common.hh"

namespace kaTe {
    /**
     * General interface for Vertex index buffers
     * */
     class IndexBuffer {
     public:
         IndexBuffer() = default;
         virtual ~IndexBuffer() = default;

         virtual auto GetID() const -> UInt32_T { return m_Id; }
         virtual auto GetCount() const -> UInt32_T { return m_Count; }

         // Temporary for OpenGL IndexBuffer
         virtual auto BindBuffer() const -> void {}
         virtual auto UnbindBuffer() const -> void {}

         static auto CreateBuffer(const std::vector<UInt32_T>& data) -> std::shared_ptr<IndexBuffer>;
     protected:
         UInt32_T m_Id{};
         UInt32_T m_Count{};
     };
}

#endif//KATE_ENGINE_INDEX_BUFFER_HH
