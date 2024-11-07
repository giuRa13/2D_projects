#include <Rendering/Essentials/TextureLoader.hpp>
#include <glad/glad.h>
#include <SOIL2/SOIL2.h>
#include <Logger/Logger.hpp>
#include <string>

/*#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <cstdlib>
#include "imgui.h"*/



namespace ENGINE_RENDERING
{

    bool TextureLoader::LoadTexture(const std::string& filepath, GLuint& ID, int& width, int& height, bool blended)
    {
        int channels = 0;

        unsigned char* image = SOIL_load_image(
            filepath.c_str(),
            &width, &height,
            &channels,
            SOIL_LOAD_AUTO
        );

        if (!image)
        {
            ENGINE_ERROR("SOIL Failed to load image [{0}] -- {1}", filepath, SOIL_last_result());
            return false;
        }

        GLint format = GL_RGBA;

        switch (channels)
        {
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (!blended)
        {
            //more pixelated
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            //more smooth (blend pixel toghether)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        glTexImage2D(
            GL_TEXTURE_2D,
            0,               // level of detail (0=base image level)
            format,          // internal format ( number of color components)
            width, height,
            0,               // border
            format,          // format of the pixel data
            GL_UNSIGNED_BYTE,
            image
        );

        SOIL_free_image_data(image);

        return true;
    }


    bool TextureLoader::LoadFBTexture(GLuint& id, int& width, int& height)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return true;
    }


    bool TextureLoader::LoadTextureFromMemory( const unsigned char* imageData, size_t length, GLuint& id, int& width, int& height, bool blended )
    {
	    id = SOIL_load_OGL_texture_from_memory( imageData, length, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, NULL );

	    if ( id == 0 )
        {
            ENGINE_ERROR( "Failed to load texture from memory" );
            return false;
        }

        // Get the width and height data from the texture
        glBindTexture( GL_TEXTURE_2D, id );
        glad_glGetTextureLevelParameteriv( id, 0, GL_TEXTURE_WIDTH, &width );
        glad_glGetTextureLevelParameteriv( id, 0, GL_TEXTURE_HEIGHT, &height);

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        if ( !blended )
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        }
        else
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        }

        return true;
    }


    std::shared_ptr<Texture> TextureLoader::Create(Texture::TextureType type, const std::string& texturePath)
	{
        GLuint id;
        int width, height;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        switch (type)
        {
        case Texture::TextureType::PIXEL:
            LoadTexture(texturePath, id, width, height, false);
            break;
        case Texture::TextureType::BLENDED :
            LoadTexture(texturePath, id, width, height, true);
            break;
        default:
            assert(false && "The current type is not defined, please use a defined texture type");
            return nullptr;
        }

        return std::make_shared<Texture>(id, width, height, type, texturePath);
	}


    std::shared_ptr<Texture> TextureLoader::Create(Texture::TextureType type, int width, int height)
    {
        ENGINE_ASSERT(type == Texture::TextureType::FRAMEBUFFER && "Must be FrameBuffer type");
        if(type != Texture::TextureType::FRAMEBUFFER)
        {
            ENGINE_ERROR( "Failed to create texture from FrameBuffer, incorrect type passed in" );
            return nullptr;   
        }

        GLuint id;
        glGenTextures(1, &id);
        LoadFBTexture(id, width, height);

        return std::make_shared<Texture>(id, width, height, type);
    }


    std::shared_ptr<Texture> TextureLoader::CreateFromMemory( const unsigned char* imageData, size_t length, bool blended)
    {
        GLuint id;
        int width, height;

        LoadTextureFromMemory( imageData, length, id, width, height, blended );

        return std::make_shared<Texture>( id, width, height, blended ? 
            Texture::TextureType::BLENDED : Texture::TextureType::PIXEL, "" );
    }


    ////////////////////////////
    // Simple helper function to load an image into a OpenGL texture with common settings
    /*bool TextureLoader::LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
    {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;
    }

    // Open and read a file, then forward to LoadTextureFromMemory()
    bool TextureLoader::LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
    {
        FILE* f = fopen(file_name, "rb");
        if (f == NULL)
            return false;
        fseek(f, 0, SEEK_END);
        size_t file_size = (size_t)ftell(f);
        if (file_size == -1)
            return false;
        fseek(f, 0, SEEK_SET);
        void* file_data = IM_ALLOC(file_size);
        fread(file_data, 1, file_size, f);
        bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
        IM_FREE(file_data);
        return ret;
    }*/

}