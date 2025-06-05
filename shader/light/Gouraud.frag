#version 330 core

in vec3 LightingColor;
out vec4 outColor;

uniform vec3 objectColor;

void main()
{
    outColor = vec4(LightingColor * objectColor, 1.0f);
};