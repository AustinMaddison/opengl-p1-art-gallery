#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <array>

#include "mylib.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void createPyramid(float* center, float sizeX, float sizeY, float sizeZ, Mesh* mesh);

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
    createPyramid(center, 0.5f, 0.5f, 0.5f, &mesh);

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

void createPyramid(float* center, float sizeX, float sizeY, float sizeZ, Mesh* mesh) {
    float halfX = sizeX / 2.0f;
    float halfY = sizeY / 2.0f;
    float halfZ = sizeZ / 2.0f;

    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    const std::vector<unsigned int> indices = {
        0, 1, 3, 2, 3, 1, // Base
        0, 3, 4, // Back
        1, 2, 4, // Front
        0, 1, 4, // Left
        2, 3, 4, // Right
    };

    for (auto v : indices)
    {
        mesh->addIndices(v);
    }

    std::vector<float> vertices = {
        centerX - halfX, centerY - halfY, centerZ - halfZ, // Base-left-back   - 0
        centerX - halfX, centerY - halfY, centerZ + halfZ, // Base-left-front  - 1
        centerX + halfX, centerY - halfY, centerZ + halfZ, // Base-right-front - 2
        centerX + halfX, centerY - halfY, centerZ - halfZ, // Base-right-back  - 3
        centerX        , centerY + halfY, centerZ        , // Tip                - 4 
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