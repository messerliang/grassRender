#include "Background.h"


Background::Background(const std::string& vertexCode, const std::string& fragmentCode) {
	m_shaderPtr = new Shader(vertexCode, fragmentCode);
	if (!m_shaderPtr) {
		std::cout << "background shader create failed\n";
		exit(-1);
	}
	m_vertBufferPtr = new VertexBuffer(m_positions, sizeof(m_positions));
	if (!m_vertBufferPtr) {
		std::cout << "background vertexbuffer create failed\n";
		exit(-1);
	}
	m_vertBufferPtr->Push<float>(3);

	m_idxBufferPtr = new IndexBuffer(m_index, sizeof(m_index) / sizeof(m_index[0]));
	
	m_vertArrayPtr = new VertexArray(*m_vertBufferPtr, *m_idxBufferPtr);
	if (!m_vertArrayPtr) {
		std::cout << "background m_vertArrayPtr create failed\n";
		exit(-1);
	}
}

Background::~Background() {
	if (m_shaderPtr) {
		delete(m_shaderPtr);
		m_shaderPtr = nullptr;
	}
	if (m_idxBufferPtr) {
		delete(m_idxBufferPtr);
		m_idxBufferPtr = nullptr;
	}
	if (m_vertBufferPtr) {
		delete(m_vertBufferPtr);
		m_vertBufferPtr = nullptr;
	}
	if (m_vertArrayPtr) {
		delete(m_vertArrayPtr);
		m_vertArrayPtr = nullptr;
	}
}

void Background::Display(GLFWwindow* window) {
	if (!m_shaderPtr) {
		std::cout << "no shader in background\n" << std::endl;
		exit(-1);
	}
	m_shaderPtr->Use();
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// 设置一时间
	m_shaderPtr->SetUniform1f(glfwGetTime(), "iTime");
	// 设置像素
	glm::vec3 resolution = glm::vec3(width, height, 1.0);
	m_shaderPtr->SetUniformV3(resolution, "iResolution");
	// 设置鼠标位置
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec4 iMouse = glm::vec4(xpos, ypos, 1.0f, 1.0f);
	m_shaderPtr->SetUniformV4(iMouse, "iMouse");

	m_vertArrayPtr->DrawElement(*m_shaderPtr);
}

void Background::setTime(float t, const std::string name) {
	m_shaderPtr->Use();
	m_shaderPtr->SetUniform1f(t, name);
}