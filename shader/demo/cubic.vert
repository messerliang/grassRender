#version 330 core

layout (location = 0) in vec4 position;

layout (location = 1) in vec4 positionOffset;           // ��ǰʵ��ƫ�����ĵľ���
layout (location = 2) in float angle;                   // ��ת�������ɲ�ͬ����Ĳ�
layout (location = 3) in float heightScale;             // �߶�������ӣ��ڲ�ͬʵ�������ɲ�ͬ�߶ȵĲ�
layout (location = 4) in float maxHeight;               // ��ǰʵ�������߶�

layout (location = 5) in vec4 cubicBezierP1;            // �ݵ�������cubic bezier curve
layout (location = 6) in vec4 cubicBezierP2;
layout (location = 7) in vec4 cubicBezierP3;
layout (location = 8) in vec4 randDir;                  // 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float iTime;
uniform float grassWidth;
uniform float grassLength;
uniform vec3 windDir;                                   // ��ķ���
uniform vec3 grassTilePosition;                         // ��ǰ�ݿ��λ��
uniform vec3 cameraPosition;                            // �������λ��

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

// α 2D perlin noise���򻯰棩
float noise(vec2 p, float scale=8) {
    p = p * scale;
    vec2 i = floor(p);
    vec2 f = fract(p);
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f); // fade ����
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
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



// ������ĳ������ת�ľ���
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

// ����ĳ������ƫ�Ƶľ���
mat4 translation(vec3 dir) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,   // ��1��
        0.0, 1.0, 0.0, 0.0,   // ��2��
        0.0, 0.0, 1.0, 0.0,   // ��3��
        dir.x, dir.y, dir.z, 1.0  // ��4�У�ƫ����
    );
}

void main() {
    
    vec4 origin = vec4(0.0f);
    mat4 rotate = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), angle);
    
    // ��ʵ������ʵλ��
    vec4 realPosition = vec4(positionOffset.x, 0.0f, positionOffset.z, 0.0f) + vec4(grassTilePosition,0.0f);
    // �ݵĳ���
    vec4 grassNorm = vec4(0.0f, 0.0f, 1.0f,0.0f);
    grassNorm = rotate * grassNorm;             // ��ת��ݵĳ���
    // ���ķ���
    vec3 viewDir= normalize(cameraPosition - realPosition.xyz);
    float orth = abs(dot(grassNorm.xyz, viewDir));
    float angleRefine = angle + (orth < 0.15 ? 3.14 / 9.0:0.0);
    rotate = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), angleRefine);
    
    
    
    vec4 instancePos = scaleHeight(position); // �߶�����
    
    heightPercent = instancePos.y / maxHeight;

    // ���������������ɵ�ǰ�ݵ�����
    vec4 cP3 = vec4(cubicBezierP3.x, cubicBezierP3.y, cubicBezierP3.z , 0.0f);
    instancePos = cubicBezier(instancePos, origin, cubicBezierP1, cubicBezierP2, cP3);



    // ��һЩ�����������û�з�����µ��������
    float offset = maxHeight * 0.2 * noise(positionOffset.xz + iTime * 0.08);   // ��ֹ����µ������������
    
    vec4 displaced =  normalize(randDir) * offset * heightPercent;              // ��ĳ�������������

    // �ݵ����ת��
    vec4 afterRotate = rotate * (instancePos + displaced);

    // �紵������ת��
    vec2 uniformUv = 0.5 + 0.5 * vec2((positionOffset.x + grassTilePosition.x) / grassWidth, (positionOffset.z + grassTilePosition.z) / grassLength);
    float bendStrenngth = pow(heightPercent, 0.5);
    vec3 windRotateAxis = cross(normalize(windDir), vec3(0.0f, 1.0f, 0.0f));
    mat4 windRotateMat = rotationMatrix(windRotateAxis, 3.14 * 0.45 * snoise(uniformUv + 0.3*iTime) * bendStrenngth );
    
    vec4 afterWind =  windRotateMat * afterRotate;

     // ��ӵ���߶����
    float bump = 10.0f * noise(uniformUv, 4);
    vec4 bumpV4 = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    gl_Position = projection * view * model * (afterWind  + bumpV4 +  positionOffset  + vec4(grassTilePosition.xyz, 0.0f));
    
}
