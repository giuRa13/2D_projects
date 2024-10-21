#pragma once

#include "Texture.hpp"
#include <memory>


namespace ENGINE_RENDERING
{
	class TextureLoader
	{

	private:
		static bool LoadTexture(const std::string& filepath, GLuint& id, int& width, int& height, bool blended = false);
	
    
    public:
		TextureLoader() = delete;
        
		static std::shared_ptr<Texture> Create(Texture::TextureType type, const std::string& texturePath);
	
    };

}