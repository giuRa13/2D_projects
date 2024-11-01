#include "Rendering/Buffers/FrameBuffer.hpp"
#include "Rendering/Essentials/TextureLoader.hpp"
#include <Logger/Logger.hpp>


namespace ENGINE_RENDERING
{

    bool FrameBuffer::Initialize()
    {
        glCreateFramebuffers(1, &m_FboID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetID(), 0);

        if(m_bUseRbo)
        {
            glCreateRenderbuffers(1, &m_RboID);
            glBindRenderbuffer(GL_RENDERBUFFER, m_RboID);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_Width, m_Height);

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RboID);
        }

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            ENGINE_ASSERT(false && "Failed to create an OpenGL FrameBuffer");

            std::string error = std::to_string(glGetError());
            ENGINE_ERROR("Failed to create an OpenGL FrameBuffer: {}", error);
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return true;
    }


    FrameBuffer::FrameBuffer()
        : FrameBuffer(640, 480, false)
    { }
     
    FrameBuffer::FrameBuffer(int width, int height, bool bUseRbo)
        : m_FboID{0}, m_RboID{0}, m_pTexture{nullptr}, m_Width{width}, m_Height{height}
        ,  m_bShouldResize{false}, m_bUseRbo{bUseRbo}
    {
        m_pTexture = std::move(TextureLoader::Create(Texture::TextureType::FRAMEBUFFER, width, height));

        if(!m_pTexture || !Initialize())
        {
            ENGINE_ASSERT(false && "Failed to create FrameBuffer");
            ENGINE_ERROR("FrameBuffer creation failed");
        }
    }

    FrameBuffer::~FrameBuffer()
    {
        CleanUp();
    }

    void FrameBuffer::CleanUp()
    {
        glDeleteFramebuffers(1, &m_FboID);
        if(m_bUseRbo)
            glDeleteFramebuffers(1, &m_RboID);

        if(m_pTexture)
        {
            auto textureID = m_pTexture->GetID();
            glDeleteTextures(1, &textureID);
        }
    }

    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
    }

    void FrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void FrameBuffer::Resize(int width, int height)
    {
        m_Width = width;
        m_Height = height;
        m_bShouldResize = true;
    }


    void FrameBuffer::CheckResize()
    {
        if(!m_bShouldResize)
            return;

        CleanUp();
        m_pTexture.reset();
        m_pTexture = std::move(TextureLoader::Create(Texture::TextureType::FRAMEBUFFER, m_Width, m_Height));
        ENGINE_ASSERT( m_pTexture && "New Texture cannot be nullptr" );    

        Initialize();
        m_bShouldResize = false;
    }

}