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
static void createBox(float* center, float sizeX, float sizeY, float sizeZ, Mesh* mesh);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_NAME = "box";


RenderMode RENDER_MODE = BOTH;
bool BUTTON_TRIGGERED_ONCE = false;

int main()
{
  try
  {
    run();
  }
  catch(const std::exception& e)
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
    createBox(center, 0.5f, 0.5f, 0.5f, &mesh);

    Shader fill_shader("default_shader.vert", "default_fill_shader.frag");
    Shader line_shader("default_shader.vert", "default_line_shader.frag");
    Shader point_shader("default_shader.vert", "default_point_shader.frag");

    mesh.addTexture("container.jpg");
    mesh.addShaderFill(&fill_shader);
    mesh.addShaderLine(&line_shader);
    mesh.addShaderPoint(&point_shader);
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

static void createBox(float *center, float sizeX, float sizeY, float sizeZ, Mesh *mesh) {
    float hSizeX = sizeX / 2.0f;
    float hSizeY = sizeY / 2.0f;
    float hSizeZ = sizeZ / 2.0f;
    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    std::vector<float> vertices = {
        // aPos                                               aTexCoord

        // Back Face
        centerX - hSizeX, centerY - hSizeY, centerZ - hSizeZ, // Bottom-left-back - 0
        centerX + hSizeX, centerY - hSizeY, centerZ - hSizeZ, // Bottom-right-back - 1
        centerX + hSizeX, centerY + hSizeY, centerZ - hSizeZ, // Top-right-back - 2
        centerX - hSizeX, centerY + hSizeY, centerZ - hSizeZ, // Top-left-back - 3
                                                            
        // Front Face                                       
        centerX - hSizeX, centerY - hSizeY, centerZ + hSizeZ, // Bottom-left-front - 4
        centerX + hSizeX, centerY - hSizeY, centerZ + hSizeZ, // Bottom-right-front - 5
        centerX + hSizeX, centerY + hSizeY, centerZ + hSizeZ, // Top-right-front - 6
        centerX - hSizeX, centerY + hSizeY, centerZ + hSizeZ, // Top-left-front - 7

        // Left Face
        centerX - hSizeX, centerY - hSizeY, centerZ - hSizeZ, // Bottom-left-back - 8
        centerX - hSizeX, centerY - hSizeY, centerZ + hSizeZ, // Bottom-left-front - 10
        centerX - hSizeX, centerY + hSizeY, centerZ + hSizeZ, // Top-left-front - 11
        centerX - hSizeX, centerY + hSizeY, centerZ - hSizeZ, // Top-left-back - 9

        // Right Face
        centerX + hSizeX, centerY - hSizeY, centerZ + hSizeZ, // Bottom-right-front - 12
        centerX + hSizeX, centerY - hSizeY, centerZ - hSizeZ, // Bottom-right-back - 14
        centerX + hSizeX, centerY + hSizeY, centerZ - hSizeZ, // Top-right-back - 15
        centerX + hSizeX, centerY + hSizeY, centerZ + hSizeZ, // Top-right-front - 13

        // Bottom Face
        centerX - hSizeX, centerY - hSizeY, centerZ - hSizeZ, // Bottom-left-back - 16
        centerX - hSizeX, centerY - hSizeY, centerZ + hSizeZ, // Bottom-left-front - 18
        centerX + hSizeX, centerY - hSizeY, centerZ + hSizeZ, // Bottom-right-front - 19
        centerX + hSizeX, centerY - hSizeY, centerZ - hSizeZ, // Bottom-right-back - 17

        // Top Face
        centerX + hSizeX, centerY + hSizeY, centerZ - hSizeZ, // Top-right-back - 20
        centerX + hSizeX, centerY + hSizeY, centerZ + hSizeZ, // Top-right-front - 22
        centerX - hSizeX, centerY + hSizeY, centerZ + hSizeZ, // Top-left-front - 23
        centerX - hSizeX, centerY + hSizeY, centerZ - hSizeZ, // Top-left-back - 21
    };

    for (auto v : vertices)
    {
        mesh->addVertex(v);
    }

    std::vector<float> texture_coords = {
        // Back face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Front face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Left face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Right face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Bottom face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Top face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    for (auto v : texture_coords)
    {
        mesh->addTextureCoord(v);
    }

    const std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,   // Back face
        4, 5, 6, 6, 7, 4,   // Front face
        8, 9, 10, 10, 11, 8, // Left face
        12, 13, 14, 14, 15, 12, // Right face
        16, 17, 18, 18, 19, 16, // Bottom face
        20, 21, 22, 22, 23, 20  // Top face
    };

    for (auto v : indices) 
    {
        mesh->addIndices(v);
    }

}