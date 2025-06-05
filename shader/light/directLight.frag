#version 330 core

// �����Դ��Ƭ����ɫ��

in vec3 Normal;			// ������
in vec3 FragPos;		// ��ǰ�������ص�λ��
in vec2 TexCoords;		// ���ʵ�λ��

out vec4 OutColor;		// ��ɫ������������ɫ

struct Light{
	vec3 lightDirection;	// ���յķ���
	vec3 ambient;			// ����Ļ�����ǿ��
	vec3 diffuse;			// �����������
	vec3 specular;			// ����ľ��淴��

};

struct Material{			// ������صĽṹ��
	sampler2D diffuse;			// �������������ɫ
	sampler2D specular;			// ����ľ��淴����ɫ
	float shiness;			// ����ǿ��
};

uniform Light light;
uniform Material materialTex;
uniform vec3 viewPos;		// �ӽǵ�λ��
void main(){
	// ����������һ��
	vec3 norm = normalize(Normal);
	// �Ӳ����л�ȡ��ɫ
	vec3 curColor = vec3(texture(materialTex.diffuse, TexCoords));
	// �Ӳ�����ͼ�л�ȡ����⣨ͨ���Ǻڰ׵�ͼ���ʾǿ�ȣ�
	vec3 specColor = vec3(texture(materialTex.specular, TexCoords));
	// ���㻷����
	vec3 ambient = light.ambient * curColor;


	// ���������䣬ֻ�й�ķ����뷨������ֱ��ʱ����������
	vec3 lightDir = normalize(-1.0f * light.lightDirection);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = diff * light.diffuse * curColor;

	// ���㾵�淴��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialTex.shiness);
	vec3 specular = (spec * specColor) * (light.specular);

	vec3 result = ambient + diffuse + specular;

	OutColor = vec4(result, 1.0f);
}