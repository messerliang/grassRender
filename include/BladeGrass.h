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
	uint32_t m_numOfInstanceX = 20;			// �����ϲݵ�����
	uint32_t m_numOfInstanceY = 20;			// �����ϲݵ�����
	std::vector<OffsetData> m_instancePosOffset;
	VertexBuffer* m_offsetVertBufferPtr = NULL;	// ��������

public:
	// 
	float m_numOfSegment=10;						// �ݵĶ������� 1~ 7
	// �ݵصĳ��Ϳ�
	float m_groundWidth = 3.0f;						// �ݵصĿ�
	float m_groundLength = 3.0f;					// �ݵصĳ�

	// �������1�� �ݵ�����λ��
	// �������2�������ݵĿ��
	// �������3�������ݵı�׼�߶�
	// �������4���ݵĶ���
	// �������5���ݵؿ��
	// �������6���ݵس���
	BladeGrass(const glm::vec3& pos, const float width, const float height, int numOfSeg=10, float w=5.0f, float l=5.0f);

	// �������1�������ݵĿ��
	// �������2�������ݵı�׼�߶�
	// �������3����ȷ���ݵ�����
	// �������4�����ȷ���ݵ�����
	// �������5���ݵĶ���
	// �������6���ݵؿ��
	// �������7���ݵس���
	BladeGrass(float width, float height, int numWidth, int numLength, int numOfSeg=10, float w=5.0f, float l=5.0f);



	~BladeGrass();

	// ���ݹ��캯����ɺ�Ĳ��������г�ʼ��
	void initialize();
	
	
	// ����λ�á���Ⱥ͸߶����ɶ�Ӧ�ĵ㣬Ϊ�� offset ��bladegrass���ն�Ӧ����ķ�ʽ���ɶ�Ӧ�Ķ��㣬
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
	
	// �޸� LOD ʱ�����¼��� vertex buffer �� index buffer
	void updateSegment(int numOfSeg);

	Point cubicBezier(float t, const Point& p0, const Point& p1, const Point& p2, const Point& p3);
	

};

