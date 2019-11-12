#version 330 core
out vec4 FragColor;


//in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
//uniform sampler2D myTexture;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = vec4(0.5,0.5,0.5,1.0);
	//texture()采样纹理颜色,参数1纹理采样器,参数2对应纹理坐标
	//FragColor=texture(myTexture,TexCoord);
	float ambientStrength=0.1f;
	vec3 ambient=ambientStrength*lightColor;

	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(lightPos-FragPos);
	float diff=max(dot(norm,lightDir),0.0f);
	vec3 diffuse=diff*lightColor;

	float specularStrength=0.5f;
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);;
	float spec=pow(max(dot(norm,reflectDir),0.0f),32);
	vec3 specular=specularStrength*spec*lightColor;
	
	vec3 result=(ambient+diffuse+specular)*objectColor;
	FragColor=vec4(result,1.0f);
}