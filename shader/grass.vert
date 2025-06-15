#version 330 core

layout (location = 0) in vec4 position;

layout (location = 1) in vec4 positionOffset;           // 当前实例偏移中心的距离
layout (location = 2) in float angle;                   // 旋转角度，生成不同方向的草
layout (location = 3) in float heightScale;             // 高度随机因子，在不同实例中生成不同高度的草
layout (location = 4) in float maxHeight;               // 当前实例的最大高度

layout (location = 5) in vec4 cubicBezierP1;            // 控制点1（起点默认是原点）
layout (location = 6) in vec4 cubicBezierP2;            // 控制点2
layout (location = 7) in vec4 cubicBezierP3;            // 曲线的终点

layout (location = 8) in vec4 randDir;                  // 无风情况下，当前草随机抖动的方向

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float iTime;
uniform float grassWidth;
uniform float grassLength;
uniform vec3 windDir;                                   // 风的方向
uniform vec3 grassTilePosition;                         // 当前草块的位置

out float heightPercent;
out vec3 OutColor;

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

float linearStep(float edge0, float edge1, float x) {
    x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
    return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
    
}

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+10.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}



void main() {
    vec4 origin = vec4(0.0f);
    mat4 rotate = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), angle);
    
    vec4 instancePos = scaleHeight(position); // 高度缩放
    
    heightPercent = instancePos.y / maxHeight;

    // 根据样条曲线生成当前草的曲线
    instancePos = cubicBezier(instancePos, origin, cubicBezierP1, cubicBezierP2, cubicBezierP3);



    // 加一些随机抖动，在没有风情况下的随机抖动
    float offset = maxHeight * 0.1 * noise(positionOffset.xz + iTime * 0.05);   // 静止情况下的随机抖动幅度

    vec4 displaced =  normalize(randDir) * offset * heightPercent;              // 往某个方向随机抖动

    // 草的随机转向
    vec4 afterRotate = rotate * (instancePos + displaced);

    // 风吹带来的转向
    vec2 uniformUv = vec2((positionOffset.x + grassTilePosition.x) / grassWidth, (positionOffset.z + grassTilePosition.z) / grassLength);
    float bendStrenngth = pow(heightPercent,0.7);
    vec3 windRotateAxis = cross(normalize(windDir), vec3(0.0f, 1.0f, 0.0f));
    float windStrength = snoise(uniformUv*6 + 0.20*iTime);
    mat4 windRotateMat = rotationMatrix(windRotateAxis, 3.14 * 0.5 * windStrength * bendStrenngth );
    vec4 afterWind =  windRotateMat*afterRotate;

    // 添加地面高度起伏
    float bump = 10.0f * noise(uniformUv, 16);
    vec4 bumpV4 = vec4(0.0f, bump, 0.0f, 0.0f);
    
    OutColor = vec3(0.45f,  1.0f-windStrength* 1, 0.63f);
    gl_Position = projection * view * model * (afterWind  +  positionOffset + bumpV4 + vec4(grassTilePosition.xyz, 0.0f));
    
}
