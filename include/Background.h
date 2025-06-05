#pragma once

#include "Shader.h"
#include "VertexArray.h"

class Background
{
private:
	Shader* m_shaderPtr;
	VertexBuffer* m_vertBufferPtr;
	IndexBuffer* m_idxBufferPtr;
	VertexArray* m_vertArrayPtr;
public:
	float m_positions[12] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
	};
	unsigned int m_index[6] = {
		0, 1, 2,
		0, 2, 3
	};
	
	Background(const std::string& vertexCode, const std::string& fragmentCode);
	Background(const Background& other) = delete;
	Background& operator=(const Background&) = delete;
	~Background();

	void Display(GLFWwindow* window);
	void setTime(float t, const std::string name);
};

