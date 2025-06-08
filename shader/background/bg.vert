#version 330 core

layout(location=0) in vec4 position;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform float groundWidth;
uniform float groundLength;



out vec2 fragCoords;                                    // 归一化后的坐标

void main() {
    

	
	vec2 uv = vec2(position.x / groundWidth, position.z / groundLength);

	gl_Position = projection * view * model * (position );
    fragCoords = 0.5 * uv + 0.5;
}
