#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <shader.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

template <typename T>
void outMatrixFormatted(T matrix) {
	int col = matrix.length(); 
	int row = matrix[0].length();
	cout << fixed << setprecision(2);
	for (int j = 0; j < col; j++) {
		cout << "[";
		for (int i = 0; i < row; i++) {
			cout << (float)matrix[i][j];
			if (i != row - 1) cout << ", ";
		}
		cout << "]\n";
	}
	cout << "[.]\n";
	return;
}

template <typename T> 
void outVectorFormatted(T V) {
	int length = V.length(); 
	cout << fixed << setprecision(2);
	for (int i = 0; i < length; i++) cout << "[" << (float)V[i] << "]\n";
	cout << "[.]\n";
	return;
}

float mixValue = 0;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader ourShader("./shader/vertexShader.txt", "./shader/fragmentShader.txt");

    // float vertices[] = {
    //     -1.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    //     1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    //     1.0f, -1.0f, 0.0f,      0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    //     -1.0f, -1.0f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    // };

    float vertices[] = {
        // x, y, z, w
        -1.0f, 1.0f, 0.5f, 1.5f,        1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 1.0f,  3.5f, 2.5f,        0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        1.0f, -1.0f, 0.5f, 1.5f,        0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, 0.5f, 1.5f,       1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./img/wall.png", &width, &height, &nrChannels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load("./img/happy.png", &width2, &height2, &nrChannels2, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data2);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // đầu vào cuối cùng trong openGL vẫn là vextext shader và fragment shader 
    // openGL không quan tâm đến các cái trong hàm main.cpp nó chỉ quan tâm đến đồ họa đó là shader vertex và fragment shader 
    // các cái mình thao tác trên màn main bản chất chỉ là truyền các thông số vào shader và gpu sử dụng đầu ra của file shader 
    // chứ không phải là các biến trong hàm main 
    // 

    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.5f));
    // projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / SCR_HEIGHT, 1.0f, 2.0f);

    // glm::vec3 p1 = glm::vec3(-1.0f, 1.0f, 0.0f);
    // glm::vec3 p2 = glm::vec3(1.0f, 1.0f, -1.0f);
    // glm::vec3 p3 = glm::vec3(1.0f, -1.0f, 0.0f);
    // glm::vec3 p4 = glm::vec3(-1.0f, -1.0f, 0.0f);

    // outVectorFormatted(projection * view * model * glm::vec4(p1, 1.0));
    // outVectorFormatted(projection * view * model * glm::vec4(p2, 1.0));
    // outVectorFormatted(projection * view * model * glm::vec4(p3, 1.0));
    // outVectorFormatted(projection * view * model * glm::vec4(p4, 1.0));

    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.setFloat("mixValue", mixValue);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f;
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*

#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float ratio; 
uniform mat4 transform; 

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * aPos;
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2; 

uniform float mixValue;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(ourColor, 1.0);
}


*/