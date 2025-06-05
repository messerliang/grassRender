#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 LightingColor;

uniform float ambientStrength;                          // 环境光强度
uniform float specularStrength;                         // 镜面反射的强度

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;                                   // 观察点的位置


void main()
{
	gl_Position = projection * view * model * position;


	// 下面使用 gouraud shading
	// 环境光
    vec3 ambient = (ambientStrength * lightColor);
    
    vec3 Position = vec3(model * position);
	vec3 norm = mat3(transpose(inverse(model))) * normal;	// 如果物体使用了不等比例的缩放，为了保证法向量不异常，我们需要为法向量设置对应的操作

	// 漫反射
    vec3 lightDir = normalize(lightPos - Position);      // 计算光的方向
    float diff = max(dot(norm, lightDir), 0.0f);        // 光方向与法向向量夹角越大。越暗
    vec3 diffuse = diff * lightColor;                   // 相当于是光的直射角度

	// 下面加上镜面反射
    vec3 viewDir = normalize(viewPos - Position);        // 
    vec3 reflectDir = reflect(-lightDir, norm);          // 计算入射向量根据指定法向量的反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    vec3 result = (ambient + diffuse + specular );   // 环境光 + 漫反射 + 镜面反射

    LightingColor = result;

};
