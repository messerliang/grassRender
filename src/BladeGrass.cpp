#include "BladeGrass.h"

BladeGrass::BladeGrass(const glm::vec3& pos, const float width, const float height):
	m_basicAttrib({pos, width, height})
{
    genPoints(m_basicAttrib);
    // 各个 instance

    genInstances();



    // 为基本属性生成 vertexbuffer 
    m_vertBufferPtr = new VertexBuffer(m_positions.data(), m_positions.size() * sizeof(m_positions[0]));
    checkPointer(m_vertBufferPtr, "BladeGrass VB create failed\n");
    m_vertBufferPtr->Push<float>(3); // 三个位置信息

    // 为属性偏移生成数据
    m_offsetVertBufferPtr = new VertexBuffer(m_instancePosOffset.data(), m_instancePosOffset.size() * sizeof(m_instancePosOffset[0]));
    checkPointer(m_offsetVertBufferPtr, "BladeGrass posOffset VB create failed\n");
    m_offsetVertBufferPtr->Push<float>(3);  // 位置偏移
    m_offsetVertBufferPtr->Push<float>(1);  // 旋转
    m_offsetVertBufferPtr->Push<float>(1);  // 高度缩放因子
    m_offsetVertBufferPtr->Push<float>(1);  // 缩放后的最大高度
    m_offsetVertBufferPtr->Push<float>(3);  // cubicBezier曲线控制点1
    m_offsetVertBufferPtr->Push<float>(3);  // cubicBezier曲线控制点2
    m_offsetVertBufferPtr->Push<float>(3);  // cubicBezier曲线控制点3
    m_offsetVertBufferPtr->Push<float>(3);  // 随机动的方向

    // 生成 indexbuffer
    m_idxBufferPtr = new IndexBuffer(m_index.data(), m_index.size());
    checkPointer(m_idxBufferPtr, "BladeGrass IB create failed\n");


    // 生成 vertexArray
    m_vertArrayPtr = new VertexArray(*m_vertBufferPtr, *m_idxBufferPtr);
    checkPointer(m_vertArrayPtr, "BladeGrass VA create failed\n");
    m_vertArrayPtr->AddBuffer(*m_offsetVertBufferPtr, true);
}

BladeGrass::~BladeGrass() {
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

void BladeGrass::genPoints(GrassAttribute& grassAttrib, uint32_t offset) {

    float halfWidth = grassAttrib.width / 2;
   
    // 
    float offsetX = halfWidth;
    float offsetY = grassAttrib.height / 7;
    std::vector<uint32_t> firstSegIdx = { 0, 1, 2, 1, 2, 3 };

    // 15 个顶点
    for (int i = 0; i < 8; i++) {
        float heightPercent = i / 7.0;
        float x = (1 - heightPercent) * halfWidth;
        float h = std::pow(heightPercent, 0.5) * grassAttrib.height;
        if (7  ==  i) {
            m_positions.insert(m_positions.end(), 
                { grassAttrib.position.x, grassAttrib.position.y + grassAttrib.height, grassAttrib.position.z });
            
            continue;
        }
        //else if (6 == i) {
        //    halfWidth = halfWidth * 0.6667f;
        //}
        m_positions.insert(m_positions.end(),{ grassAttrib.position.x - x, grassAttrib.position.y + h, grassAttrib.position.z});
        m_positions.insert(m_positions.end(),{ grassAttrib.position.x + x, grassAttrib.position.y + h, grassAttrib.position.z});


    }
    // 插入顶点索引，一共 13 个三角形， 
    for (int i = 0; i < 7; ++i) {
        if (i < 6) {
            for (auto& id : firstSegIdx) {
                m_index.push_back(i * 2 + id + 15 * offset);
            }
        }
        else {
            for (int j = 0; j < 3; ++j) {
                m_index.push_back(2 * i + firstSegIdx[j] + 15 * offset);
            }
        }
    }

    
}


void BladeGrass::genInstances() {

    uint32_t cnt = 0;
    glm::vec3 posOffset;
    float stepX = m_groundWidth / m_numOfInstanceX;
    float stepY = m_groundLength / m_numOfInstanceY;
    for (int x = 0; x < m_numOfInstanceX; ++x) {
        for (int y = 0; y < m_numOfInstanceY; ++y) {

            float offset1 = uniformFloatDist(-0.1f, 0.1f);
            float offset2 = uniformFloatDist(-0.1f, 0.1f);
            posOffset = glm::vec3(x*stepX - m_groundWidth/2 + offset1, 0.0f, y * stepY - m_groundLength/2 + offset2);

            float angle = uniformFloatDist(0.0f, 6.28f);
            float heightScale = uniformFloatDist(0.2f, 3.1f);
            float maxHeight = heightScale * m_basicAttrib.height;

            float p3Z = uniformFloatDist(0.2f, 0.45f);
            float p1Y = uniformFloatDist(0.5f, 1.2f);
            float p1Z = uniformFloatDist(0.1f * p1Y , 0.2f * p1Y);

            float p2Y = uniformFloatDist(1.0f, 1.2f);
            float p2Z = uniformFloatDist(-0.1f , 0.8f * p3Z);

            Point p1(0.0f, p1Y * maxHeight, p1Z * maxHeight);
            Point p2(0.0f, p2Y * maxHeight, p2Z * maxHeight);
            Point p3(0.0f, maxHeight, p3Z * maxHeight);

            Point randDir = Point(1.0f,0.0f, 0.0f);

            m_instancePosOffset.emplace_back(posOffset, angle , heightScale, maxHeight, p1, p2, p3, randDir);
            cnt++;
        }
    }
}
void BladeGrass::setUniform(Shader& shader, Camera& cam, GLFWwindow* window) {
    shader.Use();

    glm::mat4 model(1.0f);
    glm::mat4 view = cam.getView();
    glm::mat4 projection = cam.getProjection(window);

    shader.SetUniformMat4(model, "model");
    shader.SetUniformMat4(view, "view");
    shader.SetUniformMat4(projection, "projection");
}

void BladeGrass::display(Shader& shader, Camera& cam, GLFWwindow* window) {

    setUniform(shader, cam, window);
    m_vertArrayPtr->DrawElement(shader);
}

void BladeGrass::displayInstances(Shader& shader, Camera& cam, GLFWwindow* window) {
    setUniform(shader, cam, window);
    m_vertArrayPtr->DrawElementsInstanced(shader, getInstancesNum());
}

int BladeGrass::getInstancesNum() const {
    return m_numOfInstanceX * m_numOfInstanceY;
}
glm::vec3 BladeGrass::cubicBezier(float t, const Point& p0, const Point& p1, const Point& p2, const Point& p3) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Point p;
    p.x = uuu * p0.x            // (1 - t)^3 * P0
        + 3 * uu * t * p1.x     // 3(1 - t)^2 * t * P1
        + 3 * u * tt * p2.x     // 3(1 - t) * t^2 * P2
        + ttt * p3.x;           // t^3 * P3

    p.y = uuu * p0.y
        + 3 * uu * t * p1.y
        + 3 * u * tt * p2.y
        + ttt * p3.y;

    return p;
}