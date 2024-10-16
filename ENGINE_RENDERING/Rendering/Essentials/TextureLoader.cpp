#include "TextureLoader.hpp"
#include <SOIL/SOIL.h>
#include <Logger/Logger.hpp>


namespace ENGINE_RENDERING
{
	bool TextureLoader::LoadTexture(const std::string& filepath, GLuint& id, int& width, int& height, bool blended)
	{
        int channels = 0;

        const std::string& err = SOIL_last_result();

        unsigned char* image = SOIL_load_image(
            filepath.c_str(),
            &width, &height,
            &channels,
            SOIL_LOAD_AUTO
        );

        if (!image)
        {
            ENGINE_ERROR("SOIL Failed to load image [{0} -- {1}]",  filepath, err);
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

}