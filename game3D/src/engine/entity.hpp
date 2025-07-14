#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Entity 
{
public:
	int m_id; 
	int m_active;
	glm::vec3 m_pos; 

public:
	Entity(int id, glm::vec3 pos) 
	{
		m_id = id;
		m_pos = pos; 
		m_active = true; 
	}
}; 

class Player : Entity 
{
public:
	glm::vec3 m_direction; 

public:
	Player() = default; 

	Player(int id, glm::vec3 pos) : Entity(id, pos)
	{
		m_direction = glm::vec3(-1, -1, 0); 
	}

	glm::vec3& getPos() 
	{
		return m_pos; 
	}

	glm::vec3 getPosCamPlayer() 
	{
		glm::vec3 x = glm::normalize(getDirPlayer());
		return getPos() - x * 5.0f + glm::vec3(0.5, 0.5, 0.5);
	}

	glm::vec3& getDirPlayer() 
	{
		return m_direction;
	}
};

