#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * position;
	Normal = mat3(transpose(inverse(model))) * normal;	// 如果物体使用了不等比例的缩放，为了保证法向量不异常，我们需要为法向量设置对应的操作

	FragPos = vec3(model * position); // 这里是使用 model 矩阵，计算出物体在世界坐标的实际位置
	TexCoords = texCoords;
};
