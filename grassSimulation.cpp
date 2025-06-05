#if 1

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Background.h"
#include "BladeGrass.h"


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
	GLFWwindow* window = glfwCreateWindow(800, 600, "light scene", nullptr, nullptr);
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

	// 草地
	float ground[] = {
		// positions          // normals           // texture coords
		 10.0f,  0.0f, 10.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 10.0f,  0.0f,-10.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		-10.0f,  0.0f,-10.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-10.0f,  0.0f, 10.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		
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

	VertexArray* vaPtr = new VertexArray(*vbPtr, *ibPtr);

	Shader* bgShader = new Shader("shader/background/bg.vert", "shader/background/bg.frag");
	Shader* grassShaderPtr = new Shader("shader/grass.vert", "shader/grass.frag");

	BladeGrass* grassPtr = new BladeGrass(glm::vec3(0.0f, 0.0f, 0.0f), 0.2f, 2.0f);
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
		grassShaderPtr->Use();
		grassShaderPtr->SetUniform1f(glfwGetTime(), "iTime");
		grassShaderPtr->SetUniform1f(grassPtr->m_groundWidth, "grassWidth");
		grassShaderPtr->SetUniform1f(grassPtr->m_groundLength, "grassLength");

		grassPtr->displayInstances(*grassShaderPtr, camera, window);


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