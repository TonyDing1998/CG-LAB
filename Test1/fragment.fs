#version 330 core
out vec4 FragColor;


in vec2 TexCoord;
in vec3 Normal;
uniform sampler2D myTexture;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = vec4(0.5,0.5,0.5,1.0);
	//texture()采样纹理颜色,参数1纹理采样器,参数2对应纹理坐标
	FragColor=texture(myTexture,TexCoord);
}