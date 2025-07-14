#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Camera
{
public:
	inline static glm::vec3 m_pos;
	inline static glm::vec3 m_front;
	inline static glm::vec3 m_up;
	inline static float m_fovy = 45;
	inline static float m_near = 0.1;
	inline static float m_far = 100;
	inline static float lastX = 0;
	inline static float lastY = 0;
	inline static float yaw = -90;
	inline static float pitch = -45;
	inline static float sensitivity = 0.1; 
	inline static bool firstMouse = true;

public:
	Camera() = default;
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
	{
		m_pos = pos;
		m_front = front;
		m_up = up;
	}

	static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			return;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw -= xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		glm::vec3 front;
		front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		m_front = glm::normalize(front);
	}

	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
	{
		m_fovy -= 2 * (float)yoffset;
		if (m_fovy < 1.0f)
			m_fovy = 1.0f;
		if (m_fovy > 89.0f)
			m_fovy = 89.0f;
	}
};

