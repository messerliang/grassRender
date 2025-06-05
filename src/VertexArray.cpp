#include "VertexArray.h"

VertexArray::VertexArray(const VertexBuffer& vbObj, const IndexBuffer& idObj):m_vertexBuffer(vbObj),m_indexBuffer(idObj) {
	GLCall(glGenVertexArrays(1, &m_id));
	GLCall(glBindVertexArray(m_id));
	AddBuffer(m_vertexBuffer);
}

VertexArray::VertexArray(const VertexBuffer& vbObj):m_vertexBuffer(vbObj){
	
	GLCall(glGenVertexArrays(1, &m_id));
	GLCall(glBindVertexArray(m_id));
	AddBuffer(m_vertexBuffer);
}

VertexArray::VertexArray(){
	
	GLCall(glGenVertexArrays(1, &m_id));
	GLCall(glBindVertexArray(m_id));
}



VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::Bind() const{
	GLCall(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

uint32_t VertexArray::getExistAttribNum() const {
	return m_existAttributesNum;
}

void VertexArray::AddBuffer(const VertexBuffer& vBuf, bool isInstance) {
	if (!m_vertexBuffer.getId()) {
		m_vertexBuffer = vBuf;
	}
	const std::vector<LayoutElement> layoutSets = vBuf.GetLayout();
	unsigned int offset = 0;
	this->Bind();
	vBuf.Bind();
	for (int i = 0; i < layoutSets.size(); ++i) {
		const auto& it = layoutSets[i];
		int channelId = m_existAttributesNum;
		GLCall(glEnableVertexAttribArray(channelId));
		// ����1������id
		// ����2���������ٸ�����
		// ����3����������
		// ����4���Ƿ��׼��
		// ����5��stride
		// ����6����ǰ������ vertexbuffer ��ƫ�ƣ���λΪ byte
		GLCall(glVertexAttribPointer(channelId, it.count, it.type, it.isNormalized, vBuf.GetStride(), (void*)offset));
		offset += it.size;
		m_existAttributesNum++;

		// ��ǰ�� vertexbuffer �����ǲ��Ƕ������ݵ� instance
		if (isInstance) {
			GLCall(glVertexAttribDivisor(channelId, 1));
		}

	}

	this->Unbind();
	vBuf.Unbind();
}

void VertexArray::DrawElement(Shader& shader) const {
	shader.Use();
	Bind();
	this->m_indexBuffer.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
void VertexArray::DrawElementsInstanced(Shader& shader, int instancesNum) const {
	shader.Use();
	Bind();
	this->m_indexBuffer.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr, instancesNum));
}

void VertexArray::DrawArray(Shader& shader) const {
	shader.Use();
	Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, m_vertexBuffer.getVertexNum()));
}
void VertexArray::DrawArraysInstanced(Shader& shader, int instancesNum) const {
	shader.Use();
	Bind();
	GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertexBuffer.getVertexNum(), instancesNum));
}
