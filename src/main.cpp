#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "lib/Shader.h"

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void drawMenger(float _x, float _y, float _z, int depth, float size, int modelLoc, float rotationX, float rotationY);

// Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // Initialization of GLFW Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // GLFW 3.3
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required for macOS
#endif

    // Create window variable
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GP_1", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create a context for the window
    glfwMakeContextCurrent(window);
    // Change the draw range when changing the window size
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Change style of the ImGui
    ImGuiStyle& style = ImGui::GetStyle();

    // Change sliders and buttons appearance
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(.75f, .0f, 1.0f, .6f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(.75f, .0f, 1.0f, .6f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(.05f, .0f, .25f, .5f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(.4f, .0f, .8f, .6f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(.4f, .0f, .8f, 1.0f);

    // Change window appearance
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(.5f, .0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(.05f, .0f, .2f, .4f);
    style.Colors[ImGuiCol_Border] = ImVec4(.5f, .0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(.05f, .0f, .2f, .4f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(.8f, .0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(.8f, .0f, 1.0f, 1.0f);

    // Change resize grip appearance
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(.1f, .0f, .3f, .4f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(.4f, .0f, .65f, .7f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(.5f, .0f, 1.0f, 1.0f);

    //ImGui::StyleColorsDark();

    ImVec4 cubeColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    bool toolWindow = true;

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Build and compile shader program
    Shader shader("res/shaders/basic.vert", "res/shaders/basic.frag");

    // Set up vertex data and buffers
    GLfloat vertices[] = {
            // Position                         // Texture
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture 1 - stone.jpg
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and create mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("res/textures/rainbow.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);

    // Texture 2 - polish_cow.jpg
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Flip the texture
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("res/textures/okey.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    int recursionLevel = 0;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float zoomLevel = -3.0f;

    // Create transformations
    glm::mat4 model         = glm::mat4(1.0f);      // Make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    // Retrieve the matrix uniform locations
    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
    GLint viewLoc = glGetUniformLocation(shader.ID, "view");
    GLint projectionLoc = glGetUniformLocation(shader.ID, "projection");
    GLint colorLoc = glGetUniformLocation(shader.ID, "color");

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, zoomLevel));
    projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

    // Game loop - update buffers to create shapes and animations
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.875f, 0.8f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (toolWindow) {
            ImGui::Begin("Tools");
            ImGui::SliderFloat("Rotation X", &rotationX, -180, 180);
            ImGui::SliderFloat("Rotation Y", &rotationY, -180, 180);
            ImGui::SliderFloat("Zoom Level", &zoomLevel, -6, 0);
            ImGui::SliderInt("Recursion Level", &recursionLevel, 0, 4);
            ImGui::ColorEdit3("Cube Color", (float*)&cubeColor);
            ImGui::Text("Framerate: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform4f(colorLoc, cubeColor.x, cubeColor.y, cubeColor.z, cubeColor.w);

        view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, zoomLevel));

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Render Menger Sponge
        drawMenger(0, 0, 0, recursionLevel, 1, modelLoc, rotationX, rotationY);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap context and buffer
        glfwPollEvents();
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminate all resources used to create window
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void drawMenger(float _x, float _y, float _z, int depth, float size, int modelLoc, float rotationX, float rotationY) {
    if (depth == 0) {
        glm::mat4 newModel = glm::mat4(1.0f);
        newModel = glm::rotate(newModel, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        newModel = glm::rotate(newModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        newModel = glm::scale(newModel, glm::vec3(size));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(newModel));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        return;
    }

    float offset = size / 3.0f;

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            for (int z = -1; z < 2; z++) {
                int sum = abs(x) + abs(y) + abs(z);

                if (depth == 1 && sum > 1) {
                    glm::mat4 newModel = glm::mat4(1.0f);
                    newModel = glm::rotate(newModel, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
                    newModel = glm::rotate(newModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
                    newModel = glm::translate(newModel, glm::vec3(_x + (float)x * offset, _y + (float)y * offset, _z + (float)z * offset));
                    newModel = glm::scale(newModel, glm::vec3(offset));

                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(newModel));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                } else if (depth > 1 && sum > 1) {
                    drawMenger(_x + (float)x * offset, _y + (float)y * offset, _z + (float)z * offset, depth - 1, offset, modelLoc, rotationX, rotationY);
                }
            }
        }
    }
}