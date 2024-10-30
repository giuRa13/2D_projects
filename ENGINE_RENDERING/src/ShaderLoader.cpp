#include "Rendering/Essentials/ShaderLoader.hpp"
#include <iostream>
#include <fstream>
#include <Logger/Logger.hpp>


namespace ENGINE_RENDERING
{

    GLuint ShaderLoader::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		const GLuint program = glCreateProgram();
		const GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
		const GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		
		if (vertex == 0 || fragment == 0)
			return 0;
		
		if (!LinkShaders(program, vertex, fragment))
		{
			//std::cout << "Failed to link Shaders" << std::endl;
			ENGINE_ERROR("Failed to Link Shaders!");
			return 0;
		}
		
		return program;
	}

    GLuint ShaderLoader::CompileShader(GLuint shaderType, const std::string& filepath)
	{
		const GLuint shaderID = glCreateShader(shaderType);
		
		std::ifstream ifs(filepath);
		if (ifs.fail())
		{
			//std::cout << "Shader failed to open [" <<filepath <<"]" << std::endl;
			ENGINE_ERROR("Shader Failed to open [{}]", filepath);
			return 0;
		}
		
		std::string contents{ "" };
		std::string line;
		while (std::getline(ifs, line))
		{
			contents += line + "\n";
		}
		ifs.close();
		
		const char* contentsPtr = contents.c_str();
		glShaderSource(shaderID, 1, &contentsPtr, nullptr);
		glCompileShader(shaderID);
		
		if (!CompileSuccess(shaderID))
		{
			//std::cout << "Failed to compile shader [" << filepath << "]" << std::endl;
			ENGINE_ERROR("Failed to compile shader [{}]", filepath);
			return 0;
		}

		return shaderID;
	}


	GLuint ShaderLoader::CreateProgram(const char* vertexShader, const char* fragmentShader)
    {
        const GLuint program = glCreateProgram();
        const GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
        const GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
        if (vertex == 0 || fragment == 0)
            return 0;
        if (!LinkShaders(program, vertex, fragment))
        {
            ENGINE_ERROR("Failed to Link Shaders!");
            return 0;
        }
        return program;
    }


    GLuint ShaderLoader::CompileShader(GLuint type, const char* shader)
    {
        const GLuint id = glCreateShader(type);
        glShaderSource(id, 1, &shader, nullptr);
        glCompileShader(id);
        // Check to see if the shader compiled correctly
        if (!CompileSuccess(id))
        {
            ENGINE_ERROR("Failed to compile shader from memory!");
            return 0;
        }
        return id;
    }


	bool ShaderLoader::CompileSuccess(GLuint shader)
	{
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		
		if(status != GL_TRUE)
		{
			GLint maxLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			
			std::string errorLog(maxLength, ' ');
			glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());
			//std::cout << "Shader Compilation Failed: " << std::string(errorLog) << std::endl;
			ENGINE_ERROR("Shader Compilation failed: {}", std::string{errorLog});

			glDeleteShader(shader);
			return false;
		}

		return true;
	}


    bool ShaderLoader::IsProgramValid(GLuint program)
	{
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status != GL_TRUE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			
			std::string errorLog(maxLength, ' ');
			glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());
			//std::cout << "Shaders Failed to Link: " << std::string(errorLog) << std::endl;
			ENGINE_ERROR("Shader's Program failed to link: {}", std::string{errorLog});
			return false;
		}
		
		return true;
	}

    bool ShaderLoader::LinkShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		
		if (!IsProgramValid(program))
		{
			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return false;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return true;
	}

	std::shared_ptr<Shader> ShaderLoader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		GLuint program = CreateProgram(vertexShaderPath, fragmentShaderPath);

		if (program)
			return std::make_shared<Shader>(program, vertexShaderPath, fragmentShaderPath);
		
		return nullptr;
	}


	std::shared_ptr<Shader> ShaderLoader::CreateFromMemory(const char* vertexShader, const char* fragmentShader)
    {
        GLuint program = CreateProgram(vertexShader, fragmentShader);
        // From memory holds onto the shader itself, not the path
        if (program)
            return std::make_shared<Shader>(program, vertexShader, fragmentShader);
        return nullptr;
    }
    
	
	bool ShaderLoader::Destroy(Shader* pShader)
    {
        if (pShader->ShaderProgramID() <= 0)
            return false;
        glDeleteShader(pShader->ShaderProgramID());
        return true;
    }

}