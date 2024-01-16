#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
namespace ALStore{
class Shader
	{
	public:
		Shader(const std::string& filepath);
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void Bind() const;
		void Unbind() const;
	private:
		std::string ReadFile(const std::string& filepath);
		std::string GetShaderInfoLog(GLuint shader);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources); 
	
	private:
		uint32_t m_RendererID;
		std::string m_Name; std::string logs;
	};
}