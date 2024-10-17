#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <array>

#include "mylib.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void createBox(float center[3], float width, float height, float depth, Mesh *mesh);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

RenderMode RENDER_MODE = BOTH;
bool BUTTON_TRIGGERED_ONCE = false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "box", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Mesh mesh = Mesh();
    float center[3] = { 0.0f, 0.0f, 0.0f };
    createBox(center, 0.5f, 0.5f, 0.5f, &mesh);

    Shader wireframe_shader("default_shader.vert", "default_line_shader.frag");
    Shader fill_shader("default_shader.vert", "default_fill_shader.frag");

    mesh.addShaderFill(&fill_shader);
    mesh.addShaderLine(&wireframe_shader);
    mesh.initialize();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glPointSize(10.0f);
    glLineWidth(2.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mesh.renderMode = RENDER_MODE;
        mesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void createBox(float center[3], float width, float height, float depth, Mesh *mesh) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;
    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    const std::vector<unsigned int> indices = {
        0, 1, 3, 2, 3, 1, // Back face
        4, 5, 7, 6, 7, 5, // Front face
        7, 4, 0, 7, 3, 0, // Left face
        6, 5, 1, 1, 2, 6, // Right face
        4, 5, 1, 1, 0, 4, // Bottom face
        2, 3, 6, 3, 7, 6,  // Top face
    };

    for (auto v : indices) 
    {
        mesh->addIndices(v);
    }

    std::vector<float> vertices= {
        centerX - halfWidth, centerY - halfHeight, centerZ - halfDepth, // Bottom-left-back - 0
        centerX + halfWidth, centerY - halfHeight, centerZ - halfDepth, // Bottom-right-back - 1
        centerX + halfWidth, centerY + halfHeight, centerZ - halfDepth, // Top-right-back - 2
        centerX - halfWidth, centerY + halfHeight, centerZ - halfDepth, // Top-left-back - 3
        centerX - halfWidth, centerY - halfHeight, centerZ + halfDepth, // Bottom-left-front - 4
        centerX + halfWidth, centerY - halfHeight, centerZ + halfDepth, // Bottom-right-front - 5
        centerX + halfWidth, centerY + halfHeight, centerZ + halfDepth, // Top-right-front - 6
        centerX - halfWidth, centerY + halfHeight, centerZ + halfDepth  // Top-left-front - 7
    };
    
    for (auto v : vertices) 
    {
        mesh->addVertex(v);
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (!BUTTON_TRIGGERED_ONCE)
        {
            RENDER_MODE = (RenderMode)((RENDER_MODE + 1) % 3);
            BUTTON_TRIGGERED_ONCE = true;
        }
    }

    if (BUTTON_TRIGGERED_ONCE && glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
    {
        BUTTON_TRIGGERED_ONCE = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}