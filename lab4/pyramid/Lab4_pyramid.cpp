// Press W to switch between render modes.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <array>
#include <stdexcept>

#include "mylib.h"

void run();
GLFWwindow* createWindow();
void setGlSettings();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
static void createPyramid(float* center, float sizeX, float sizeY, float sizeZ, Mesh* mesh);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_NAME = "pyramid";

RenderMode RENDER_MODE = BOTH;
bool BUTTON_TRIGGERED_ONCE = false;

int main()
{
    try
    {
        run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}

void run()
{
    GLFWwindow* mainWindow = createWindow();
    setGlSettings();

    Mesh mesh = Mesh();
    float center[3] = { 0.0f, 0.0f, 0.0f };
    createPyramid(center, 0.5f, 0.5f, 0.5f, &mesh);

    Shader fill_shader("default_shader.vert", "default_fill_shader.frag");
    Shader line_shader("default_shader.vert", "default_line_shader.frag");
    Shader point_shader("default_shader.vert", "default_point_shader.frag");

    mesh.addShaderFill(&fill_shader);
    mesh.addShaderLine(&line_shader);
    mesh.addShaderPoint(&point_shader);
    mesh.addTexture("container.jpg");
    mesh.initialize();

    while (!glfwWindowShouldClose(mainWindow))
    {
        processInput(mainWindow);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.setRenderMode(RENDER_MODE);
        mesh.draw();

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();
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

GLFWwindow* createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    GLFWwindow* window = nullptr;
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, NULL, NULL);

    if (window == nullptr)
    {
        throw std::exception("ERROR::CREATE_CONTEXT: Failed to initialize GLAD.");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::exception("ERROR::CREATE_CONTEXT: Failed to initialize GLAD.");
    }

    return window;
}

void setGlSettings()
{
    glEnable(GL_DEPTH_TEST);
    glPointSize(12.0f);
    glLineWidth(2.0f);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


static void createPyramid(float* center, float sizeX, float sizeY, float sizeZ, Mesh* mesh) {
    float halfX = sizeX / 2.0f;
    float halfY = sizeY / 2.0f;
    float halfZ = sizeZ / 2.0f;

    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    std::vector<float> vertices = {
        // Base
        centerX - halfX, centerY - halfY, centerZ - halfZ, // Base-left-back   - 0
        centerX - halfX, centerY - halfY, centerZ + halfZ, // Base-left-front  - 1
        centerX + halfX, centerY - halfY, centerZ + halfZ, // Base-right-front - 2
        centerX + halfX, centerY - halfY, centerZ - halfZ, // Base-right-back  - 3

         // Back
        centerX - halfX, centerY - halfY, centerZ - halfZ, // Base-left-back   - 0
        centerX + halfX, centerY - halfY, centerZ - halfZ, // Base-right-back  - 3
        centerX        , centerY + halfY, centerZ        , // Tip              - 4 

         // Front
        centerX - halfX, centerY - halfY, centerZ + halfZ, // Base-left-front  - 1
        centerX + halfX, centerY - halfY, centerZ + halfZ, // Base-right-front - 2
        centerX        , centerY + halfY, centerZ        , // Tip              - 4 

         // Left
        centerX - halfX, centerY - halfY, centerZ - halfZ, // Base-left-back   - 0
        centerX - halfX, centerY - halfY, centerZ + halfZ, // Base-left-front  - 1
        centerX        , centerY + halfY, centerZ        , // Tip              - 4 

         // Right
        centerX + halfX, centerY - halfY, centerZ + halfZ, // Base-right-front - 2
        centerX + halfX, centerY - halfY, centerZ - halfZ, // Base-right-back  - 3
        centerX        , centerY + halfY, centerZ        , // Tip              - 4 
    };

    for (auto v : vertices)
    {
        mesh->addVertex(v);
    }

    std::vector<float> texture_coords = {
        // Base
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,

        // Front
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,

        // Left
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,

        // Right
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,
    };

    for (auto v : texture_coords)
    {
        mesh->addTextureCoord(v);
    }

    const std::vector<unsigned int> indices = {
        0, 1, 3, 2, 3, 1, // Base
        4, 5, 6, // Back
        7, 8, 9, // Front
        10, 11, 12, // Left
        13, 14, 15, // Right
    };

    for (auto v : indices)
    {
        mesh->addIndices(v);
    }

}



