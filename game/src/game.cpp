// Press W to switch between render modes.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <array>
#include <stdexcept>
#include <vector>

#include "model.hpp"
#include "camera.hpp"
#include "box.hpp"
#include "math3d.hpp"
#include "box.hpp"

void run();
GLFWwindow* createWindow();
void setGlSettings();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;
const char* WINDOW_NAME = "Art Gallery";

Camera camera(Vector3D(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


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

    Model box = Model();
    createBox(Vector3D(0.0f, 0.0f, 0.0f), 1.0, 0, 0, &box);

    Shader shader = Shader("resource/default_shader.vert", "resource/default_fill_shader.frag");
    
    box.addShaderFill(&shader);
    box.addTexture("resource/girl.jpg");
    // create game room
    Model model = Model();
    float dimensions = 1.0f;
    float floor = -(dimensions/2);
    float center[3] = { 0.0f, floor, 0.0f };
    createBox(center, dimensions, dimensions, dimensions, &model);

    Shader fill_shader("../resources/shaders/default_shader.vert", "../resources/shaders/default_fill_shader.frag");
    model.addShaderFill(&fill_shader);

    model.addTexture("../resources/textures/tileset/00.png");
    model.initialize();


    while (!glfwWindowShouldClose(mainWindow))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(mainWindow);

        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            
        Matrix4D projection = Perspective(radians(45.0f), (float)SCR_HEIGHT / (float)SCR_WIDTH, 0.1f, 100.0f );
         box.shader->setMatrix4D("projection", projection);   
        box.shader->setMatrix4D("view", camera.GetViewMatrix());

        Matrix4D model(1.0);
        box.shader->setMatrix4D("model", model);


        box.draw();
        model.draw();

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
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
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
        std::cerr << "Failed to create GLFWwindow" << std::endl;
        glfwTerminate();
        return window;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return window;
    }

    return window;
}

void setGlSettings()
{
    glEnable(GL_DEPTH_TEST);
    glPointSize(12.0f);
    glLineWidth(2.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
}



