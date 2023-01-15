#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad.c>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <string>
#include <vector>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        Position = position;
        Front = front;
        Up = up;
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= 0.1f;
        yoffset *= 0.1f;
        Front.x = cos(glm::radians(yoffset)) * cos(glm::radians(xoffset));
        Front.y = sin(glm::radians(yoffset));
        Front.z = cos(glm::radians(yoffset)) * sin(glm::radians(xoffset));
        Front = glm::normalize(Front);
    }
};
void clearBuffer(int width, int height)
{
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}
bool initWindow(GLFWwindow*& window, int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

std::string vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 rotation;\n"
"uniform vec3 cameraFront;\n"
"uniform vec3 viewPos;\n"
"uniform mat4 projection;\n"
"out vec3 Normal;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * rotation * model * vec4(aPos, 1.0);\n"
"   Normal = aNormal;\n"
"   TexCoord = aTexCoord;\n"
"}\0";
std::string fragmentShaderSource =
"#version 330 core\n"
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main()\n"
"{\n"
"   vec3 diffuse = vec3(0.5, 0.0, 0.3);\n"
"   FragColor = vec4(diffuse, 1.0);\n"
"}\n\0";

int width, height;
int totalVertices = 0;
unsigned int VBO, EBO;
unsigned int VAO;
float xRotation = 0.0f;
float yRotation = 0.0f;
std::vector<unsigned int> indices;
std::vector<float> vertexData;

bool loadModel(const std::string& path, std::vector<float>& vertexData, std::vector<unsigned int>& indices) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return false;
    }
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D pos = mesh->mVertices[j];
            vertexData.push_back(pos.x);
            vertexData.push_back(pos.y);
            vertexData.push_back(pos.z);

            if (mesh->HasNormals()) {
                aiVector3D normal = mesh->mNormals[j];
                vertexData.push_back(normal.x);
                vertexData.push_back(normal.y);
                vertexData.push_back(normal.z);
            }

            if (mesh->HasTextureCoords(0)) {
                aiVector3D uv = mesh->mTextureCoords[0][j];
                vertexData.push_back(uv.x);
                vertexData.push_back(uv.y);
            }
        }
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                indices.push_back(face.mIndices[k]);
            }
        }
        }
    }

int main() {
    Camera camera(glm::vec3(3.0f, 0.0f, 3.0f));
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }
    GLFWwindow* window = glfwCreateWindow(800, 600, "SS : Model Viewer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }
    glfwSetCursorPos(window, width / 2, height / 2);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    const char* version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSourceC = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Error compiling vertex shader: " << infoLog << std::endl;
        return 1;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSourceC = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader thrown.\n";
    }
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Error compiling fragment shader: " << infoLog << std::endl;
        return 1;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error linking shader program: " << infoLog << std::endl;
        return 1;
    }
  
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("model.obj", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading scene: " << importer.GetErrorString() << std::endl;
        return 1;
    }
    glBindVertexArray(VAO);
   
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        totalVertices += mesh->mNumVertices;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D position = mesh->mVertices[j];
            vertexData.push_back(position.x);
            vertexData.push_back(position.y);
            vertexData.push_back(position.z);
            if (mesh->HasNormals()) {
                aiVector3D normal = mesh->mNormals[j];
                vertexData.push_back(normal.x);
                vertexData.push_back(normal.y);
                vertexData.push_back(normal.z);
            }
            if (mesh->HasTextureCoords(0)) {
                aiVector3D texCoord = mesh->mTextureCoords[0][j];
                vertexData.push_back(texCoord.x);
                vertexData.push_back(texCoord.y);
            }
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; k++) {
                    indices.push_back(face.mIndices[k]);
                }
            }
        }
        bool success = loadModel("model.obj", vertexData, indices);
        if (!success) {
            return 1;
        }
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        std::vector<float> vertexData;
        std::vector<unsigned int> indices;
        loadModel("model.obj", vertexData, indices);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        if (mesh->HasNormals()) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
        }
        if (mesh->HasTextureCoords(0)) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
        }
    }
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    importer.FreeScene();
    glEnable(GL_DEPTH_TEST);

    glm::mat4 rotation;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
    camera.ProcessMouseMovement((float)xpos, (float)ypos, true);
    glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.0f), glm::vec3(-0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    float fov = 45.0f;
    float aspect = (float)width / (float)height;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glLoadIdentity();                 // Reset the model-view matrix
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    float maxAnglePerFrame = 0.001f;
    float limitAngle = 0.002f;
    float dampingFactor = 0.0001f;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glValidateProgram(shaderProgram);
        model = glm::rotate(model, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            yRotation += maxAnglePerFrame;
            if (yRotation > limitAngle) {
                yRotation = limitAngle;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            yRotation -= maxAnglePerFrame;
            if (yRotation < -limitAngle) {
                yRotation = -limitAngle;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            xRotation += maxAnglePerFrame;
            if (xRotation > limitAngle) {
                xRotation = limitAngle;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            xRotation -= maxAnglePerFrame;
            if (xRotation < -limitAngle) {
                xRotation = -limitAngle;
            }
        }
   
        rotation = glm::rotate(glm::mat4(1.0f), xRotation, glm::vec3(1.0f, 0.0f, 0.0f))* glm::rotate(glm::mat4(1.0f), yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotation"), 1, GL_FALSE, glm::value_ptr(rotation));

        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(shaderProgram, "cameraFront"), camera.Front.x, camera.Front.y, camera.Front.z);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        clearBuffer(width, height);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glfwTerminate();
    return 0;
}
