#if 0


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
	// ����ʹ�õİ汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// ��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "light scene", nullptr, nullptr);
	if (!window) {
		std::cerr << "window create failed\n";
		glfwTerminate();
		return -1;
	}

	camera.bindCallbackToWindow(window);

	// ��ָ�����ڵ�����������Ϊ��ǰ�̵߳�������
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;


	// ��ʼ�� GLEW�������������Ĵ�����
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n";
		return -1;
	}




	// ��׼��һ������
	Background* sky = new Background("shader/sky/sky.vert", "shader/sky/sky.frag");

	// �ݵ�
	float ground[] = {

		// positions         // // normals           // texture coords
		//-0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		// 0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		// 0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		// 0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		//-0.5f,  0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		//-0.5f, -0.5f, -0.5f, // 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		// Positions	//	// Colors
		-0.5f,  0.5f, 0.0f, // 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, // 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, // 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, // 0.0f, 1.0f, 1.0f
	};



	unsigned int groundIdx[] = {
		0, 1, 2,
		0, 1, 3,
	};


	Shader* grassShaderPtr = new Shader("shader/grass.vert", "shader/grass.frag");


	VertexBuffer* vbPtr = new VertexBuffer(ground, sizeof(ground));
	vbPtr->Push<float>(3);

	IndexBuffer* ibPtr = new IndexBuffer(groundIdx, sizeof(groundIdx) / sizeof(groundIdx[0]));

	VertexArray* vaPtr = new VertexArray(*vbPtr, *ibPtr);


	glm::vec2 translations[4];
	int index = 0;
	GLfloat offset = .5f;
	int step = 2;
	for (GLint y = 0; y < 2 * step; y += step)
	{
		for (GLint x = 0; x < 2 * step; x += step)
		{
			glm::vec2 translation;
			translation.x = (GLfloat)x  + offset;
			translation.y = (GLfloat)y  + offset;

			translations[index++] = translation;
		}
	}
	VertexBuffer* vbOffset = new VertexBuffer(&translations[0], sizeof(translations));
	vbOffset->Push<float>(2);



	vaPtr->AddBuffer(*vbOffset, true);
	
	//vaPtr->Bind();
	//int channelId = vaPtr->getExistAttribNum();
	//vbPtr->Bind();
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//channelId++;
	//vbOffset->Bind();
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glVertexAttribDivisor(1, 1); // Tell OpenGL this is an instanced vertex attribute.
	//
	//vaPtr->Unbind();



	// model��view��projection������
	glm::mat4 model(1.0f);
	glm::mat4 view = camera.getView();
	glm::mat4 projection = camera.getProjection(window);


	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		camera.updateCamera(window);

		// render here
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// ���Ʊ���
		glDisable(GL_DEPTH_TEST);
		sky->Display(window);

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
		// �� shader ��������ǰ��������Ҫ�ȼ���
		grassShaderPtr->Use();
		grassShaderPtr->setView(camera, window);

		//vaPtr->DrawArray(*grassShaderPtr);
		//vaPtr->DrawArraysInstanced(*grassShaderPtr, 4);
		vaPtr->DrawElementsInstanced(*grassShaderPtr, 4);

		//vaPtr->Bind();
		//GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 4));

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	delete(vaPtr);
	delete(vbPtr);

	delete(grassShaderPtr);

	delete(sky);


	glfwDestroyWindow(window);
	glfwTerminate();

}

#endif // 1