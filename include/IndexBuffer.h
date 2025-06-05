#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GL_Error.h"


class IndexBuffer
{
private:
	unsigned int m_id;
	unsigned int m_count;
public:
	//
	// 参数1：index 数组
	// 参数2：数组包含多少个元素
	//

	IndexBuffer(const unsigned int* index, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
};

