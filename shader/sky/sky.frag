#version 330 core


uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB do



out vec4 OutColor;
in vec2 fragCoord;                                          // 材质位置

// 经典 hash 函数
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}
// 经典 value noise
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    // 插值曲线
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(
        mix(hash(i + vec2(0.0, 0.0)), hash(i + vec2(1.0, 0.0)), u.x),
        mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), u.x),
        u.y
    );
}

// 分形布朗运动（Fractal Brownian Motion）
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

    float minDist = 1.0;  // 初始化为最大距离

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

// 根据位置，生成一个随机梯度，但是相同的输入，得到的输出结果是相同的
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

// 平滑
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

    // 添加平滑
    gridUv = quintic(gridUv);

    // 计算当前坐标到四个角的向量
    vec2 toBl = gridUv - vec2(0.0, 0.0);
    vec2 toBr = gridUv - vec2(1.0, 0.0);
    vec2 toTl = gridUv - vec2(0.0, 1.0);
    vec2 toTr = gridUv - vec2(1.0, 1.0);

    // 计算位置差向量和梯度的点积
    float dotBl = dot(gradBl, toBl);
    float dotBr = dot(gradBr, toBr);
    float dotTl = dot(gradTl, toTl);
    float dotTr = dot(gradTr, toTr);

    // 进行混合
    float b = mix(dotBl, dotBr, gridUv.x); // 根据 x 对底部的两个 gradient 进行混合
    float t = mix(dotTl, dotTr, gridUv.x); // 顶部两个的混合

    float perlin = mix(b, t, gridUv.y);
    return perlin+0.2;
}



void main() {

     // 云的偏移
    vec2 movement = vec2(iTime * 0.05, iTime * 0.02);
    // 获取噪声值
    vec2 uv = fragCoord * 0.5 + 0.5;
    float n = fbm(uv + movement);

    vec3 skyColor = vec3(0.4, 0.75, 1.0); // 天空蓝
    vec3 cloudColor = vec3(1.0);          // 白云
     // 插值
    vec3 color = mix(skyColor, cloudColor, smoothstep(0.5, 0.8, n));



//    color = vec3(getPerlin(fragCoord*0.5 + 0.5));
//    vec3 color = vec3(getPerlin(fragCoord*0.5 + 0.5));

//    color = vec3(voronoi(uv));


    OutColor = vec4(color, 1.0);
}