#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Texture
{
public:

private:
	unsigned int m_textureId;
	std::string m_imagePath;
	int m_width;
	int m_height;
	int m_channel;
	unsigned char* m_imageData = nullptr;

	// 读取数据，默认垂直翻转
	int ReadImage(int flip);

public:

	Texture(const std::string& imgPath,  int flipY=111);
	Texture(const Texture& other):
		m_textureId(other.m_textureId),
		m_imagePath(other.m_imagePath),
		m_width(other.m_width),
		m_height(other.m_height),
		m_channel(other.m_channel)
	{
		if (!other.m_imageData) {
			uint32_t size = m_width * m_height * m_channel;
			m_imageData = new unsigned char[size];
			std::copy(other.m_imageData, other.m_imageData + size, m_imageData);
		}
		else {
			m_imageData = nullptr;
		}
	}
	Texture& operator=(const Texture& other) {
		if (this == &other) {
			return *this;
		}
		if (m_imageData) {
			free(m_imageData);
		}
		m_textureId = other.m_textureId;
		m_imagePath = other.m_imagePath;
		m_width = other.m_width;
		m_height = other.m_height;
		m_channel = other.m_channel;
		uint32_t size = m_width * m_height * m_channel;
		m_imageData = new unsigned char[size];
		std::copy(other.m_imageData, other.m_imageData + size, m_imageData);
		return *this;
	}
	~Texture();

	void BasicSet();
	unsigned int GetId() const;

	void Bind2D() const;
	void UnBind2D() const;
	void SetImagData2D() const;


};

