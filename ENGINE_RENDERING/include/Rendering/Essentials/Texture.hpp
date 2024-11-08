#pragma once

#include <glad/glad.h>
#include <string>


namespace ENGINE_RENDERING
{

	class Texture
	{

    public:
		enum class TextureType{PIXEL = 0, BLENDED, FRAMEBUFFER, NONE};
    
    private:
		GLuint m_TextureID;
		int m_Width, m_Height;
		std::string m_sPath;
		TextureType m_eType;
		bool m_bTileset;
		//bool m_bEditorTexture;
	
    
    public:
		Texture();
		Texture(GLuint id, int width, int height, TextureType type = TextureType::PIXEL, 
			const std::string& texturePath = "", bool bTileset = false);
		~Texture() = default;
		
        inline const int& GetWidth() const { return m_Width; } // & only for return value with Logger
		inline const int& GetHeight() const { return m_Height; }
		inline const GLuint GetID() const { return m_TextureID; }

		inline const bool IsTileset() const { return m_bTileset; }
		inline void SetIsTileset(bool bIsTileset) { m_bTileset = bIsTileset; }

		//inline const bool IsEditorTexture() const { return m_bEditorTexture; }
		//inline void SetIsEditorTexture(bool bIsEditorTexture) { m_bEditorTexture = bIsEditorTexture; }
		
        void Bind();
		void Unbind();
    };

}