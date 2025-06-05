#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GL_Error.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{


private:
	unsigned int m_id;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	uint32_t m_existAttributesNum = 0;
public:



	// 需要使用 vertexBuffer 和 IndexBuffer 来创建 vertexArray
	VertexArray(const VertexBuffer& vbObj, const IndexBuffer& idObj);
	VertexArray(const VertexBuffer& vbObj);
	VertexArray();

	~VertexArray();
	
	void Bind()const;
	void Unbind()const;

	// vertexarray添加一个属性
	void AddBuffer(const VertexBuffer& vBuf, bool isInstance = false);

	void DrawElement(Shader& shader) const;
	void DrawElementsInstanced(Shader& shader, int instancesNum) const;
	
	void DrawArray(Shader& shader) const;
	void DrawArraysInstanced(Shader& shader, int instancesNum) const;
	
	uint32_t getExistAttribNum() const;
};

