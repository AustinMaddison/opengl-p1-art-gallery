#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <array>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void createBox(float center[3], float width, float height, float depth, float* vertices, unsigned int* indices);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

enum RenderMode {
    FILL,
    WIREFRAME,
    BOTH
};

RenderMode RENDER_MODE = FILL;
bool BUTTON_TRIGGERED_ONCE = false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    Shader wireframe_shader("default_shader.vert", "default_line_shader.frag");
    Shader fill_shader("default_shader.vert", "default_fill_shader.frag");

    float center[3] = { 0.0f, 0.0f, 0.0f };
    float vertices[24];
    unsigned int indices[36];
    createBox(center, 0.5f, 0.5f, 0.5f, vertices, indices);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);

	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

    glPointSize(10.0f);
    glLineWidth(2.0f);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
         glClearColor(0.f, 0.f, 0.f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        switch (RENDER_MODE)
        {
        case FILL:
            fill_shader.setFloat("uTime", glfwGetTime());
            fill_shader.use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);
            break;
        
        case WIREFRAME:
            wireframe_shader.setFloat("uTime", glfwGetTime());
            wireframe_shader.use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);
            break;
        
        case BOTH:
            fill_shader.setFloat("uTime", glfwGetTime());
            fill_shader.use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);

            wireframe_shader.setFloat("uTime", glfwGetTime());
            wireframe_shader.use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_UNSIGNED_INT, 0);
            break;

        default:
            break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// Takes in the center of box, the width, height, depth and make the vertices
void createBox(float center[3], float width, float height, float depth, float* vertices, unsigned int* indices) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;
    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    const unsigned int tempIndices[] = {
        0, 1, 3, 2, 3, 1, // Back face
        4, 5, 7, 6, 7, 5, // Front face
        7, 4, 0, 7, 3, 0, // Left face
        6, 5, 1, 1, 2, 6, // Right face
        4, 5, 1, 1, 0, 4, // Bottom face
        2, 3, 6, 3, 7, 6  // Top face
    };

    for (int i = 0; i < 36; ++i) {
        indices[i] = tempIndices[i];
    }

    float tempVertices[] = {
        centerX - halfWidth, centerY - halfHeight, centerZ - halfDepth, // Bottom-left-back - 0
        centerX + halfWidth, centerY - halfHeight, centerZ - halfDepth, // Bottom-right-back - 1
        centerX + halfWidth, centerY + halfHeight, centerZ - halfDepth, // Top-right-back - 2
        centerX - halfWidth, centerY + halfHeight, centerZ - halfDepth, // Top-left-back - 3
        centerX - halfWidth, centerY - halfHeight, centerZ + halfDepth, // Bottom-left-front - 4
        centerX + halfWidth, centerY - halfHeight, centerZ + halfDepth, // Bottom-right-front - 5
        centerX + halfWidth, centerY + halfHeight, centerZ + halfDepth, // Top-right-front - 6
        centerX - halfWidth, centerY + halfHeight, centerZ + halfDepth  // Top-left-front - 7
    };
    
    for (int i = 0; i < 24; ++i) {
        vertices[i] = tempVertices[i];
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