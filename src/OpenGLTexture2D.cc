//
// Created by kate on 6/8/23.
//

#include <GL/glew.h>

#include <stb_image.h>

#include <Core/Logger.hh>
#include <Core/Assert.hh>
#include <Tools/Common.hh>

#include <Renderer/OpenGL/OpenGLTexture2D.hh>

namespace kaTe {

    OpenGLTexture2D::OpenGLTexture2D(const Path_T &path, bool retainFileData)
        :   m_RetainData{ retainFileData }
    {
        // STB image expects width and height and channel to be signed integers
        Int32_T width{}, height{}, channels{};

        auto fileDir{ getByteChar(path) };
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* imageData{ stbi_load(fileDir.c_str(), &width, &height, &channels, 4) };

        if (imageData) {
            m_Width = width;
            m_Height = height;
            m_Channels = channels;
            glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
            setupTexture(imageData);

            // free the data if we do not want to keep it
            if (!m_RetainData)
                stbi_image_free(imageData);
        }
        else {
            throw std::runtime_error("Could not load Texture data");
        }

    }

    auto OpenGLTexture2D::bind(UInt32_T slot) -> void {
        glBindTextureUnit(slot, m_Id);
    }

    auto OpenGLTexture2D::operator=(OpenGLTexture2D &&other) noexcept -> OpenGLTexture2D& {
        m_Id        = other.getId();
        m_Width     = other.getWidth();
        m_Height    = other.getHeight();
        m_Channels  = other.getChannels();
        m_TextureFileData = std::move(other.m_TextureFileData);

        other.m_Id          = 0;
        other.m_Width       = 0;
        other.m_Height      = 0;
        other.m_Channels    = 0;

        return *this;
    }

    OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D &&other) noexcept
        :   m_Id{ other.getId() }, m_Width{ other.getWidth() }, m_Height{ other.getHeight() }
        ,   m_Channels{ other.getChannels() }, m_TextureFileData{ std::move(other.m_TextureFileData) }
    {
        other.m_Id          = 0;
        other.m_Width       = 0;
        other.m_Height      = 0;
        other.m_Channels    = 0;
    }

    auto OpenGLTexture2D::setupTexture(const stbi_uc* data) -> void {
        // Specifies the sized internal format to be used to store texture image data
        GLenum internalFormat{};
        // Specifies the format of the pixel data
        GLenum dataFormat{};

        switch (m_Channels) {
            case 3:
                dataFormat = GL_RGB;
                internalFormat = GL_RGB8;
                break;
            case 4:
                dataFormat = GL_RGBA;
                internalFormat = GL_RGBA8;
                break;
        }
        KT_ASSERT(dataFormat & internalFormat, "Texture data format unsupported");

        glTextureStorage2D(m_Id, 1, internalFormat, m_Width, m_Height);

        // Mipmap
        glGenerateTextureMipmap(m_Id);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
    }

    auto OpenGLTexture2D::getByteChar(const Path_T &path) -> std::string {
        std::string fileDir(4096, '\0');
#ifdef defined(_WIN32) || defined(_WIN64)
        // fileDir.size() will return the amount of elements of fileDir, since it contains char which are byte sized
        wcstombs_s(nullptr, fileDir.data(), fileDir.size(), path.c_str(), 4096);
#else
        std::copy(path.native().begin(), path.native().end(), fileDir.begin());
#endif

        return fileDir;
    }
}