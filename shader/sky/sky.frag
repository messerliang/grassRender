#version 330 core


uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB do



out vec4 OutColor;
in vec2 fragCoord;                                          // ����λ��

// ���� hash ����
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}
// ���� value noise
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    // ��ֵ����
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(
        mix(hash(i + vec2(0.0, 0.0)), hash(i + vec2(1.0, 0.0)), u.x),
        mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), u.x),
        u.y
    );
}

// ���β����˶���Fractal Brownian Motion��
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 0.0;

    for (int i = 0; i < 5; ++i) {
        value += amplitude * noise(p);
        p *= 2.0;
        amplitude *= 0.5;
    }

    return value;
}

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float voronoi(vec2 st) {
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float minDist = 1.0;  // ��ʼ��Ϊ������

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 neighbor = vec2(float(x), float(y));
            vec2 point = random(i_st + neighbor) * vec2(1.0, 1.0) + neighbor;
            float dist = length(f_st - point);
            minDist = min(minDist, dist);
        }
    }
    return minDist;
}

// ����λ�ã�����һ������ݶȣ�������ͬ�����룬�õ�������������ͬ��
vec2 randomGradient(vec2 uv){
    
    uv = uv + 0.01;
    float x = dot(uv, vec2(123.4, 234.5));
    float y = dot(uv, vec2(234.5, 345.6));
    vec2 result = vec2(x, y);
    result = sin(result);
    result = result * 43758.5453;
    result = sin(result);
    return result;
}

// ƽ��
vec2 quintic(vec2 p) {
  return p * p * p * (10.0 + p * (-15.0 + p * 6.0));
}

float getPerlin(vec2 uv){

    uv = 4 * uv + iTime;
    vec2 gridUv = fract(uv);
    vec2 gridId = floor(uv);
    vec2 bl = gridId + vec2(0.0, 0.0);
    vec2 br = gridId + vec2(1.0, 0.0);
    vec2 tl = gridId + vec2(0.0, 1.0);
    vec2 t2 = gridId + vec2(1.0, 1.0);

    vec2 gradBl = randomGradient(bl);
    vec2 gradBr = randomGradient(br);
    vec2 gradTl = randomGradient(tl);
    vec2 gradTr = randomGradient(t2);

    // ���ƽ��
    gridUv = quintic(gridUv);

    // ���㵱ǰ���굽�ĸ��ǵ�����
    vec2 toBl = gridUv - vec2(0.0, 0.0);
    vec2 toBr = gridUv - vec2(1.0, 0.0);
    vec2 toTl = gridUv - vec2(0.0, 1.0);
    vec2 toTr = gridUv - vec2(1.0, 1.0);

    // ����λ�ò��������ݶȵĵ��
    float dotBl = dot(gradBl, toBl);
    float dotBr = dot(gradBr, toBr);
    float dotTl = dot(gradTl, toTl);
    float dotTr = dot(gradTr, toTr);

    // ���л��
    float b = mix(dotBl, dotBr, gridUv.x); // ���� x �Եײ������� gradient ���л��
    float t = mix(dotTl, dotTr, gridUv.x); // ���������Ļ��

    float perlin = mix(b, t, gridUv.y);
    return perlin+0.2;
}



void main() {

     // �Ƶ�ƫ��
    vec2 movement = vec2(iTime * 0.05, iTime * 0.02);
    // ��ȡ����ֵ
    vec2 uv = fragCoord * 0.5 + 0.5;
    float n = fbm(uv + movement);

    vec3 skyColor = vec3(0.4, 0.75, 1.0); // �����
    vec3 cloudColor = vec3(1.0);          // ����
     // ��ֵ
    vec3 color = mix(skyColor, cloudColor, smoothstep(0.5, 0.8, n));



//    color = vec3(getPerlin(fragCoord*0.5 + 0.5));
//    vec3 color = vec3(getPerlin(fragCoord*0.5 + 0.5));

//    color = vec3(voronoi(uv));


    OutColor = vec4(color, 1.0);
}