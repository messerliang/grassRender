#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GL_Error.h"

#include <unordered_map>

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{


private:
	unsigned int m_id;
	VertexBuffer* m_vertexBuffer=nullptr;
	IndexBuffer* m_indexBuffer=nullptr;
	std::unordered_map<VertexBuffer*, int> m_vertexBufferChannelId;		//  vertexBuffer ����������ɫ���ϵ�ͨ����ʼ��λ��
	uint32_t m_existAttributesNum = 0;
public:



	// ��Ҫʹ�� vertexBuffer �� IndexBuffer ������ vertexArray
	VertexArray(VertexBuffer* vbObj, IndexBuffer* idObj);
	VertexArray(VertexBuffer* vbObj);
	VertexArray();

	~VertexArray();
	
	void Bind()const;
	void Unbind()const;

	// vertexarray���һ������
	void AddBuffer(VertexBuffer* vBuf, bool isInstance = false);
	// ����ĳ��ͨ���� vertex buffer ����
	//void updateBufferData(VertexBuffer* vBuf, int channelId);
	
	// ʹ�� index buffer ������ͼ��
	void DrawElement(Shader& shader) const;
	// ʹ�� index buffer ������ͼ��ʵ��
	void DrawElementsInstanced(Shader& shader, int instancesNum) const;
	
	// ���ƶ�������
	void DrawArray(Shader& shader) const;
	// ����ʵ��
	void DrawArraysInstanced(Shader& shader, int instancesNum) const;
	
	uint32_t getExistAttribNum() const;
};

