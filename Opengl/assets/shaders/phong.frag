#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;	//diffuse 漫反射
uniform sampler2D specularMaskSampler;//specularMask 高光蒙版

uniform vec3 ambientColor;

uniform vec3 cameraPosition;
uniform float shiness;

uniform float opacity; // 透明度

struct DirectionalLight{
	vec3 direction;
	vec3 color;
	float specularIntensity;
	float intensity; // 光强
};

struct PointLight{
	vec3 position;
	vec3 color;
	float specularIntensity;

	float k2;
	float k1;
	float kc;
};

struct SpotLight{
	vec3 position;
	vec3 targetDirection;
	vec3 color;
	float outerLine;
	float innerLine;
	float specularIntensity;
};

uniform DirectionalLight directionalLight;

vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal){
	float diffuse = clamp(dot(-lightDir, normal), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;

	return diffuseColor;
}

vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity){

	float dotResult = dot(-lightDir, normal);
	float flag = step(0.0, dotResult);
	vec3 lightReflect = normalize(reflect(lightDir,normal));

	//2 计算 specular
	float specular = max(dot(lightReflect,-viewDir), 0.0);

	specular = pow(specular, shiness);

	float specularMask = texture(specularMaskSampler, uv).r;

	vec3 specularColor = lightColor * specular * flag * intensity * specularMask;

	return specularColor;
}

vec3 calculateSpotLight(vec3 objectColor, SpotLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(worldPosition - light.position);
	vec3 targetDir = normalize(light.targetDirection);

	//����spotlight�����䷶Χ
	float cGamma = dot(lightDir, targetDir);
	float intensity =clamp( (cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0, 1.0);

	//1 ����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 ����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return (diffuseColor + specularColor)*intensity;
}

vec3 calculateDirectionalLight(vec3 objectColor, DirectionalLight light, vec3 normal ,vec3 viewDir){
	light.color *= light.intensity;

	vec3 lightDir = normalize(light.direction);
	//1 ����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 ����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return diffuseColor + specularColor;
}

vec3 calculatePointLight(vec3 objectColor, PointLight light, vec3 normal ,vec3 viewDir){

	vec3 lightDir = normalize(worldPosition - light.position);

	//����˥��
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	//1 ����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 ����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return (diffuseColor + specularColor)*attenuation;
}

void main()
{
	vec3 result = vec3(0.0,0.0,0.0);

	vec3 normalN = normalize(normal);
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 objectColor = texture(sampler, uv).xyz;
	result += calculateDirectionalLight(objectColor, directionalLight,normalN, viewDir);

	float alpha = texture(sampler, uv).a;
	// 环境光
	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = result + ambientColor;
	// 深度检测
//	float flag = step(400, gl_FragCoord.x);
//	vec3 blendColor = mix(vec3(0.5,0.0,0.0), finalColor, flag);
//	finalColor *= blendColor;
//	finalColor = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
	// 深度图
//	float Zndc = gl_FragCoord.z * 2.0 - 1.0;
//	float linearDepth = 2.0 * near / (far + near - Zndc * (far - near));
//	finalColor = vec3(linearDepth, linearDepth, linearDepth);

	FragColor = vec4(finalColor, alpha * opacity);
}