#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<vector>

enum CameraMoveDirection
{
	FORWORD, BACKWORD, RIGHT, LEFT
};
float Yaw = -90.0f;
float Pitch = 0.0f;
float Sense = 0.05f;
float Speed = 3.0f;
float Fov = 45.0f;

class Camera
{
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float yaw;
	float pitch;
	float speed;
	float sense;
	float fov;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float YAW = Yaw, float PITCH = Pitch) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(Speed), sense(Sense), fov(Fov)
	{
		Pos = pos;
		WorldUp = up;
		yaw = YAW;
		pitch = PITCH;
		upCamera();
	}
	Camera(float pos_x, float pos_y,float pos_z,float up_x, float up_y, float up_z, float YAW,float PITCH) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(Speed), sense(Sense), fov(Fov)
	{
		Pos.x = pos_x;
		Pos.y = pos_y;
		Pos.z = pos_z;
		WorldUp.x = up_x;
		WorldUp.y = up_y;
		WorldUp.z = up_z;
		Yaw = YAW;
		Pitch = PITCH;
		upCamera();
	}
	void KeyBoard(CameraMoveDirection dir, float deltatime)
	{
		float MoveSpeed = speed*deltatime;
		switch (dir)
		{
		case FORWORD:
			Pos += MoveSpeed * Front;
			break;
		case BACKWORD:
			Pos -= MoveSpeed * Front;
			break;
		case RIGHT:
			Pos += Right * MoveSpeed;
			break;
		case LEFT:
			Pos -= Right * MoveSpeed;
			break;
		}
	}
	void MouseMove(float xoffset,float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= sense;
		yoffset *= sense;

		yaw += xoffset;
		pitch += yoffset;

		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		upCamera();
	}
	void MouseScroll(float yoffset)
	{
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}
	glm::mat4 getViewMtx()
	{
		return glm::lookAt(Pos, Pos + Front, Up);
	}
private:
	void upCamera()
	{
		glm::vec3 fr;
		fr.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
		fr.y = sin(glm::radians(pitch));
		fr.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
		Front = glm::normalize(fr);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif // ! CAMERA_H

