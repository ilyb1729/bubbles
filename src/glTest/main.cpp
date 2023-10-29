#include <debugbreak.h>

#include <cstddef>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glTest/camera.h>
#include <glTest/shader.h>
#include <glTest/mesh.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

using namespace std::literals;

// empty constructor????
GLuint gVertexArrayPositionColor = {}; // vao
GLuint gVertexArrayLight = {};
GLuint gVertexBuffer = {}; // vbo
GLuint gIndexBuffer = {};  // veo
GLuint texture1 = {};
GLuint texture2 = {};

Shader *gProgram = {};
Shader *gLight = {};

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.0f, 0.0f, -3.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0.0f; // roll not implemented
float fov = 45.0f;
bool firstMouse = true;

std::vector<Vertex> gVertices = {Vertex{.Position = glm::vec3{-0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 0.0f, -1.0f}, .Texture = glm::vec2{0.0f, 0.0f}}, //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 0.0f, -1.0f}, .Texture = glm::vec2{1.0f, 0.0f}},  //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 0.0f, -1.0f}, .Texture = glm::vec2{1.0f, 1.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 0.0f, -1.0f}, .Texture = glm::vec2{1.0f, 1.0f}},   //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 0.0f, -1.0f}, .Texture = glm::vec2{0.0f, 1.0f}},  //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 0.0f, -1.0f}, .Texture = glm::vec2{0.0f, 0.0f}}, //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 0.0f, 1.0f}, .Texture = glm::vec2{0.0f, 0.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 0.0f, 1.0f}, .Texture = glm::vec2{1.0f, 0.0f}},    //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 0.0f, 1.0f}, .Texture = glm::vec2{1.0f, 1.0f}},     //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 0.0f, 1.0f}, .Texture = glm::vec2{1.0f, 1.0f}},     //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 0.0f, 1.0f}, .Texture = glm::vec2{0.0f, 1.0f}},    //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 0.0f, 1.0f}, .Texture = glm::vec2{0.0f, 0.0f}},   //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{-1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},   //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{-1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{1.0f, 1.0f}},  //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{-1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}}, //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{-1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}}, //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{-1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{0.0f, 0.0f}},  //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{-1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},     //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{1.0f, 1.0f}},    //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{0.0f, 0.0f}},    //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{1.0f, 0.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},     //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{0.0f, -1.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}}, //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{0.0f, -1.0f, 0.0f}, .Texture = glm::vec2{1.0f, 1.0f}},  //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{0.0f, -1.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{0.0f, -1.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},   //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, 0.5f}, .Normal = glm::vec3{0.0f, -1.0f, 0.0f}, .Texture = glm::vec2{0.0f, 0.0f}},  //
                                 Vertex{.Position = glm::vec3{-0.5f, -0.5f, -0.5f}, .Normal = glm::vec3{0.0f, -1.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}}, //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 1.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}},   //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 1.0f, 0.0f}, .Texture = glm::vec2{1.0f, 1.0f}},    //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 1.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},     //
                                 Vertex{.Position = glm::vec3{0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 1.0f, 0.0f}, .Texture = glm::vec2{1.0f, 0.0f}},     //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, 0.5f}, .Normal = glm::vec3{0.0f, 1.0f, 0.0f}, .Texture = glm::vec2{0.0f, 0.0f}},    //
                                 Vertex{.Position = glm::vec3{-0.5f, 0.5f, -0.5f}, .Normal = glm::vec3{0.0f, 1.0f, 0.0f}, .Texture = glm::vec2{0.0f, 1.0f}}};  //

std::vector<glm::vec3> cubePositions = {
    glm::vec3(0.0f, 0.0f, 0.0f),     //
    glm::vec3(2.0f, 5.0f, -15.0f),   //
    glm::vec3(-1.5f, -2.2f, -2.5f),  //
    glm::vec3(-3.8f, -2.0f, -12.3f), //
    glm::vec3(2.4f, -0.4f, -3.5f),   //
    glm::vec3(-1.7f, 3.0f, -7.5f),   //
    glm::vec3(1.3f, -2.0f, -2.5f),   //
    glm::vec3(1.5f, 2.0f, -2.5f),    //
    glm::vec3(1.5f, 0.2f, -1.5f),    //
    glm::vec3(-1.3f, 1.0f, -1.5f)    //
};

// std::vector<std::uint32_t> gIndices = {0, 1, 3, 1, 2, 3}; // not being used

void CleanupRender() {
    glDeleteBuffers(1, &gVertexBuffer);
    glDeleteBuffers(1, &gIndexBuffer);
    glDeleteVertexArrays(1, &gVertexArrayPositionColor);

    delete gProgram;
}

bool LoadTexture(const char *filename, GLuint &texture) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        spdlog::error("Failed to load texture"); // this feels like it should not be here by standard of his other code
        return false;
    }
    stbi_image_free(data);
    return true;
}

bool SetupRender() {

    glGenBuffers(1, &gVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * gVertices.size(), gVertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &gVertexArrayPositionColor); // does this need to be above the creation of VBO
    glBindVertexArray(gVertexArrayPositionColor);

    // glGenBuffers(1, &gIndexBuffer);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer); // VAO has to be bound before the element array buffer, use properties from the VAO i guess
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * gIndices.size(), gIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Texture));

    glGenVertexArrays(1, &gVertexArrayLight);
    glBindVertexArray(gVertexArrayLight); // newly added code

    glEnableVertexAttribArray(0); // can i put this in 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));

    stbi_set_flip_vertically_on_load(true);
    LoadTexture("C:\\Users\\porem\\OneDrive\\Desktop\\School\\projs\\OpenGL-Getting-Started\\src\\glTest\\textures\\karina.png", texture1);
    // LoadTexture("C:\\Users\\porem\\OneDrive\\Desktop\\School\\projs\\OpenGL-Getting-Started\\src\\glTest\\textures\\kayguya.jpg", texture2);

    // turn this into a try catch
    gProgram = Shader("C:\\Users\\porem\\OneDrive\\Desktop\\School\\projs\\OpenGL-Getting-Started\\src\\glTest\\shaders\\lightedVS.glsl",
                      "C:\\Users\\porem\\OneDrive\\Desktop\\School\\projs\\OpenGL-Getting-Started\\src\\glTest\\shaders\\lightedFS.glsl");
    gProgram.use();
    gProgram.setFloat("material.shininess", 32.0f);
    gProgram.setInt("material.diffuse", 0);
    gProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    gProgram.setVec3("light.ambient", 0.01f, 0.01f, 0.01f);
    gProgram.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    gProgram.setFloat("light.constant", 1.0f);
    gProgram.setFloat("light.linear", 0.09f);
    gProgram.setFloat("light.quadratic", 0.32f);

    gLight = Shader("C:\\Users\\porem\\OneDrive\\Desktop\\School\\projs\\OpenGL-Getting-Started\\src\\glTest\\shaders\\lightVS.glsl",
                    "C:\\Users\\porem\\OneDrive\\Desktop\\School\\projs\\OpenGL-Getting-Started\\src\\glTest\\shaders\\lightFS.glsl");
    gLight.use();

    return true;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gProgram.use();
    gProgram.setVec3("view_position", cameraPosition);
    gProgram.setVec3("light.position", cameraPosition);
    gProgram.setVec3("light.direction", cameraFront);
    gProgram.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
    gProgram.setFloat("light.outercutoff", glm::cos(glm::radians(17.5f)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    // oscillation of blending
    float time = static_cast<float>(glfwGetTime());
    float stime = (sin(time) + 1.f) / 2;
    gProgram.setFloat("s_time", stime);

    // glm::vec3 lightColor; // uh need to clean up where i have this light stuff
    // lightColor.x = sin(time * 2.0f);
    // lightColor.y = sin(time * 0.7f);
    // lightColor.z = sin(time * 1.3f);
    // glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
    // gProgram.setVec3("light.ambient", ambientColor);
    // gProgram.setVec3("light.diffuse", diffuseColor);

    // set frame time
    deltaTime = time - lastFrame;
    lastFrame = time;

    // view
    glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    gProgram.setMat4("view", view);

    // projection
    glm::mat4 projection = glm::perspective(glm::radians(fov), 800.f / 600.f, .1f, 100.f);
    gProgram.setMat4("projection", projection);

    glBindVertexArray(gVertexArrayPositionColor);

    for (unsigned int i = 0; i < 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle) + time * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

        gProgram.setMat4("model", model); // glGetUniformLocation(gProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // gLight.use();
    // gLight.setMat4("view", view);
    // gLight.setMat4("projection", projection);
    // gLight.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f))); // figure out the order of matrices
    // glBindVertexArray(gVertexArrayLight);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DebugMessageCallback([[maybe_unused]] GLenum source, GLenum type, [[maybe_unused]] GLuint id, [[maybe_unused]] GLenum severity, [[maybe_unused]] GLsizei messageLength, const GLchar *message, const void *) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        spdlog::error("GL: DEBUG CALLBACK type = {}, severity = error, message = {}\n", type, message);
        debug_break();
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }
    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    fov -= static_cast<float>(yoffset);
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
}

int main(int, char **) {
    if (glfwInit() == GLFW_FALSE) {
        spdlog::error("GLFW: Unable to initialize");
        return 1;
    }

    glfwSetErrorCallback([]([[maybe_unused]] auto errorCode, auto errorMessage) { spdlog::error("GLFW: Error {0}", errorMessage); });

    auto windowWidth = 1650;
    auto windowHeight = 1050;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    auto windowHandle = glfwCreateWindow(windowWidth, windowHeight, "Basic Triangle nonDSA", nullptr, nullptr);
    if (windowHandle == nullptr) {
        spdlog::error("GLFW: Unable to create window");
        glfwTerminate();
        return 1;
    }

    const auto primaryMonitor = glfwGetPrimaryMonitor();
    const auto primaryMonitorResolution = glfwGetVideoMode(primaryMonitor);
    glfwSetWindowPos(windowHandle, primaryMonitorResolution->width / 2 - windowWidth / 2, primaryMonitorResolution->height / 2 - windowHeight / 2);

    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(windowHandle, mouseCallback);
    glfwSetScrollCallback(windowHandle, scrollCallback);

    glfwMakeContextCurrent(windowHandle);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugMessageCallback, nullptr);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    if (!SetupRender()) {
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(windowHandle)) {
        glfwPollEvents();
        processInput(windowHandle);

        Render();

        glfwSwapBuffers(windowHandle);
    }

    CleanupRender();

    glfwDestroyWindow(windowHandle);
    glfwTerminate();

    return 1;
}