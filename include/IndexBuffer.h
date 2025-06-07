#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GL_Error.h"


class IndexBuffer
{
private:
	unsigned int m_id=0;
	unsigned int m_count=0;
public:
	//
	// ����1��index ����
	// ����2������������ٸ�Ԫ��
	//
	IndexBuffer(const unsigned int* index, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	// ���� IndexArray �ж��ٸ��±�Ԫ��
	unsigned int GetCount() const;

	// ���� Index ����
	void updatteData(const unsigned int* index, unsigned int count);
};

