#include "Core/Resources/AssetManager.hpp"
#include <Rendering/Essentials/TextureLoader.hpp>
#include <Rendering/Essentials/ShaderLoader.hpp>
#include <Rendering/Essentials/FontLoader.hpp>
#include <Logger/Logger.hpp>
#include "Core/ECS/MainRegistry.hpp"
#include <EngineUtils/EngineUtilities.hpp>
#include "Core/Resources/default_fonts.hpp"



namespace ENGINE_RESOURCES
{
	bool AssetManager::CreateDefaultFonts()
	{
		if(!AddFontFromMemory("pixel", matrixtype)) //pixel_font
		{
			ENGINE_ERROR("Failed to create pixel font");
			return false;
		}
		return true;
	}

	std::vector<std::string> AssetManager::GetAssetKeyNames(ENGINE_UTIL::AssetType eAssetType) const
	{
		switch(eAssetType)
		{
		case ENGINE_UTIL::AssetType::TEXTURE: 
			return ENGINE_UTIL::GetKeys(m_mapTextures, 
				[](const auto& pair) { return !pair.second->IsEditorTexture(); });
		case ENGINE_UTIL::AssetType::FONT:
			return ENGINE_UTIL::GetKeys(m_mapFonts);
		case ENGINE_UTIL::AssetType::SOUNDFX:
			return ENGINE_UTIL::GetKeys(m_mapSoundFX);
		case ENGINE_UTIL::AssetType::MUSIC:
			return ENGINE_UTIL::GetKeys(m_mapMusic);
		default: ENGINE_ASSERT(false && "Cannot get this asset type");
		}

		return {};
	}

	std::vector<std::string> AssetManager::GetTilesetNames() const
	{
		return ENGINE_UTIL::GetKeys( m_mapTextures, []( const auto& pair ) { return pair.second->IsTileset(); } );
	}

	bool AssetManager::ChangeAssetName(const std::string& sOldName, const std::string& sNewName, ENGINE_UTIL::AssetType eAssetType)
	{
		switch(eAssetType)
		{
		case ENGINE_UTIL::AssetType::TEXTURE: 
			return ENGINE_UTIL::KeyChange(m_mapTextures, sOldName, sNewName);
		case ENGINE_UTIL::AssetType::FONT:
			return ENGINE_UTIL::KeyChange(m_mapFonts, sOldName, sNewName);
		case ENGINE_UTIL::AssetType::SOUNDFX:
			return ENGINE_UTIL::KeyChange(m_mapSoundFX, sOldName, sNewName);
		case ENGINE_UTIL::AssetType::MUSIC:
			return ENGINE_UTIL::KeyChange(m_mapMusic, sOldName, sNewName);
		default: ENGINE_ASSERT(false && "Cannot get this asset type");
		}

		return false;
	}

	bool AssetManager::CheckHasAsset(const std::string& sCheckName, ENGINE_UTIL::AssetType eAssetType)
	{
		switch(eAssetType)
		{
		case ENGINE_UTIL::AssetType::TEXTURE: 
			return m_mapTextures.contains(sCheckName);
		case ENGINE_UTIL::AssetType::FONT:
			return m_mapFonts.contains(sCheckName);
		case ENGINE_UTIL::AssetType::SOUNDFX:
			return m_mapSoundFX.contains(sCheckName);
		case ENGINE_UTIL::AssetType::MUSIC:
			return m_mapMusic.contains(sCheckName);
		default: ENGINE_ASSERT(false && "Cannot get this asset type");
		}

		return false;
	}

	bool AssetManager::DeleteAsset(const std::string& sAssetName, ENGINE_UTIL::AssetType eAssetType)
	{
		switch(eAssetType)
		{
		case ENGINE_UTIL::AssetType::TEXTURE: 
			return std::erase_if(m_mapTextures, 
				[&](const auto& pair) { return pair.first == sAssetName; }
			) > 0;
		case ENGINE_UTIL::AssetType::FONT:
			return std::erase_if(m_mapFonts, 
				[&](const auto& pair) { return pair.first == sAssetName; }
			) > 0;
		case ENGINE_UTIL::AssetType::SOUNDFX:
			return std::erase_if(m_mapSoundFX, 
				[&](const auto& pair) { return pair.first == sAssetName; }
			) > 0;
		case ENGINE_UTIL::AssetType::MUSIC:
			return std::erase_if(m_mapMusic, 
				[&](const auto& pair) { return pair.first == sAssetName; }
			) > 0;

		default: ENGINE_ASSERT(false && "Cannot get this asset type");
		}

		return false;
	}


	// Texture //////////////////////////
    bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt, bool bTileset)
	{
		if (m_mapTextures.find(textureName) != m_mapTextures.end()) 
		{
			ENGINE_ERROR("Failed to add Texture [{0}] -- Already exists", textureName);
			return false;
		}

		auto texture = ENGINE_RENDERING::TextureLoader::Create(
			pixelArt ? ENGINE_RENDERING::Texture::TextureType::PIXEL : ENGINE_RENDERING::Texture::TextureType::BLENDED,
			texturePath, bTileset);

		if (!texture)
		{
			ENGINE_ERROR("Failed to load Texture [{0}] at path [{1}]", textureName, texturePath);
			return false;
		}

		auto [ itr, bSuccess ] = m_mapTextures.emplace( textureName, std::move( texture ) );
		return bSuccess;
		//m_mapTextures.emplace(textureName, std::move(texture));
		//return true;
	}

    std::shared_ptr<ENGINE_RENDERING::Texture> AssetManager::GetTexture(const std::string& textureName)
	{
		auto texItr = m_mapTextures.find(textureName);

		if (texItr == m_mapTextures.end())
		{
			ENGINE_ERROR("Failed to add Texture [{0}] -- does not exists", textureName);
			return nullptr;
		}

		return texItr->second;//Dereferece Itr and get the second that is the Shared_Pointer
	}

	bool AssetManager::AddTextureFromMemory( const std::string& textureName, const unsigned char* imageData, size_t length,
											bool pixelArt, bool bTileset )
	{
		// Check to see if the Texture already exist
		if ( m_mapTextures.contains( textureName ) )
		{
			ENGINE_ERROR( "AssetManager: Texture [{}] -- Already exists!", textureName );
			return false;
		}

		auto texture =
			std::move( ENGINE_RENDERING::TextureLoader::CreateFromMemory( imageData, length, pixelArt, bTileset ) );

		// Load the texture
		if ( !texture )
		{
			ENGINE_ERROR( "Unable to load texture [{}] from memory!", textureName );
			return false;
		}

		// Insert the texture into the map
		auto [ itr, bSuccess ] = m_mapTextures.emplace( textureName, std::move( texture ) );
		return bSuccess;
	}

	// Shaders //////////////////////////
    bool AssetManager::AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
	{
		if (m_mapShaders.find(shaderName) != m_mapShaders.end()) 
		{
			ENGINE_ERROR("Failed to add Shader [{0}] -- Already exists", shaderName);
			return false;
		}

		auto shader = std::move(ENGINE_RENDERING::ShaderLoader::Create(vertexPath, fragmentPath));
		if (!shader)
		{
			ENGINE_ERROR("Failed to load Shader [{0}] at  vert path [{1}] and frag path [{2}]", shaderName, vertexPath, fragmentPath);
			return false;
		}

		auto [ itr, bSuccess ] = m_mapShaders.insert( std::make_pair( shaderName, std::move( shader ) ) );
		return bSuccess;
		//m_mapShaders.emplace(shaderName, std::move(shader));
		//return true;
	}

	bool AssetManager::AddShaderFromMemory(const std::string& shaderName, const char* vertexShader, const char* fragShader)
    {
        if (m_mapShaders.find(shaderName) != m_mapShaders.end())
        {
            ENGINE_ERROR("Failed to add shader - [{0}] -- Already exists!", shaderName);
            return false;
        }
        auto shader = std::move(ENGINE_RENDERING::ShaderLoader::CreateFromMemory(vertexShader, fragShader));
        m_mapShaders.insert(std::make_pair(shaderName, std::move(shader)));
        return true;
    }

    std::shared_ptr<ENGINE_RENDERING::Shader> AssetManager::GetShader(const std::string& shaderName)
	{
		auto shaderItr = m_mapShaders.find(shaderName);

		if (shaderItr == m_mapShaders.end())
		{
			ENGINE_ERROR("Failed to add Shader [{0}] -- does not exists", shaderName);
			ENGINE_RENDERING::Shader shader{};
			return nullptr;
		}
        
		return shaderItr->second;
	}


	// Font //////////////////////////
	bool AssetManager::AddFont(const std::string& fontName, const std::string& fontPath, float fontSize)
	{
		if ( m_mapFonts.contains( fontName ) )
		{
			ENGINE_ERROR( "Failed to add font [{0}] -- already exists", fontName );
			return false;
		}

		auto pFont = ENGINE_RENDERING::FontLoader::Create( fontPath, fontSize );

		if ( !pFont )
		{
			ENGINE_ERROR( "Failed to add Font [{}] at path [{}] -- to the Asset Manager", fontName, fontPath );
			return false;
		}

		//m_mapFonts.emplace(fontName, std::move(pFont));
		//return true;
		auto [ itr, bSuccess ] = m_mapFonts.emplace( fontName, std::move( pFont ) );
		return bSuccess;
	}

	std::shared_ptr<ENGINE_RENDERING::Font> AssetManager::GetFont(const std::string& fontName)
	{
		auto fontItr = m_mapFonts.find(fontName);

		if (fontItr == m_mapFonts.end())
		{
			ENGINE_ERROR("Failed to add Font [{0}] -- does not exists", fontName);
			return nullptr;
		}

		return fontItr->second;
	}

	bool AssetManager::AddFontFromMemory(const std::string& fontName, unsigned char* fontData, float fontSize)
	{
		if ( m_mapFonts.contains( fontName ) )
		{
			ENGINE_ERROR( "Failed to add font [{0}] -- already exists", fontName );
			return false;
		}

		auto pFont = ENGINE_RENDERING::FontLoader::CreateFromMemory( fontData, fontSize );

		if ( !pFont )
		{
			ENGINE_ERROR( "Failed to add Font [{}] from memory -- to the Asset Manager", fontName);
			return false;
		}

		auto [ itr, bSuccess ] = m_mapFonts.emplace( fontName, std::move( pFont ) );
	 	return bSuccess;
		//m_mapFonts.emplace(fontName, std::move(pFont));
		//return true;
	}


	// Sound //////////////////////////
	bool AssetManager::AddMusic(const std::string& musicName, const std::string& filepath)
	{
		//if(m_mapMusic.find(musicName) != m_mapMusic.end())
		if ( m_mapMusic.contains( musicName ) )
		{
			ENGINE_ERROR("Failed to add music [{}] -- Already exists", musicName);
			return false;
		}

		Mix_Music* music = Mix_LoadMUS(filepath.c_str());

		if(!music)
		{
			std::string error{Mix_GetError()};
			ENGINE_ERROR("Failed to load [{}] at path [{}] -- Mixer_Error: {}", musicName, filepath, error);
			return false;
		}

		ENGINE_SOUNDS::SoundParams params{
			.name = musicName,
			.filename = filepath,
			.duration = Mix_MusicDuration(music)
		};
		
		auto musicPtr = std::make_shared<ENGINE_SOUNDS::Music>(params, MusicPtr{ music});
		if(!musicPtr)
		{
			ENGINE_ERROR("Failed to create the music ptr for [{}]", musicName);
			return false;
		}

		//m_mapMusic.emplace(musicName, std::move(musicPtr));
		//return true;
		auto [ itr, bSuccess ] = m_mapMusic.emplace( musicName, std::move( musicPtr ) );
		return bSuccess;
	}

	std::shared_ptr<ENGINE_SOUNDS::Music> AssetManager::GetMusic(const std::string& musicName)
	{
		auto musicItr = m_mapMusic.find(musicName);
		if(musicItr == m_mapMusic.end())
		{
			ENGINE_ERROR("Failed to get [{}] -- Does not exists", musicName);
			return nullptr;
		}

		return musicItr->second;
	}


	bool AssetManager::AddSoundFx(const std::string& soundFxName, const std::string& filepath)
	{
		//if(m_mapSoundFX.find(soundFxName) != m_mapSoundFX.end())
		if ( m_mapSoundFX.contains( soundFxName ) )
		{
			ENGINE_ERROR("Failed to add SoundFX [{}] -- Already exists", soundFxName);
			return false;
		}

		Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str()); 

		if(!chunk)
		{
			std::string error{Mix_GetError()};
			ENGINE_ERROR("Failed to load [{}] at path [{}] -- Mixer_Error: {}", soundFxName, filepath, error);
			return false;
		}

		ENGINE_SOUNDS::SoundParams params{
			.name = soundFxName,
			.filename = filepath,
			.duration = chunk->alen / 179.4
		};

		auto pSoundFx = std::make_shared<ENGINE_SOUNDS::SoundFX>(params, SoundFxPtr{ chunk });
		auto [ itr, bSuccess ] = m_mapSoundFX.emplace( soundFxName, std::move( pSoundFx ) );
		return bSuccess;
		//m_mapSoundFX.emplace(soundFxName, std::move(pSoundFx));
		//return true;
	}

	std::shared_ptr<ENGINE_SOUNDS::SoundFX> AssetManager::GetSoundFx(const std::string& soundFxName)
	{
		auto soundItr = m_mapSoundFX.find(soundFxName);
		if(soundItr == m_mapSoundFX.end())
		{
			ENGINE_ERROR("Failed to get SoundFX [{}] -- Does not exists", soundFxName);
			return nullptr;
		}

		return soundItr->second;
	}


	// Lua /////////////////
	void AssetManager::CreateLuaAssetManager(sol::state& lua)
	{
		auto& mainRegistry = MAIN_REGISTRY();
		auto& asset_manager = mainRegistry.GetAssetManager();
		//auto& asset_manager = registry.GetContext<std::shared_ptr<AssetManager>>();
		/*if(!asset_manager)
		{
			ENGINE_ERROR("Failed tp bind the asset manager to Lua - Does not exists in the registry");
			return;
		}*/

		lua.new_usertype<AssetManager>(
			"AssetManager",
			sol::no_constructor,
			"add_texture", 
			sol::overload(
			[&](const std::string& assetName, const std::string& filepath, bool pixel_art)
			{
				return asset_manager.AddTexture(assetName, filepath, pixel_art, false);
			},
			[&](const std::string& assetName, const std::string& filepath, bool pixel_art, bool bTileset)
			{
				return asset_manager.AddTexture(assetName, filepath, pixel_art, bTileset);
			}),
			/*[&](const std::string& assetName, const std::string& filepath, bool pixel_art)
			{
				return asset_manager.AddTexture(assetName, filepath, pixel_art);
			},*/
			"add_music", 
			[&](const std::string& musicName,const std::string& filepath){
				return asset_manager.AddMusic( musicName, filepath );
			},
			"add_soundfx",
			[&](const std::string& soundFxName, const std::string& filepath)
			{
				return asset_manager.AddSoundFx(soundFxName, filepath);
			},
			"add_font",
			[&] (const std::string& fontName, const std::string& fontPath, float fontSize)
			{
				return asset_manager.AddFont(fontName, fontPath, fontSize);
			}
		);
	}

}