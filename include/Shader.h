#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GL_Error.h"
#include "Texture.h"
#include "Camera.h"

class Shader
{
public:
	unsigned int m_shaderProgramId;

private:
	unsigned int m_vertexShaderId;
	unsigned int m_vertexType = GL_VERTEX_SHADER;
	std::string m_vertexShaderSource;

	unsigned int m_fragmentShaderId;
	unsigned int m_fragmentType = GL_FRAGMENT_SHADER;
	std::string m_fragmentSource;


	std::unordered_map<std::string, int> m_Cache;


public:
	Shader(const std::string& vertexCode, const std::string& fragmentCode);
	~Shader();

	void setView(Camera& cam, GLFWwindow* window);

	int GetUniformLocation(const std::string& name="u_Color");
	void SetUniform(const float r, const float g, const float b, const float a);
	void SetUniform1f(const float val, const std::string& name) const;
	void SetUniformV3(const glm::vec3 val, const std::string& name) const;
	void SetUniformV4(const glm::vec4 val, const std::string& name) const;
	void SetTexture(Texture& tex, unsigned int texId, const std::string& name) const;
	void Compile();
	void Use()const;
	void Unuse()const;

	void SetUniformMat4(const glm::mat4& trans, const std::string& uniformName)const;

private:
	std::string ParseDefaultShaderSource(const std::string& file);
	
	unsigned int CompileShader(unsigned int type, const std::string& source) const;
};

