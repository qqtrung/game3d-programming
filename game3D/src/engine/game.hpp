#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <engine/map.hpp>
#include <engine/entity.hpp>
#include <engine/camera.hpp>
#include <engine/vertex.hpp>
#include <engine/physics.hpp>
#include <engine/shader.hpp>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int face_opposite(float &x_pos, float &y_pos, float &z_pos, float &x_dir, float &y_dir, float &z_dir);

class Game
{
	Map *m_map;
	Camera *m_camera;
	Player *m_player;
	GLFWwindow *m_window;
	std::vector<Entity *> m_entity;
	std::vector<Shader *> m_shader;

	float m_deltaTime;
	float m_lastFrame;

	bool mouse_left_press = false;
	bool canAddBlock = true;
	bool m_run = true;

	unsigned int VBO_Block, VAO_Block;
	unsigned int VBO_Line, VAO_Line, EBO_Line;
	unsigned int texture1;
	unsigned int texture2;
	unsigned int texture3; 

public:
	Game(const std::string &file_Map)
	{
		init();
		m_player = new Player(-1, glm::vec3(5.6, 5.6, 5.6));
		m_camera = new Camera(m_player->getPosCamPlayer(), m_player->getDirPlayer(), glm::vec3(0, 1, 0));
		m_map = new Map(file_Map);
		for (int x = 0; x < Map::max_cel_x; x++)
		{
			for (int y = 0; y < Map::max_cel_y; y++)
			{
				for (int z = 0; z < Map::max_cel_z; z++)
				{
					if (m_map->getCell(x, y, z))
					{
						m_entity.push_back(new Entity(m_entity.size(), glm::vec3(x, y, z)));
					}
				}
			}
		}
	}

	void init();
	bool run();
	void processInput();

	void update();
	void updateCollision();

	void render();
	void renderCoordinate(Shader *shader);
	void renderEntity(Shader *shader);
	void renderPlayer(Shader *shader);

	~Game()
	{
		glfwTerminate();
	}
};

bool Game::run()
{
	return m_run;
}

void Game::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(m_window, Camera::framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, Camera::mouse_callback);
	glfwSetScrollCallback(m_window, Camera::scroll_callback);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glLineWidth(10.0f);
	glPointSize(10.0f);

	glGenBuffers(1, &VBO_Block);
	glGenBuffers(1, &VBO_Line);
	glGenBuffers(1, &EBO_Line);
	glGenVertexArrays(1, &VAO_Block);
	glGenVertexArrays(1, &VAO_Line);

	glBindVertexArray(VAO_Block);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Block);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBrick), vertexBrick, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO_Line);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLineCoordinate), vertexLineCoordinate, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width1, height1, nrChannels1;
	unsigned char *data1 = (unsigned char *)stbi_load("./img/wall.png", &width1, &height1, &nrChannels1, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data1);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width2, height2, nrChannels2;
	unsigned char *data2 = (unsigned char *)stbi_load("./img/smile.png", &width2, &height2, &nrChannels2, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data2);

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width3, height3, nrChannels3;
	unsigned char *data3 = (unsigned char *)stbi_load("./img/vip.png", &width3, &height3, &nrChannels3, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width3, height3, 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data3);

	m_shader.push_back(new Shader("./shader/block/block.vex", "./shader/block/block.frag"));
	m_shader.push_back(new Shader("./shader/line/line.vex", "./shader/line/line.frag"));
	m_shader.push_back(new Shader("./shader/player/player.vex", "./shader/player/player.frag"));

	m_shader[0]->use();
	m_shader[0]->setInt("texture1", 0);
	m_shader[0]->setInt("texture2", 1);

	m_shader[2]->use();
	m_shader[2]->setInt("texture", 0);
}

void Game::processInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		m_run = false;

	float speed = static_cast<float>(5 * m_deltaTime);

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		m_player->getPos() += speed * glm::vec3(m_player->m_direction.x, 0, m_player->m_direction.z);

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		m_player->getPos() -= speed * glm::vec3(m_player->m_direction.x, 0, m_player->m_direction.z);

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		m_player->getPos() -= glm::normalize(glm::cross(m_player->m_direction, m_camera->m_up)) * speed;

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		m_player->getPos() += glm::normalize(glm::cross(m_player->m_direction, m_camera->m_up)) * speed;

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_player->getPos() += 0.01f * m_camera->m_up;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		mouse_left_press = true;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		mouse_left_press = false;
}

void Game::update()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	m_camera->m_pos = m_player->getPosCamPlayer() + glm::normalize(glm::cross(m_player->m_direction, m_camera->m_up)) * 2.0f;
	if (m_player->getPos().y > 1.1)
	{
		m_player->getPos() += glm::vec3(0, -0.005, 0);
		if (m_player->getPos().y < 1.1)
			m_player->getPos().y = 1.1;
	}

	m_player->m_direction = m_camera->m_front;
	updateCollision();
}

void Game::updateCollision()
{
	TAABB playerBox;
	playerBox.m_vecMin = m_player->getPos();
	playerBox.m_vecMax = m_player->getPos() + glm::vec3(1, 1, 1);
	for (auto &entity : m_entity)
	{
		TAABB blockBox;
		blockBox.m_vecMin = entity->m_pos;
		blockBox.m_vecMax = entity->m_pos + glm::vec3(1, 1, 1);
		if (Physics::AABBtoAABB(playerBox, blockBox))
		{
			float overlapX = std::min(playerBox.m_vecMax.x - blockBox.m_vecMin.x, blockBox.m_vecMax.x - playerBox.m_vecMin.x);
			float overlapY = std::min(playerBox.m_vecMax.y - blockBox.m_vecMin.y, blockBox.m_vecMax.y - playerBox.m_vecMin.y);
			float overlapZ = std::min(playerBox.m_vecMax.z - blockBox.m_vecMin.z, blockBox.m_vecMax.z - playerBox.m_vecMin.z);
			if (overlapX <= overlapY && overlapX <= overlapZ)
			{
				if (playerBox.m_vecMin.x < blockBox.m_vecMin.x)
					m_player->getPos().x -= overlapX;
				else
					m_player->getPos().x += overlapX;
			}
			else if (overlapY <= overlapX && overlapY <= overlapZ)
			{
				if (playerBox.m_vecMin.y < blockBox.m_vecMin.y)
					m_player->getPos().y -= overlapY;
				else
					m_player->getPos().y += overlapY;
			}
			else
			{
				if (playerBox.m_vecMin.z < blockBox.m_vecMin.z)
					m_player->getPos().z -= overlapZ;
				else
					m_player->getPos().z += overlapZ;
			}
			playerBox.m_vecMin = m_player->getPos();
			playerBox.m_vecMax = m_player->getPos() + glm::vec3(1, 1, 1);
		}
	}
}

void Game::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderEntity(m_shader[0]);
	renderPlayer(m_shader[2]);
	renderCoordinate(m_shader[1]);
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Game::renderPlayer(Shader *shader)
{
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_player->getPos());
	glm::mat4 view = glm::lookAt(m_camera->m_pos, m_camera->m_pos + m_camera->m_front, m_camera->m_up);
	glm::mat4 perspective = glm::perspective(glm::radians(m_camera->m_fovy), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", perspective);
	glBindVertexArray(VAO_Block);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	float x_pos = m_camera->m_pos.x;
	float y_pos = m_camera->m_pos.y;
	float z_pos = m_camera->m_pos.z;
	float x_dir = m_camera->m_front.x;
	float y_dir = m_camera->m_front.y;
	float z_dir = m_camera->m_front.z;
	int x_cel = (int)x_pos;
	int y_cel = (int)y_pos;
	int z_cel = (int)z_pos;
	x_pos -= x_cel;
	y_pos -= y_cel;
	z_pos -= z_cel;
	glm::vec3 sightBlockPositions;
	bool seen = false;
	for (int i = 0; i < 20; i++)
	{
		int x_cel_old = x_cel;
		int y_cel_old = y_cel;
		int z_cel_old = z_cel;
		int face = face_opposite(x_pos, y_pos, z_pos, x_dir, y_dir, z_dir);
		if (face == 1)
			z_cel -= 1;
		else if (face == 2)
			x_cel -= 1;
		else if (face == 3)
			y_cel -= 1;
		else if (face == 4)
			z_cel += 1;
		else if (face == 5)
			x_cel += 1;
		else if (face == 6)
			y_cel += 1;
		if (x_cel < 0 || y_cel < 0 || z_cel < 0)
			break;
		if (m_map->getCell(x_cel, y_cel, z_cel))
		{
			seen = true;
			sightBlockPositions = glm::vec3(x_cel_old, y_cel_old, z_cel_old);
			break;
		}
	}

	if (seen) 
	{
		model = glm::translate(glm::mat4(1.0f), sightBlockPositions);
		shader->setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	if (mouse_left_press)
	{
		if (canAddBlock)
		{
			m_entity.push_back(new Entity(m_entity.size(), sightBlockPositions));
			m_map->map[(int)sightBlockPositions.x][(int)sightBlockPositions.y][(int)sightBlockPositions.z] = 1;
			canAddBlock = false;
		}
	}
	else
	{
		canAddBlock = true;
	}
}

void Game::renderCoordinate(Shader *shader)
{
	shader->use();
	glm::mat4 view = glm::lookAt(m_camera->m_pos, m_camera->m_pos + m_camera->m_front, m_camera->m_up);
	glm::mat4 perspective = glm::perspective(glm::radians(m_camera->m_fovy), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::vec3 centerWindow = m_camera->m_pos + m_camera->m_front;
	shader->setMat4("model", glm::translate(glm::mat4(1.0f), centerWindow));
	shader->setMat4("view", view);
	shader->setMat4("projection", perspective);
	glBindVertexArray(VAO_Line);
	glDrawArrays(GL_POINTS, 0, 1);
	shader->setMat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f)));
	glBindVertexArray(VAO_Line);
	glDrawArrays(GL_LINES, 0, 6);
}

void Game::renderEntity(Shader *shader)
{
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glm::mat4 view = glm::lookAt(m_camera->m_pos, m_camera->m_pos + m_camera->m_front, m_camera->m_up);
	glm::mat4 perspective = glm::perspective(glm::radians(m_camera->m_fovy), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader->setMat4("view", view);
	shader->setMat4("projection", perspective);
	glBindVertexArray(VAO_Block);
	for (unsigned int i = 0; i < m_entity.size(); i++)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_entity[i]->m_pos);
		shader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

int face_opposite(float &x_pos, float &y_pos, float &z_pos, float &x_dir, float &y_dir, float &z_dir)
{
	int face = -1;
	float k_min = 100;
	if (z_dir != 0)
	{
		float k = -z_pos / z_dir;
		if (k > 0 && k < k_min)
		{
			face = 1;
			k_min = k;
		}
	}
	if (x_dir != 0)
	{
		float k = -x_pos / x_dir;
		if (k > 0 && k < k_min)
		{
			face = 2;
			k_min = k;
		}
	}
	if (y_dir != 0)
	{
		float k = -y_pos / y_dir;
		if (k > 0 && k < k_min)
		{
			face = 3;
			k_min = k;
		}
	}
	if (z_dir != 0)
	{
		float k = (1 - z_pos) / z_dir;
		if (k > 0 && k < k_min)
		{
			face = 4;
			k_min = k;
		}
	}
	if (x_dir != 0)
	{
		float k = (1 - x_pos) / x_dir;
		if (k > 0 && k < k_min)
		{
			face = 5;
			k_min = k;
		}
	}
	if (y_dir != 0)
	{
		float k = (1 - y_pos) / y_dir;
		if (k > 0 && k < k_min)
		{
			face = 6;
			k_min = k;
		}
	}

	if (k_min > 2)
	{
		assert(false);
	}

	if (face == 1)
	{
		x_pos = x_pos + x_dir * k_min;
		y_pos = y_pos + y_dir * k_min;
		z_pos = 1;
	}
	else if (face == 2)
	{
		x_pos = 1;
		y_pos = y_pos + y_dir * k_min;
		z_pos = z_pos + z_dir * k_min;
	}
	else if (face == 3)
	{
		x_pos = x_pos + x_dir * k_min;
		y_pos = 1;
		z_pos = z_pos + z_dir * k_min;
	}
	else if (face == 4)
	{
		x_pos = x_pos + x_dir * k_min;
		y_pos = y_pos + y_dir * k_min;
		z_pos = 0;
	}
	else if (face == 5)
	{
		x_pos = 0;
		y_pos = y_pos + y_dir * k_min;
		z_pos = z_pos + z_dir * k_min;
	}
	else if (face == 6)
	{
		x_pos = x_pos + x_dir * k_min;
		y_pos = 0;
		z_pos = z_pos + z_dir * k_min;
	}
	else
	{
		assert(false);
	}

	if (x_pos == 1)
		x_pos = 0.99;
	if (y_pos == 1)
		y_pos = 0.99;
	if (z_pos == 1)
		z_pos = 0.99;
	if (x_pos == 0)
		x_pos = 0.01;
	if (y_pos == 0)
		y_pos = 0.01;
	if (z_pos == 0)
		z_pos = 0.01;

	return face;
}
