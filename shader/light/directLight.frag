#version 330 core

// 定向光源的片段着色器

in vec3 Normal;			// 法向量
in vec3 FragPos;		// 当前绘制像素的位置
in vec2 TexCoords;		// 材质的位置

out vec4 OutColor;		// 着色器最后输出的颜色

struct Light{
	vec3 lightDirection;	// 光照的方向
	vec3 ambient;			// 物体的环境光强度
	vec3 diffuse;			// 物体的漫反射
	vec3 specular;			// 物体的镜面反射

};

struct Material{			// 材质相关的结构体
	sampler2D diffuse;			// 物体的漫反射颜色
	sampler2D specular;			// 物体的镜面反射颜色
	float shiness;			// 反射强度
};

uniform Light light;
uniform Material materialTex;
uniform vec3 viewPos;		// 视角的位置
void main(){
	// 法线向量归一化
	vec3 norm = normalize(Normal);
	// 从材质中获取颜色
	vec3 curColor = vec3(texture(materialTex.diffuse, TexCoords));
	// 从材质贴图中获取反射光（通常是黑白的图像表示强度）
	vec3 specColor = vec3(texture(materialTex.specular, TexCoords));
	// 计算环境光
	vec3 ambient = light.ambient * curColor;


	// 计算漫反射，只有光的方向与法向量垂直的时候，是最亮的
	vec3 lightDir = normalize(-1.0f * light.lightDirection);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = diff * light.diffuse * curColor;

	// 计算镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialTex.shiness);
	vec3 specular = (spec * specColor) * (light.specular);

	vec3 result = ambient + diffuse + specular;

	OutColor = vec4(result, 1.0f);
}