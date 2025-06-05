#version 330 core

in vec3 Normal;
in vec3 FragPos;                                            // 使用 model 矩阵进行变换后的位置
in vec2 TexCoords;                                          // 材质位置

out vec4 outColor;

    
//uniform float ambientStrength;                          // 环境光强度
//uniform float specularStrength;                         // 镜面反射的强度
//uniform vec3 objectColor;



uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;                                   // 观察点的位置


// 使用结构体来进行操作
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

uniform sampler2D emitterShader;

// 我们也可以通过设置漫反射贴图和镜面反射贴图来实现
struct MaterialTex{
    sampler2D diffuse;
    sampler2D specular;
    float shiness;
};


//uniform Material material;
//void calcColor(out vec3 result){
//    // 通过结构体来设置物体的颜色二
//    vec3 ambient = (material.ambient * light.ambient) ;
//    
//    vec3 norm = normalize(Normal);                      // 法向向量归一化
//    
//    // 漫反射
//    vec3 lightDir = normalize(lightPos - FragPos);      // 计算光的方向
//    float diff = max(dot(norm, lightDir), 0.0f);        // 光方向与法向向量夹角越大。越暗
//    vec3 diffuse = light.diffuse * diff * material.diffuse;                   // 相当于是光的直射角度
//
//    
//
//    // 下面加上镜面反射
//    vec3 viewDir = normalize(viewPos - FragPos);        // 
//    vec3 reflectDir = reflect(-lightDir, norm);          // 计算入射向量根据指定法向量的反射向量
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiness);
//    vec3 specular = (spec  * material.specular)* light.specular;
//    result = (ambient + diffuse + specular );   // 环境光 + 漫反射 + 镜面反射
//}


uniform MaterialTex materialTex;
void calcColorUsingTex(out vec3 result){
    vec3 norm = normalize(Normal);

    vec3 basicColor = vec3(texture(materialTex.diffuse, TexCoords));


    vec3 lightDir = normalize(lightPos - FragPos);      // 计算光的方向
    float diff = max(dot(norm, lightDir), 0.0f);        // 光方向与法向向量夹角越大。越暗
    
    ///环境光和漫反射
    vec3 diffuse = light.diffuse * (diff * basicColor); // 漫反射
    vec3 ambient = light.ambient * basicColor;          // 环境光

    // 镜像反射
    vec3 specColor = vec3(texture(materialTex.specular, TexCoords));
    vec3 viewDir = normalize(viewPos - FragPos);        // 
    vec3 reflectDir = reflect(-lightDir, norm);          // 计算入射向量根据指定法向量的反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialTex.shiness);
    vec3 specular = (spec  * specColor)* light.specular;
    result = (ambient + diffuse + specular );           // 环境光 + 漫反射 + 镜面反射

}

void main()
{
    vec3 result;
    calcColorUsingTex(result);

    outColor = vec4(result, 1.0f);
};