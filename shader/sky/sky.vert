#version 330 core

layout (location = 0) in vec4 position;

out vec2 fragCoord;

void main() {
    fragCoord = position.xy;
    gl_Position = position;
}
