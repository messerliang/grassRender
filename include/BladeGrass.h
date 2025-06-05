#pragma once

#include <vector>
#include <random>

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "functions.h"

class BladeGrass
{
	using Point = glm::vec3;
	struct GrassAttribute {
		Point position;							// 位置，单位：m
		float width;							// 草片的宽度，单位：m
		float height;							// 草片的高度，单位：m
	};


	struct OffsetData {							// 实例化用到的偏移数据
		Point positionOffset;					// 位置偏移
		float yRotate;							// y轴旋转
		float heightScale;						// 高度缩放
		float maxHeight;						// 高度缩放
		Point cubicBezierP1;					// cubic控制点1
		Point cubicBezierP2;					// cubic控制点2
		Point cubicBezierP3;					// cubic控制点2
		Point randomDir;						// 随机动的方向矢量
		OffsetData(Point& pos, const float& f, const float& h, float& mh, 
			Point& p1, Point& p2, Point& p3, Point& dir) :
			positionOffset(pos), yRotate(f), heightScale(h) , maxHeight(mh),
			cubicBezierP1(p1), cubicBezierP2(p2), cubicBezierP3(p3), randomDir(dir)
		{}
		OffsetData() :
			positionOffset(0.0f), yRotate(0.0f), heightScale(1.0f),
			cubicBezierP1(0.0f), cubicBezierP2(0.0f), cubicBezierP3(0.0f),
			randomDir(1.0f, 0.0f, 0.0f)
		{}
	};
private:			
	GrassAttribute m_basicAttrib;			// 基本草的属性
	// 所有的数据
	std::vector<float> m_positions;			// 基本模型的顶点数据
	std::vector<uint32_t> m_index;			// 基本模型的顶点索引

	VertexBuffer* m_vertBufferPtr=NULL;			// 基本顶点
	VertexArray* m_vertArrayPtr = NULL;
	IndexBuffer* m_idxBufferPtr = NULL;			// 基本的 indexbuffer



	// instance 数据，每个草相对于基本属性的偏移
	uint32_t m_numOfInstanceX = 800;			// 横轴上草的数量
	uint32_t m_numOfInstanceY = 800;			// 纵轴上草的数量
	std::vector<OffsetData> m_instancePosOffset;
	VertexBuffer* m_offsetVertBufferPtr = NULL;	// 基本顶点

public:
	// 草地的长和宽
	float m_groundWidth = 300.0f;					// 草地的宽
	float m_groundLength = 300.0f;					// 草地的长

	BladeGrass(const glm::vec3& pos, const float width, const float height);

	~BladeGrass();

	// 根据位置、宽度和高度生成对应的点，为第 offset 个bladegrass按照15个顶点的方式生成对应的顶点，
	void genPoints(GrassAttribute& grassAttrib, uint32_t offset=0);
	
	// 生成位置偏移
	void genInstances();
	// 设置投影矩阵
	void setUniform(Shader& shaderPtr, Camera& cam, GLFWwindow* window);
	// 绘制基础形状
	void display(Shader& shaderPtr, Camera& cam, GLFWwindow* window);
	// 绘制实例
	void displayInstances(Shader& shaderPtr, Camera& cam, GLFWwindow* window);


	// 返回 instances 的数量
	int getInstancesNum() const;

	Point cubicBezier(float t, const Point& p0, const Point& p1, const Point& p2, const Point& p3);
	

};

