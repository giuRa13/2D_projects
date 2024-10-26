#include "Core/Resources/AssetManager.hpp"
#include <Rendering/Essentials/TextureLoader.hpp>
#include <Rendering/Essentials/ShaderLoader.hpp>
#include <Logger/Logger.hpp>



namespace ENGINE_RESOURCES
{

    bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt)
	{
		if (m_mapTextures.find(textureName) != m_mapTextures.end()) //check if already exists
		{
			ENGINE_ERROR("Failed to add Texture [{0}] -- Already exists", textureName);
			return false;
		}

		auto texture = ENGINE_RENDERING::TextureLoader::Create(
			pixelArt ? ENGINE_RENDERING::Texture::TextureType::PIXEL : ENGINE_RENDERING::Texture::TextureType::BLENDED,
			texturePath
		);

		if (!texture)
		{
			ENGINE_ERROR("Failed to load Texture [{0}] at path [{1}]", textureName, texturePath);
			return false;
		}

		m_mapTextures.emplace(textureName, std::move(texture));
		return true;
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

		m_mapShaders.emplace(shaderName, std::move(shader));
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


	bool AssetManager::AddMusic(const std::string& musicName, const std::string& filepath)
	{
		if(m_mapMusic.find(musicName) != m_mapMusic.end())
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

		m_mapMusic.emplace(musicName, std::move(musicPtr));
		return true;
		//auto [ itr, bSuccess ] = m_mapMusic.emplace( musicName, std::move( musicPtr ) );
		//return bSuccess;
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
		if(m_mapSoundFX.find(soundFxName) != m_mapSoundFX.end())
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
		m_mapSoundFX.emplace(soundFxName, std::move(pSoundFx));
		return true;
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


	void AssetManager::CreateLuaAssetManager(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
	{
		auto& asset_manager = registry.GetContext<std::shared_ptr<AssetManager>>();
		if(!asset_manager)
		{
			ENGINE_ERROR("Failed tp bind the asset manager to Lua - Does not exists in the registry");
			return;
		}

		lua.new_usertype<AssetManager>(
			"AssetManager",
			sol::no_constructor,
			"add_texture", 
			[&](const std::string& assetName, const std::string& filepath, bool pixel_art)
			{
				return asset_manager->AddTexture(assetName, filepath, pixel_art);
			},
			"add_music", 
			[&](const std::string& musicName, const std::string& filepath)
			{
				return asset_manager->AddMusic(musicName, filepath);
			},
			"add_soundfx",
			[&](const std::string& soundFxName, const std::string& filepath)
			{
				return asset_manager->AddSoundFx(soundFxName, filepath);
			}
		);
	}

}