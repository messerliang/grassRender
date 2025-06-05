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
		Point position;							// λ�ã���λ��m
		float width;							// ��Ƭ�Ŀ�ȣ���λ��m
		float height;							// ��Ƭ�ĸ߶ȣ���λ��m
	};


	struct OffsetData {							// ʵ�����õ���ƫ������
		Point positionOffset;					// λ��ƫ��
		float yRotate;							// y����ת
		float heightScale;						// �߶�����
		float maxHeight;						// �߶�����
		Point cubicBezierP1;					// cubic���Ƶ�1
		Point cubicBezierP2;					// cubic���Ƶ�2
		Point cubicBezierP3;					// cubic���Ƶ�2
		Point randomDir;						// ������ķ���ʸ��
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
	GrassAttribute m_basicAttrib;			// �����ݵ�����
	// ���е�����
	std::vector<float> m_positions;			// ����ģ�͵Ķ�������
	std::vector<uint32_t> m_index;			// ����ģ�͵Ķ�������

	VertexBuffer* m_vertBufferPtr=NULL;			// ��������
	VertexArray* m_vertArrayPtr = NULL;
	IndexBuffer* m_idxBufferPtr = NULL;			// ������ indexbuffer



	// instance ���ݣ�ÿ��������ڻ������Ե�ƫ��
	uint32_t m_numOfInstanceX = 800;			// �����ϲݵ�����
	uint32_t m_numOfInstanceY = 800;			// �����ϲݵ�����
	std::vector<OffsetData> m_instancePosOffset;
	VertexBuffer* m_offsetVertBufferPtr = NULL;	// ��������

public:
	// �ݵصĳ��Ϳ�
	float m_groundWidth = 300.0f;					// �ݵصĿ�
	float m_groundLength = 300.0f;					// �ݵصĳ�

	BladeGrass(const glm::vec3& pos, const float width, const float height);

	~BladeGrass();

	// ����λ�á���Ⱥ͸߶����ɶ�Ӧ�ĵ㣬Ϊ�� offset ��bladegrass����15������ķ�ʽ���ɶ�Ӧ�Ķ��㣬
	void genPoints(GrassAttribute& grassAttrib, uint32_t offset=0);
	
	// ����λ��ƫ��
	void genInstances();
	// ����ͶӰ����
	void setUniform(Shader& shaderPtr, Camera& cam, GLFWwindow* window);
	// ���ƻ�����״
	void display(Shader& shaderPtr, Camera& cam, GLFWwindow* window);
	// ����ʵ��
	void displayInstances(Shader& shaderPtr, Camera& cam, GLFWwindow* window);


	// ���� instances ������
	int getInstancesNum() const;

	Point cubicBezier(float t, const Point& p0, const Point& p1, const Point& p2, const Point& p3);
	

};

