#pragma once

#include <map>
#include <memory>
#include <string>
#include <Rendering/Essentials/Shader.hpp>
#include <Rendering/Essentials/Texture.hpp>



namespace ENGINE_RESOURCES
{
	class AssetManager
	{

	private:
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<ENGINE_RENDERING::Shader>> m_mapShaders{};


	public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string& textureName, const std::string& TexturePath, bool pixelArt = true);
		const ENGINE_RENDERING::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		ENGINE_RENDERING::Shader& GetShader(const std::string& shaderName);
	};

}