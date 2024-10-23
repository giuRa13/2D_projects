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

		auto texture = std::move(ENGINE_RENDERING::TextureLoader::Create(
			pixelArt ? ENGINE_RENDERING::Texture::TextureType::PIXEL : ENGINE_RENDERING::Texture::TextureType::BLENDED,
			texturePath
		));

		if (!texture)
		{
			ENGINE_ERROR("Failed to load Texture [{0}] at path [{1}]", textureName, texturePath);
			return false;
		}

		m_mapTextures.emplace(textureName, std::move(texture));
		return true;
	}

    const ENGINE_RENDERING::Texture& AssetManager::GetTexture(const std::string& textureName)
	{
		auto texItr = m_mapTextures.find(textureName);

		if (texItr == m_mapTextures.end())
		{
			ENGINE_ERROR("Failed to add Texture [{0}] -- does not exists", textureName);
			return ENGINE_RENDERING::Texture();
		}

		return *texItr->second;//Dereferece Itr and get the second that is the Shared_Pointer
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

    ENGINE_RENDERING::Shader& AssetManager::GetShader(const std::string& shaderName)
	{
		auto shaderItr = m_mapShaders.find(shaderName);

		if (shaderItr == m_mapShaders.end())
		{
			ENGINE_ERROR("Failed to add Shader [{0}] -- does not exists", shaderName);
			ENGINE_RENDERING::Shader shader{};
			return shader;
		}
        
		return *shaderItr->second;
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
			"add_texture", [&](const std::string& assetName, const std::string& filepath, bool pixel_art)
			{
				return asset_manager->AddTexture(assetName, filepath, pixel_art);
			}
		);
	}

}