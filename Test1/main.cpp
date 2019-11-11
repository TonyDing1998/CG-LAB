#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include"camera.h"

#include <iostream>
#include"obj_loader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int loadTexture(char const *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastTime = 0.0f;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_WIDTH / 2;
bool firstMouse = true;
bool flag = true;
/*float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;*/

//glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//��Ϊ����ģʽ
    //glfwWindowHint(��Ҫ���õ�����, ���Ե�Ԥ��ֵ)
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGL", NULL, NULL);//5�����������ߡ����⡢2�ɺ���
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//����obj�ļ�,�������ݴ���vertData��
	vector<vertex> verData;
	if (!Loader::loadOBJ("./3D_model/SpaceShuttle.obj", verData))
	{
		cerr << "Failed to load obj model.";
		system("pause");
		exit(-1);
	}
	
	unsigned int myTexture = loadTexture("./3D_model/SpaceShuttle.png");
	/*glGenTextures(1, &myTexture);//��������Ϊ1���洢��myTexture������
	glBindTexture(GL_TEXTURE_2D, myTexture);//��
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	//������˺���glTexParamateri(a,b,c) 
	//a:GL_TEXTURE_2D������2D���� 
	//b1:GL_TEXTURE_WRAP_S(S�����ϵ���ͼģʽ)||GL_TEXTURE_WRAP_T(T�����ϵ���ͼģʽ)
	//c1:GL_REPEAT(Ĭ��,�ظ�����ͼ��)||GL_MIRRORED_REPEAT(�ظ�ʱ����)||GL_CLAMP_TO_EDGE(��������Լ����0-1֮��,�����������������Ե���)||GL_CLAMP_TO_BORDER(�����������ָ����ɫ)
	//b1��c1����ʹ��,�����������Ʒ�ʽ������
	//b2:GL_TEXTURE_MAG_FILTER(�Ŵ����)||GL_TEXTURE_MIN_FILTER(��С����)
	//c2:GL_LINEAR(���Թ���,ȡ��Χ���ؼ�Ȩƽ��ֵ)||GL_NEAREST(Ĭ��,�ڽ�����,ȡ�������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("./3D_model/SpaceShuttle.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(data);
	
	set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
		float vertices[] = { //����:����xyz,��ɫRGB,����ST
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};

	unsigned int VBO;//���㻺����󣬴��涥��Ӷ�һ���Է��͸��Կ�
	unsigned int VAO;//�����������
	//unsigned int EBO;//�����������
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);//ʹ��ID����һ��VBO����
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);//���㻺�����Ļ�������ΪGL_ARRAY_BUFFER�����´�����VBO����󶨵�GL_ARRAY_BUFFER��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//��֮ǰ����Ķ������ݸ��Ƶ�GL_ARRAY_BUFFER�У��������������͡��������ݵĴ�С��ϣ�����͵�ʵ�����ݡ��Կ���ι�������
	//GL_STATIC_DRAW:���ݲ���� GL_DYNAMIC_DRAW:���ݻᱻ��θı� GL_STREAM_DRAW:����ÿ�λ���ʱ�ı�

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*
	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("./awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);*/
	Mesh mesh(verData);
	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("./vertex.vs", "./fragment.fs");
	//Shader lightShader("./lamp_v.vs", "./lamp_f.fs");
	glEnable(GL_DEPTH_TEST);//��Ȳ��ԣ�������ֻ��ʾλ����ǰ��Ĳ���
	glEnable(GL_CULL_FACE);//���޳������������泯�����
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// bind textures on corresponding texture units
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myTexture);


		// activate shader
		ourShader.use();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//view = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));//�۵�λ�ã�����λ�ã������е����ϵ�����
		//float radius = 1.0f;																										  
		//float cameraX = sin(glfwGetTime())*radius;
		//float cameraY = cos(glfwGetTime())*radius;
		//view = glm::lookAt(glm::vec3(cameraX, 0.5f, cameraY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));//ƽ��(Ŀ�����,��������)

		if (flag)
			view = camera.getViewMtx();
			//view = glm::lookAt(cameraPos, cameraFront, cameraUp);
		else
		{
			float radius = 1.0f;
			float cameraX = sin(glfwGetTime())*radius;
			float cameraY = cos(glfwGetTime())*radius;
			view = glm::lookAt(glm::vec3(cameraX, 0.5f, cameraY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		ourShader.setMat4("view", view);
		float angle = 90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));//��ת
		//model = glm::scale(model, glm::vec3(1.5f,1.5f,1.5f));//����
		ourShader.setMat4("model", model);
		//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);//����ͶӰ
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);//͸��ͶӰ
		// set��uniform����֮������ֶԾ�������޸ģ�����Ҫʹ��������������ɫ���еľ�������޸ģ������޸Ĳ�����Ч
		//unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		//unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		// pass them to the shaders (3 different ways)
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);//�޸�uniform����(����λ��,����,GL_FALSEָʾ����ʱ�����Ⱦ���(Ĭ�Ͼ���Ϊ�����ȵ�)||GL_TRUE������,ָ�룩
		
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		ourShader.setMat4("projection", projection);
		mesh.draw(ourShader);
		// render container
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	mesh.de_allocate();//�ͷ�VAO��VBO
	glDeleteTextures(1, &myTexture);//ɾ���������
	glfwTerminate();//�ͷ�glfw��Դ
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	/*if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		flag = !flag;*/
	if(flag)
	{
		//
		float cameraSpeed = 2.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera.KeyBoard(FORWORD, deltaTime);
			//cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera.KeyBoard(BACKWORD, deltaTime);
			//cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera.KeyBoard(LEFT, deltaTime);
			//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera.KeyBoard(RIGHT, deltaTime);
			//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.MouseMove(xoffset, yoffset);
	
	/*float sense = 0.1f;
	xoffset *= sense;
	yoffset *= sense;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = glm::cos(glm::radians(yaw))*cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw))*cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);*/
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	/*if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;*/
	camera.MouseScroll(yoffset);
}
void key_callback(GLFWwindow *window, int key,int scancode, int action,int mods)
{
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		flag = !flag;
}
unsigned int loadTexture(char const *path)
{
	unsigned int TextureID;
	glGenTextures(1, &TextureID);//��������Ϊ1���洢��myTexture������
	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format == GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, TextureID);//��
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		//������˺���glTexParamateri(a,b,c) 
		//a:GL_TEXTURE_2D������2D���� 
		//b1:GL_TEXTURE_WRAP_S(S�����ϵ���ͼģʽ)||GL_TEXTURE_WRAP_T(T�����ϵ���ͼģʽ)
		//c1:GL_REPEAT(Ĭ��,�ظ�����ͼ��)||GL_MIRRORED_REPEAT(�ظ�ʱ����)||GL_CLAMP_TO_EDGE(��������Լ����0-1֮��,�����������������Ե���)||GL_CLAMP_TO_BORDER(�����������ָ����ɫ)
		//b1��c1����ʹ��,�����������Ʒ�ʽ������
		//b2:GL_TEXTURE_MAG_FILTER(�Ŵ����)||GL_TEXTURE_MIN_FILTER(��С����)
		//c2:GL_LINEAR(���Թ���,ȡ��Χ���ؼ�Ȩƽ��ֵ)||GL_NEAREST(Ĭ��,�ڽ�����,ȡ�������)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		

		stbi_image_free(data);
	}
	else
	{
		cout << "Failed to load texture." << endl;
		stbi_image_free(data);
	}
	return TextureID;
}