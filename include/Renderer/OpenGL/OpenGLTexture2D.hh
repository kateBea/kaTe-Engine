//
// Created by kate on 6/8/23.
//

#ifndef KATE_ENGINE_OPENGL_TEXTURE2D_HH
#define KATE_ENGINE_OPENGL_TEXTURE2D_HH

#include <stb_image.h>

#include <Tools/Common.hh>
#include <Renderer/Material/Texture2D.hh>

namespace kaTe {
    class OpenGLTexture2D : public Texture2D {
    public:
        /**
         * Creates a new Texture object and fills it with the data
         * from Texture file in path. If no data is provided it simply creates
         * a valid Texture object with a valid id
         * @param path the path to the Texture file
         * */
        explicit OpenGLTexture2D(const Path_T& path, bool retainFileData = false);

        /**
         * Move constructor
         * @param other move from Texture
         * */
        OpenGLTexture2D(OpenGLTexture2D&& other) noexcept;

        /**
         * Move assignment
         * @return *this
         * */
        auto operator=(OpenGLTexture2D&& other) noexcept -> OpenGLTexture2D&;

        /**
         * Returns the identifier of this Texture
         * @return id of this object
         * */
        [[nodiscard]]
        auto getId() const -> UInt32_T { return m_Id; }

        auto getChannels() const -> UInt32_T { return m_Channels; }
        auto getWidth() -> UInt32_T override { return m_Width; }
        auto getHeight() -> UInt32_T override { return m_Height; }

        auto getTextureFileData() -> std::vector<UInt8_T> { return m_TextureFileData; }

        auto bind(UInt32_T slot = 0) -> void override;

        ~OpenGLTexture2D() override { glDeleteTextures(1, &m_Id); }
    private:
        // Forbidden operations
        OpenGLTexture2D(const OpenGLTexture2D& other) = delete;
        auto operator=(const OpenGLTexture2D& other) -> OpenGLTexture2D& = delete;

    private:
        // Helper because on windows std::filesystem uses wchar_t
        static auto getByteChar(const Path_T& path) -> std::string;
        auto setupTexture(const stbi_uc* data) -> void;

        UInt32_T    m_Id{};
        UInt32_T    m_Height{};
        UInt32_T    m_Width{};
        UInt32_T    m_Channels{};

        bool m_RetainData{};
        std::vector<UInt8_T> m_TextureFileData;
    };
}


#endif//KATE_ENGINE_OPENGL_TEXTURE2D_HH
