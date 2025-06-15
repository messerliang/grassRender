#version 330 core



in float heightPercent;
in vec3 OutColor;

float easeInQuad(float t) {
    return t * t;
}

vec3 lavenderGradient(float y) {
    vec3 bottom = vec3(0.48, 0.32, 0.6); // ����
    vec3 middle = vec3(0.71, 0.49, 0.86); // ޹�²���
    vec3 top    = vec3(0.90, 0.80, 1.0); // ����/�߹�

    if (y < 0.5) {
        // �ײ� �� �в�����
        return mix(bottom, middle, y / 0.5);
    } else {
        // �в� �� ��������
        return mix(middle, top, (y - 0.5) / 0.5);
    }
}

out vec4 FragColor;
void main() {
    vec3 color_bottom = vec3(0.05, 0.35, 0.01);
    vec3 color_top = vec3(0.4, 0.8, 0.3);

    vec3 grass_color = mix(color_bottom, color_top, easeInQuad(heightPercent));
    
    vec3 renderColor = mix(grass_color, OutColor, 0.1);
    FragColor = vec4(renderColor, 1.0);
}
