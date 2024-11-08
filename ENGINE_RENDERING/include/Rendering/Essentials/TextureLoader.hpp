#pragma once

#include "Texture.hpp"
#include <memory>


namespace ENGINE_RENDERING
{
	class TextureLoader
	{

	private:
		static bool LoadTexture(const std::string& filepath, GLuint& id, int& width, int& height, bool blended = false);
		static bool LoadFBTexture( GLuint& id, int& width, int& height );
		static bool LoadTextureFromMemory( const unsigned char* imageData, size_t length, GLuint& id, int& width,
			int& height, bool blended = false );
	
		//bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height);
		//bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height);
    
    public:
		TextureLoader() = delete;
        
		static std::shared_ptr<Texture> Create(Texture::TextureType type, const std::string& texturePath, bool bTileset = false);
		static std::shared_ptr<Texture> Create(Texture::TextureType type, int width, int height, bool bTileset = false);
		static std::shared_ptr<Texture> CreateFromMemory(const unsigned char* imageData, size_t lenght, bool blended = false, bool bTileset = false);
	
    };

}