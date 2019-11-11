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
	//texture()����������ɫ,����1���������,����2��Ӧ��������
	FragColor=texture(myTexture,TexCoord);
}