#if 1

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Background.h"
#include "BladeGrass.h"

// 根据距离，生成对应的 lod
int getLOD(float distance, int tileWidth) {
	float normDistance = distance / tileWidth;
	if (normDistance >= 20) {
		return 1;
	}
	else if (normDistance < 2) {
		return 15;
	}
	else {
		int result = 20 - normDistance;
		return (result < 15) ? result : 15;
	}
}

int main() {
	std::cout << "grassSimulation\n";
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return -1;
	}
	// 配置使用的版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "grass scene", nullptr, nullptr);
	if (!window) {
		std::cerr << "window create failed\n";
		glfwTerminate();
		return -1;
	}

	camera.bindCallbackToWindow(window);

	// 讲指定窗口的上下文设置为当前线程的上下文
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;


	// 初始化 GLEW（必须在上下文创建后）
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n";
		return -1;
	}


	// 先准备一个背景
	Background* sky = new Background("shader/sky/sky.vert", "shader/sky/sky.frag");



	Shader* bgShader = new Shader("shader/background/bg.vert", "shader/background/bg.frag");
	Shader* grassShaderPtr = new Shader("shader/grass.vert", "shader/grass.frag");

	BladeGrass* grassPtr = new BladeGrass(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, 1.5f, 10);
	std::vector<BladeGrass*> grasses;
	std::vector<glm::vec3> grassTilePosition;
	float grassTileWidth = 5.0f, grassTileLength = 5.0f;
	int N = 50;
	for (int i = 0; i < N*N; ++i) {
		glm::vec3 offset(((i % N) - N / 2) * grassTileWidth, 0.0f, ((i / N) - N / 2) * grassTileLength);
		float distance = glm::distance(camera.getPosition(), offset);
		grassTilePosition.emplace_back(offset);
		int lod = getLOD(distance, grassTileWidth);

		BladeGrass* grass = new BladeGrass(glm::vec3(0.0f), 0.2f, 2.0f, lod, grassTileWidth, grassTileLength);
		grasses.push_back(grass);
		
		
		
		//std::cout << distance <<", lod: "<< lod <<", " << offset.x << " " << offset.z << std::endl;
	}
	
	// 草地
	float groundWidth = N * grassTileWidth / 2;
	float groundLength = N * grassTileLength / 2;
	float ground[] = {
		// positions          // normals           // texture coords
		 groundWidth,  0.0f, groundLength,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 groundWidth,  0.0f,-groundLength,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		-groundWidth,  0.0f,-groundLength,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-groundWidth,  0.0f, groundLength,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,

	};
	unsigned int groundIdx[] = {
		0, 1, 2,
		0, 2, 3,
	};

	VertexBuffer* vbPtr = new VertexBuffer(ground, sizeof(ground));
	vbPtr->Push<float>(3, false);
	vbPtr->Push<float>(3, false);
	vbPtr->Push<float>(2, false);

	IndexBuffer* ibPtr = new IndexBuffer(groundIdx, sizeof(groundIdx) / sizeof(groundIdx[0]));

	VertexArray* vaPtr = new VertexArray(vbPtr, ibPtr);

	// 风的方向
	glm::vec3 windDir(0.0f, 0.0f, 1.0f);

	// model、view、projection三件套
	glm::mat4 model(1.0f);
	glm::mat4 view = camera.getView();
	glm::mat4 projection = camera.getProjection(window);


	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		camera.updateCamera(window);

		// render here
		GLCall(glClearColor(0.1f, 0.1f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		
		// 绘制背景
		glDisable(GL_DEPTH_TEST);
		
		sky->Display(window);
		sky->setTime(glfwGetTime(), "iTime");

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
		// 对 shader 进行设置前，都必须要先激活
		bgShader->Use();
		bgShader->setView(camera, window);
		vaPtr->DrawElement(*bgShader);
		
		// 绘制草
		glm::vec3 camPos = camera.getPosition();
		grassShaderPtr->Use();
		grassShaderPtr->SetUniform1f(glfwGetTime(), "iTime");
		grassShaderPtr->SetUniformV3(windDir, "windDir");
		grassShaderPtr->SetUniform1f(grassPtr->m_groundWidth*N, "grassWidth");
		grassShaderPtr->SetUniform1f(grassPtr->m_groundLength*N, "grassLength");
		int numOfSeg = 20;
		for (int i = 0; i < N * N; i++) {
			glm::vec3 tilePos = grassTilePosition[i];
			
			float distance = glm::distance(tilePos, glm::vec3(camPos.x, 0.0f, camPos.z));
			numOfSeg = getLOD(distance, grassTileWidth);
			//std::cout<< i << ": distance: " << distance << ", seg: " << numOfSeg << "\n";
			grassShaderPtr->SetUniformV3(grassTilePosition[i], "grassTilePosition");
			grasses[i]->updateSegment(numOfSeg);
			grasses[i]->displayInstances(*grassShaderPtr, camera, window);
		}
		//grassPtr->displayInstances(*grassShaderPtr, camera, window);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	delete(vaPtr);
	delete(vbPtr);
	delete(bgShader);
	delete(grassShaderPtr);
	delete(grassPtr);
	delete(sky);


	glfwDestroyWindow(window);
	glfwTerminate();

}

#endif // 1