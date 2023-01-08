#define GLFW_INCLUDE_NONE
#pragma once
#include <yourGlad.c\glad.c>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>
#include <vector>
//window vars
GLFWwindow* window;
int _width = 640;
int _height = 480;
//shader vars
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int VBO, VAO;
unsigned int ModelLoc;
unsigned int shaderProgram;
const char* vertexShaderSourceChar;
const char* fragmentShaderSourceChar;
unsigned int viewLoc;
unsigned int projectionLoc;
glm::mat4 view;
//display and camera vars
float aspectRatio = 640.0f / 480.0f;
float fieldOfView = 180.0f;
float nearPlane = 0.1f;
float farPlane = 100.0f;
glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 MVP;
//player vars
const float min_x = 0.0f;
const float max_x = 3.0f;
const float min_y = 0.0f;
float xoffset = 0.0f;
float yoffset = 0.0f;
glm::vec3 playerPos = glm::vec3(0.0f, 0.0f, 0.0f);

struct Triangle
{
    std::vector<glm::vec3> vertices;
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f);
};

void clearBuffer()
{
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void initWindow(GLFWwindow*& window, int width, int height, const char* title)
{
    if (!glfwInit())
        exit(-1);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
}

void initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void updateViewMatrix(glm::mat4& view, glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp)
{
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
}
void updateProjectionMatrix(glm::mat4& projection, float aspectRatio, float fieldOfView, float nearPlane, float farPlane)
{
    projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearPlane, farPlane);
}

void renderTriangle(Triangle& triangle, unsigned int shaderProgram, unsigned int modelLoc, unsigned int viewLoc, unsigned int projectionLoc)
{
    glUseProgram(shaderProgram);

    int colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(colorLoc, triangle.color.r, triangle.color.g, triangle.color.b);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangle.vertices.size() * sizeof(glm::vec3), &triangle.vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(triangle.model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
int main()
{
    GLFWwindow* window;
    initWindow(window, 640, 480, "Jump nB");
    initGL();

    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
    Triangle triangle;
    triangle.vertices = {
        glm::vec3(-0.5f, -0.5f, 4.0f),
        glm::vec3(0.5f, -0.5f, 4.0f),
        glm::vec3(0.0f,  0.5f, 0.0f)
    };
    triangle.color = glm::vec3(0.6f, 0.2f, 0.2f);
    glUniform3f(colorLoc, triangle.color.x, triangle.color.y, triangle.color.z);
    triangle.model = glm::mat4(1.0f);
    Triangle triangle2;
    triangle2.vertices = { 
        glm::vec3(-0.5f, -0.5f, 3.0f),
        glm::vec3(0.5f, -0.5f, 3.0f),
        glm::vec3(0.0f,  0.5f, 0.0f)
    }; 
    triangle2.color = glm::vec3(1.0f, 1.0f, 0.7f);  
    glUniform3f(colorLoc, triangle.color.x, triangle.color.y, triangle.color.z);
    renderTriangle(triangle2, shaderProgram, ModelLoc, viewLoc, projectionLoc);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 MVP;\n"
        "uniform float xoffset;\n"
        "uniform float yoffset;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = MVP * vec4(aPos.x +xoffset, aPos.y +yoffset, aPos.z, 1.0);\n"
        "}\0";
    std::string fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\n\0";

    const char* vertexShaderSourceChar = vertexShaderSource.c_str();
    const char* fragmentShaderSourceChar = fragmentShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceChar, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceChar, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (triangle.vertices.size() / 3) * sizeof(glm::vec3), &triangle.vertices[0], GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glEnableVertexAttribArray(0);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    float aspectRatio = 640.0f / 480.0f;
    float fieldOfView = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    updateViewMatrix(view, cameraPos, cameraTarget, cameraUp);
    updateProjectionMatrix(projection, aspectRatio, fieldOfView, nearPlane, farPlane);

    while (!glfwWindowShouldClose(window))
    {
        clearBuffer();
        glUseProgram(shaderProgram);
        glValidateProgram(shaderProgram);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            yoffset += 0.035f;
        }
        yoffset -= 0.01f;
        if (yoffset < -0.15f) {
            yoffset = -0.15f;
        }
        playerPos.x = xoffset;
        playerPos.y = yoffset;
        xoffset = glm::clamp(xoffset, min_x, max_x);
        yoffset = glm::clamp(yoffset, min_y, 0.3f);
        glUniform1f(glGetUniformLocation(shaderProgram, "xoffset"), xoffset);
        glUniform1f(glGetUniformLocation(shaderProgram, "yoffset"), yoffset);
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        updateViewMatrix(view, playerPos + glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(playerPos.x, playerPos.y, 0.0f), cameraUp);
        updateProjectionMatrix(projection, aspectRatio, fieldOfView, nearPlane, farPlane);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        MVP = projection * view * triangle.model;
        unsigned int MVP_loc = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, glm::value_ptr(MVP));

        clearBuffer();
        renderTriangle(triangle, shaderProgram, modelLoc, viewLoc, projectionLoc);
        renderTriangle(triangle2, shaderProgram, ModelLoc, viewLoc, projectionLoc);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
    glfwTerminate();
    return 0;
}
