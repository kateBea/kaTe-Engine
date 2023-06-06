#ifndef KATE_ENGINE_OPEN_GL_RENDERER_API_HH
#define KATE_ENGINE_OPEN_GL_RENDERER_API_HH

#include "RendererAPI.hh"

namespace kaTe {
	class OpenGLRenderer : public RendererAPI {
	public:
		auto setClearColor(/**const glm::vec4& color*/) -> void override;
		// auto setClearColor(UInt16_T r, UInt16_T g, UInt16_T b, UInt16_T a) -> void override;
		auto clear(BufferBit bufferBits) -> void override;

		auto drawIndexed(/* With possibles overloads: mesh, VertexBuffer & IndexBuffer*/) -> void override;
		// RendererAPI::draw() possible overloads
		// 
		// virtual auto draw(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) -> void override;
		// virtual auto draw(const Shader& shader, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) -> void override;
		// There's no reason to draw a mesh indexed as ioit may probably have it's own indices

		auto draw(/* With possibles overloads: mesh, VertexBuffer*/) -> void override;

		// RendererAPI::draw() possible overloads
		// 
		// auto draw(const Mesh& mesh) -> void override;
		// auto draw(const VertexBuffer& vertexBuffer) -> void override;

		// auto draw(const Shader& shader, const Mesh& mesh) -> void override;
		// auto draw(const Shader& shader, const VertexBuffer& vertexBuffer) -> void override;

	private:
		/**
		 * See: https://learnopengl.com/Getting-started/Hello-Triangle
		 * A vertex array object (also known as VAO) can be bound just like a vertex buffer 
		 * object and any subsequent vertex attribute calls from that point on will be stored 
		 * inside the VAO. This has the advantage that when configuring vertex attribute pointers 
		 * you only have to make those calls once and whenever we want to draw the object, we can just 
		 * bind the corresponding VAO. This makes switching between different vertex data and attribute 
		 * configurations as easy as binding a different VAO. All the state we just set is stored inside the VAO.
		 *
		 * Currently we are using CORE_OPENGL_PROFILE which is specified when we create an OpenGL context
		 * with the OpenGLContext class therefore OpenGL requires that we use a VAO so it knows what to do with 
		 * our vertex inputs. If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.
		 * 
		 * In compatibility mode, OpenGL already offers a default Vertex Array.
		 * See: https://www.khronos.org/opengl/wiki/Vertex_Specification
		 * 
		 * There's no need to have multiple VAO's really, we can just use a single one and setup the attributes properly before 
		 * a draw call with glEnableVertexAttribArray() && glVertexAttribPointer()
		 * */
		
		 // OpenVertexArray m_VertexArray{};

	};
}

#endif