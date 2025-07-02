#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
		void setVec4f(const std::string& name, float x, float y, float z, float t); 
		void setMat4(const std::string& name, glm::mat4& transform);		
		void updateMixValue(float mixValue); 
		void updateMat4(glm::mat4& transform);
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{		
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream;
	std::stringstream fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();		
	vShaderFile.close();
	fShaderFile.close();
	vertexCode   = vShaderStream.str();
	fragmentCode = fShaderStream.str();		
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

inline void Shader::use()
{
	glUseProgram(ID);
}

inline void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

inline void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

inline void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

inline void Shader::setVec4f(const std::string &name, float x, float y, float z, float t)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, t); 
}

inline void Shader::setMat4(const std::string &name, glm::mat4 &trans)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}

inline void Shader::updateMixValue(float mixValue)
{
	setFloat("mixValue", mixValue);
}

inline void Shader::updateMat4(glm::mat4 &transform)
{
  setMat4("transform", transform);
}
