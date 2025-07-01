#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader ourShader("./shader/vertexShader.txt", "./shader/fragmentShader.txt"); 
    
    float vertices[] = {
        // system coordinate  // color            // texture coordinate 
         -1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.5f, 0.0f,
         -1.0f,  -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.5f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.5f, 0.0f 
    };

    // float vertices[] = {
    //     // system coordinate  // color            // texture coordinate > 1
    //      0.0f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    //      0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   5.0f, 0.0f,
    //      0.0f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 5.0f,
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.5f, 0.0f 
    // };

    unsigned int indices[] = {  
        0, 1, 3,
        0, 1, 2
    };

    unsigned int id_VBO, id_VAO, id_EBO;

    glGenVertexArrays(1, &id_VAO);
    glGenVertexArrays(1, &id_VAO);
    glGenVertexArrays(1, &id_VAO);
    glGenVertexArrays(1, &id_VAO);
    glGenVertexArrays(1, &id_VAO);

    glGenBuffers(1, &id_VBO); 
    glGenBuffers(1, &id_VBO);
    glGenBuffers(1, &id_VBO);
    glGenBuffers(1, &id_VBO);
    glGenBuffers(1, &id_VBO);

    glGenBuffers(1, &id_EBO);
    glGenBuffers(1, &id_EBO);
    glGenBuffers(1, &id_EBO);
    glGenBuffers(1, &id_EBO);
    glGenBuffers(1, &id_EBO);

    glBindVertexArray(5);

        glBindBuffer(GL_ARRAY_BUFFER, 2);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 6);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    glBindVertexArray(0); 

    unsigned int id_texture;
    glGenTextures(1, &id_texture);
    glBindTexture(GL_TEXTURE_2D, id_texture);

    // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // set texture filtering parameters
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./img/wall.jpg", &width, &height, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, id_texture);
        ourShader.use();

        glBindVertexArray(5);
            glDrawArrays(GL_TRIANGLES, 0, 3); 
            // glDrawArrays(GL_TRIANGLES, 1, 3); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &id_VAO);
    glDeleteBuffers(1, &id_VBO);
    glDeleteBuffers(1, &id_EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

