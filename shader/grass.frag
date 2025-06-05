#version 330 core



in float heightPercent;

float easeInQuad(float t) {
    return t * t;
}

vec3 lavenderGradient(float y) {
    vec3 bottom = vec3(0.48, 0.32, 0.6); // 深紫
    vec3 middle = vec3(0.71, 0.49, 0.86); // 薰衣草紫
    vec3 top    = vec3(0.90, 0.80, 1.0); // 淡紫/高光

    if (y < 0.5) {
        // 底部 → 中部过渡
        return mix(bottom, middle, y / 0.5);
    } else {
        // 中部 → 顶部过渡
        return mix(middle, top, (y - 0.5) / 0.5);
    }
}

out vec4 FragColor;
void main() {
    vec3 color_bottom = vec3(0.05, 0.35, 0.01);
    vec3 color_top = vec3(0.4, 0.8, 0.3);

    vec3 grass_color = mix(color_bottom, color_top, easeInQuad(heightPercent));
    

    FragColor = vec4(grass_color, 1.0);
}
