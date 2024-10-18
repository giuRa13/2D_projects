#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ENGINE_RENDERING
{
	class Shader
	{
	private:
		GLuint m_ShaderProgramID;
		std::string m_sVertexPath, m_sFragmentPath;

		std::unordered_map<std::string, GLuint> m_UniformLocationMap;

		GLuint GetUniformLocation(const std::string& uniformName);


	public:
		Shader();
		Shader(GLuint program, const std::string vertexPath, const std::string& fragmentPath);
		
		~Shader();

		void SetUniformInt(const std::string& name, int value);


		void SetUniformMat4(const std::string& name, const glm::mat4& mat);

		void Enable() const;
		void Disable() const;

		inline const GLuint ShaderProgramID() const { return m_ShaderProgramID; }
	};
}
		