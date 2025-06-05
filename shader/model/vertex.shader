#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;


out vec4 color;
out vec2 TexCoord;

//uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * position;

	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	TexCoord = texCoord;
};
