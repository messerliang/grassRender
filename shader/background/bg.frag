#version 330 core



uniform vec3      iResolution;           // viewport resolution (in pixels)，width, height, 1.0
uniform float     iTime;                 // shader playback time (in seconds)


//out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;                                            // 使用 model 矩阵进行变换后的位置
in vec2 TexCoords;                                          // 材质位置

out vec4 outColor;


uniform float curTime;


void main(){
	
	outColor = vec4(.3f, .2f, .6f, 1.0f);

}