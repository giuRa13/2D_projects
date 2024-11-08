#pragma once

#include <map>
#include <memory>
#include <string>
#include <Rendering/Essentials/Shader.hpp>
#include <Rendering/Essentials/Texture.hpp>
#include <Rendering/Essentials/Font.hpp>
#include <Core/ECS/Registry.hpp>
#include <Sounds/Essentials/Music.hpp>
#include <Sounds/Essentials/SoundFX.hpp>
#include <sol/sol.hpp>


namespace ENGINE_RESOURCES
{

    class AssetManager
	{

	private:
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Shader>> m_mapShaders{};
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Font>> m_mapFonts{};

		std::map<std::string, std::shared_ptr<ENGINE_SOUNDS::Music>> m_mapMusic{};
		std::map<std::string, std::shared_ptr<ENGINE_SOUNDS::SoundFX>> m_mapSoundFX{};
	
    
    public:
		AssetManager() = default;
		~AssetManager() = default;

		bool CreateDefaultFonts();

		bool AddTexture(const std::string& textureName, const std::string& TexturePath, bool pixelArt = true, bool bTileset = false);
		bool AddTextureFromMemory( const std::string& textureName, const unsigned char* imageData, size_t length,
							   bool pixelArt = true, bool bTileset = false );
		std::shared_ptr<ENGINE_RENDERING::Texture> GetTexture(const std::string& textureName);
		
        bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		bool AddShaderFromMemory(const std::string& shaderName, const char* vertexShader, const char* fragmentShader);
		std::shared_ptr<ENGINE_RENDERING::Shader> GetShader(const std::string& shaderName);

		bool AddFont(const std::string& fontName, const std::string& fontPath, float fontSize = 32.f);
		bool AddFontFromMemory(const std::string& fontName, unsigned char* fontData, float fontSize = 32.f);
		std::shared_ptr<ENGINE_RENDERING::Font> GetFont(const std::string& fontName);
		
		bool AddMusic(const std::string& musicName, const std::string& filepath);
		std::shared_ptr<ENGINE_SOUNDS::Music> GetMusic(const std::string& musicName);

		bool AddSoundFx(const std::string& soundFxName, const std::string& filepath);
		std::shared_ptr<ENGINE_SOUNDS::SoundFX> GetSoundFx(const std::string& soundFxName);

		std::vector<std::string> GetTilesetNames() const;
		
		inline const std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Texture>>& GetAllTextures() const { return m_mapTextures; };

		static void CreateLuaAssetManager(sol::state& lua);
	};

}