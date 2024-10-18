// Press W to switch between render modes.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>

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
static void createSphere(float* center, float radius, unsigned int segments, unsigned int rings, Mesh* mesh);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;


const char* WINDOW_NAME = "sphere";

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
    createSphere(center, 0.5f, 12, 12, &mesh);

    Shader fill_shader("default_shader.vert", "default_fill_shader.frag");
    Shader line_shader("default_shader.vert", "default_line_shader.frag");
    Shader point_shader("default_shader.vert", "default_point_shader.frag");

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


static void createSphere(float* center, float radius, unsigned int segments, unsigned int rings, Mesh* mesh)
{
    const unsigned int MIN_SEGMENTS = 3;
    const unsigned int MIN_RINGS = 3;
    
    if (segments < MIN_SEGMENTS || rings < MIN_RINGS)
    {
        std::cout << "ERROR::CREATE_CONTEXT: Failed to initialize GLAD." << '\n';
        return;
    }

    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    float dTheta = M_PI / rings;
    float dPhi = (2 * M_PI) / segments;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Generate Vertices

    // North Pole
    vertices.push_back(0.0f);
    vertices.push_back(radius);
    vertices.push_back(0.0f);

    float pX, pY, pZ;
    for (int r = 1; r < rings; ++r) {
        float theta = r * dTheta;
        for (int s = 0; s <= segments; ++s) {
            float phi = s * dPhi;

            pX = radius * cos(phi) * sin(theta);
            pY = radius * cos(theta);
            pZ = radius * sin(phi) * sin(theta);

            vertices.push_back(pX);
            vertices.push_back(pY);
            vertices.push_back(pZ);

        }
    }

    // South Pole
    vertices.push_back(0.0f);
    vertices.push_back(-radius);
    vertices.push_back(0.0f);

    for (auto v : vertices) {
        mesh->addVertex(v);
    }

    // Generate Indices

    for (int i = 0; i < segments; ++i) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    for (int rIdx = 0; rIdx < rings - 2; ++rIdx) {
        for (int sIdx = 0; sIdx < segments; ++sIdx) {
            int p0 = rIdx * (segments + 1) + sIdx;
            int p1 = p0 + 1;
            int p2 = p0 + (segments + 1);
            int p3 = p2 + 1;

            indices.push_back(p0 + 1);
            indices.push_back(p1 + 1);
            indices.push_back(p2 + 1);

            indices.push_back(p1 + 1);
            indices.push_back(p3 + 1);
            indices.push_back(p2 + 1);
        }
    }


    int offset = (vertices.size() / 3 - 1) - segments -1;
    for (int i = 0; i < segments; ++i) {
        indices.push_back(vertices.size()/3 -1);
        indices.push_back(offset + i);
        indices.push_back(offset + i + 1);
    }

    for (auto idx : indices) {
        mesh->addIndices(idx);
    }





}
