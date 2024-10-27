#include "Rendering/Essentials/FontLoader.hpp"
#include "Rendering/Essentials/Font.hpp"
#include <fstream>
#include <vector>
#include <Logger/Logger.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>



namespace ENGINE_RENDERING
{

    std::shared_ptr<class Font> FontLoader::Create(const std::string& fontPath, float fontSize, int width, int height)
    {
        std::ifstream fontStream{fontPath, std::ios::binary};

        if( fontStream.fail())
        {
            ENGINE_ERROR("Failed to load font [{}] - Unable to read buffer", fontPath);
            return nullptr;
        }

        fontStream.seekg(0, fontStream.end);
        int64_t lenght = fontStream.tellg();
        fontStream.seekg(0, fontStream.beg);

        std::vector<unsigned char> buffer;
        buffer.resize(lenght);
        std::vector<unsigned char> bitmap;
        bitmap.resize(width * height);
        fontStream.read((char*)(&buffer[0]), lenght);

        auto data = (void*) new stbtt_bakedchar[96];

        stbtt_BakeFontBitmap(buffer.data(), 0, fontSize, bitmap.data(), width, height, 32, 96, (stbtt_bakedchar*)(data));

        GLuint fontId;
        glGenTextures(1, &fontId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return std::make_shared<Font>(fontId, width, height, fontSize, data);
    }

    /*std::shared_ptr<class Font> FontLoader::CreateFromMemory(const unsigned char* fontData, float fontSize, int width, int height)
    {

    }*/

}
