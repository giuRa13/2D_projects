#pragma once

#include <map>
#include <memory>
#include <string>
#include <Rendering/Essentials/Shader.hpp>
#include <Rendering/Essentials/Texture.hpp>
#include <Core/ECS/Registry.hpp>
#include <Sounds/Essentials/Music.hpp>
#include <sol/sol.hpp>


namespace ENGINE_RESOURCES
{

    class AssetManager
	{

	private:
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Shader>> m_mapShaders{};

		std::map<std::string, std::shared_ptr<ENGINE_SOUNDS::Music>> m_mapMusic{};
	
    
    public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string& textureName, const std::string& TexturePath, bool pixelArt = true);
		std::shared_ptr<ENGINE_RENDERING::Texture> GetTexture(const std::string& textureName);
		
        bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		std::shared_ptr<ENGINE_RENDERING::Shader> GetShader(const std::string& shaderName);

		bool AddMusic(const std::string& musicName, const std::string& filepath);
		std::shared_ptr<ENGINE_SOUNDS::Music> GetMusic(const std::string& musicName);

		static void CreateLuaAssetManager(sol::state& lua, ENGINE_CORE::ECS::Registry& registry);
	};

}