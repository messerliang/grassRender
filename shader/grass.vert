#version 330 core

layout (location = 0) in vec4 position;

layout (location = 1) in vec4 positionOffset;
layout (location = 2) in float angle;
layout (location = 3) in float heightScale;
layout (location = 4) in float maxHeight;

layout (location = 5) in vec4 cubicBezierP1;
layout (location = 6) in vec4 cubicBezierP2;
layout (location = 7) in vec4 cubicBezierP3;
layout (location = 8) in vec4 randDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float iTime;
uniform float grassWidth;
uniform float grassLength;

out float heightPercent;


vec4 scaleHeight(vec4 pos){
    vec4 result = vec4(pos.x, pos.y * heightScale, pos.z, pos.w);
    return result;
}



vec4 cubicBezier(vec4 curP, vec4 p0, vec4 p1, vec4 p2, vec4 p3) {
    float t = curP.y / maxHeight;
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    vec4 result = curP;

    result.y = uuu * p0.y           // (1 - t)^3 * P0
        + 3 * uu * t * p1.y         // 3(1 - t)^2 * t * P1
        + 3 * u * tt * p2.y         // 3(1 - t) * t^2 * P2
        + ttt * p3.y;               // t^3 * P3

    result.z =  uuu * p0.z
                + 3 * uu * t * p1.z
                + 3 * u * tt * p2.z
                + ttt * p3.z;

    return result;
}


float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// 伪 2D perlin noise（简化版）
float noise(vec2 p, float scale=8) {
    p = p * scale;
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f); // fade 函数
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// 生成绕某根轴旋转的矩阵
mat4 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0 - c;

    axis = normalize(axis);
    float x = axis.x, y = axis.y, z = axis.z;
    mat3 r3 = 
    mat3(
        t*x*x + c    , t*x*y - s*z, t*x*z + s*y,
        t*x*y + s*z  , t*y*y + c  , t*y*z - s*x,
        t*x*z - s*y  , t*y*z + s*x, t*z*z + c
    );
    mat4 resultMat4 = mat4(
        vec4(r3[0], 0.0),
        vec4(r3[1], 0.0),
        vec4(r3[2], 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
    return resultMat4;
}

// 生成某个方向偏移的矩阵
mat4 translation(vec3 dir) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,   // 第1列
        0.0, 1.0, 0.0, 0.0,   // 第2列
        0.0, 0.0, 1.0, 0.0,   // 第3列
        dir.x, dir.y, dir.z, 1.0  // 第4列：偏移量
    );
}

void main() {
    vec4 origin = vec4(0.0f);
    mat4 rotate = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), angle);
    
    vec4 instancePos = scaleHeight(position); // 高度缩放
    
    heightPercent = instancePos.y / maxHeight;

    instancePos = cubicBezier(instancePos, origin, cubicBezierP1, cubicBezierP2, cubicBezierP3);



    // 加一些随机抖动，在没有风情况下的随机抖动
    float offset = maxHeight * 0.2 * noise(positionOffset.xz + iTime * 0.05);

    vec4 displaced =  vec4(0.0f, 0.0f, 1.0f, 0.0f) * offset * heightPercent;

    // 草的随机转向
    vec4 afterRotate = rotate * (instancePos + displaced);

    // 风吹带来的转向
    vec2 uniformUv = vec2(positionOffset.x / grassWidth, positionOffset.z / grassLength);
    float bendStrenngth = pow(heightPercent, 0.5);
    mat4 windRotateMat = rotationMatrix(vec3(1.0f, 0.0, 0.0), 3.14 * 0.5 * noise(uniformUv + 0.06*iTime, 16) * bendStrenngth );
    vec4 afterWind =  windRotateMat*afterRotate;

    // 添加地面高度起伏
    float bump = 10.0f * noise(uniformUv, 4);
    vec4 bumpV4 = vec4(0.0f, bump, 0.0f, 0.0f);
    
    gl_Position = projection * view * model * (afterWind  +  positionOffset + bumpV4);
    
}
