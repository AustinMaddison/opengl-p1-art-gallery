#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "shader.hpp"
#include "camera.hpp"

GLFWwindow* createWindow();
void setGlGlobalSettings();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void processCameraCollision(Camera* camera);

/* -------------------------------- Settings -------------------------------- */
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;
const char* WINDOW_NAME = "Art Gallery";

/* --------------------------------- Player --------------------------------- */
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
glm::vec4 cameraCollisonBounds(glm::vec4(-1, 1, -1, 1) * 5.0f);  // x-min, x-max, z-min, z-max


/* ---------------------------------- Time ---------------------------------- */
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
    /* ------------------- Create OpenGL Context and Windowing ------------------ */
    GLFWwindow* mainWindow = createWindow();
    setGlGlobalSettings();


    /* ---------------------------- Create Materials ---------------------------- */
    
    // Floor Material
    unsigned int floorDiffuseTexture = loadTexture("resources/textures/enviroment/floor.jpg");
    unsigned int floorSpecularTexture = loadTexture("resources/textures/enviroment/floor.jpg");
    Shader floorShader("src/shaders/default.vert", "src/shaders/default.frag");
    floorShader.use();
    floorShader.setInt("material.diffuse", 0);
    floorShader.setInt("material.specular", 1);
    floorShader.setFloat("material.shininess", 32.0f);
    floorShader.setInt("material.sampleSpace", 1);
    floorShader.setVec3("material.scale", glm::vec3(1.0f));
    floorShader.setVec3("material.translate", glm::vec3(0.0f));

    /* --------------------------- Primitives Vertcies -------------------------- */
    // layout: Pos vec3, Normals vec3, TexCoords vec2 

    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    float planeXZVertices[] = {
        -0.5f,  0.0f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 
         0.5f,  0.0f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 
         0.5f,  0.0f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 
         0.5f,  0.0f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 
        -0.5f,  0.0f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 
        -0.5f,  0.0f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 
    };


    /* ----------------------- Create VAOs From Primitives ---------------------- */

    // Cube
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Plane
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeXZVertices), &planeXZVertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);


    /* -------------------------------------------------------------------------- */
    /*                                  Main Loop                                 */
    /* -------------------------------------------------------------------------- */
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Update time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Window and Player Input
        processInput(mainWindow);
        processCameraCollision(&camera);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* -------------------------------------------------------------------------- */
        /*                                Render Scene                                */
        /* -------------------------------------------------------------------------- */


        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        /* --------------------------------- Lights --------------------------------- */

        glm::vec3 lightPos(0.0f, 3.0f, 0.0f);
        glm::vec3 lightDir(0.0f, -1.0f, 0.0f);


        /* ---------------------------------- Floor --------------------------------- */
        floorShader.use();
        floorShader.setMat4("model", glm::scale(model, glm::vec3(1.0f) * 5.0f));
        floorShader.setMat4("view", view);
        floorShader.setMat4("projection", projection);
        floorShader.setVec3("viewPos", camera.Position);

        floorShader.setVec3("light.position",lightPos);
        floorShader.setVec3("light.direction", lightDir);
        floorShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        floorShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        
        floorShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        floorShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        floorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("light.constant", 1.0f);
        floorShader.setFloat("light.linear", 0.09f);
        floorShader.setFloat("light.quadratic", 0.032f);

        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorDiffuseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, floorSpecularTexture);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);





        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processCameraCollision(Camera* camera)
{
    // x-min
    if (camera->Position.x < cameraCollisonBounds.x)
       camera->Position.x = cameraCollisonBounds.x;
    // x-max
    if (camera->Position.x > cameraCollisonBounds.y)
        camera->Position.x = cameraCollisonBounds.y;
    // z-min
    if (camera->Position.z < cameraCollisonBounds.z)
        camera->Position.z = cameraCollisonBounds.z;
    // z-max
    if (camera->Position.z > cameraCollisonBounds.w)
        camera->Position.z = cameraCollisonBounds.w;
}


void processInput(GLFWwindow* window)
{
    // Close Window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera Movement
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
    float yoffset = lastY - ypos; 

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

    // Input
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load Opengl Function Pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return window;
    }

    return window;
}

void setGlGlobalSettings()
{
    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
}


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



