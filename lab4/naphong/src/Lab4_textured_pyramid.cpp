#include "config.h"
#include "material.h"
#include <iostream>
#include <cmath>

// PSA: Camera code is from chatgpt but all logic is from me

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void updateCameraPosition();  // Camera movement logic

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera settings
float cameraAngle = 0.0f;  // angle around the box
float cameraHeight = 0.0f; // height of the camera
float radius = 5.0f;       // distance from the box
float cameraSpeed = 0.05f; // how fast the camera moves

// simple matrix multiplications
void matrixMultiply(float* result, const float* mat1, const float* mat2);

// function to generate identity matrix
void loadIdentity(float* matrix);

// function to generate perspective matrix
void perspective(float* matrix, float fov, float aspect, float near, float far);

// function to generate look-at matrix
void lookAt(float* matrix, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);

// Shaders
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 fragmentTexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   fragmentTexCoord = vec2(1.0, -1.0) * aTexCoord;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec2 fragmentTexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D materials[5];\n"
    "uniform int faceIndex;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(texture(materials[faceIndex], fragmentTexCoord).rgb, 1.0);\n"
    "}\n\0";

// Pyramid vertices and texture and indices
void createPyramid(float center[3], float width, float height, float* vertices) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;
    float x = center[0];
    float y = center[1];
    float z = center[2];

    // Define the 5 faces of the pyramid
    float tempVertices[] = {
        // Bottom Face
        x - hw, y - hh, z - hw, // 0 - back left
        x + hw, y - hh, z - hw, // 1 - back right
        x - hw, y - hh, z + hw, // 2 - front left
        x + hw, y - hh, z + hw, // 3 - front right

        // Front Face
        x - hw, y - hh, z + hw, // 4 - front left
        x + hw, y - hh, z + hw, // 5 - front right
        x, y + hh, z,           // 6 - top
        
        // Back Face
        x - hw, y - hh, z - hw, // 7 - back left
        x + hw, y - hh, z - hw, // 8 - back right
        x, y + hh, z,           // 9 - top

        // Left Face
        x - hw, y - hh, z - hw, // 10 - back left
        x - hw, y - hh, z + hw, // 11 - front left
        x, y + hh, z,           // 12 - top

        // Right Face
        x + hw, y - hh, z - hw, // 13 - back right
        x + hw, y - hh, z + hw, // 14 - front right
        x, y + hh, z           // 15 - top
    };

    // Copy the vertices to the provided array
    for (int i = 0; i < 48; ++i) {
        vertices[i] = tempVertices[i];
    }
}

void getTextureCoordinates(float *texturecoords) {
    float tempTexCoords[] = {
        // Bottom face
        0.0f, 0.0f, // back left
        1.0f, 0.0f, // back right
        0.0f, 1.0f, // front left
        1.0f, 1.0f, // front right

        // Front face
        0.0f, 0.0f, // front left
        1.0f, 0.0f, // front right
        0.5f, 1.0f, // top

        // Back face
        0.0f, 0.0f, // back left
        1.0f, 0.0f, // back right
        0.5f, 1.0f, // top

        // Left Face
        0.0f, 0.0f, // back left
        0.75f, 0.0f, // front left
        0.25f, 1.0f, // top

        // Right Face
        1.0f, 0.0f, // back right
        0.0f, 0.0f, // front right
        0.5f, 1.0f, // top
    };

    for (int i = 0; i < 32; ++i) {
        texturecoords[i] = tempTexCoords[i];
    }
}

void getIndices(unsigned int* indices) {
    unsigned int tempIndices[] = {
        0, 1, 2, 1, 2, 3,   // Bottom face
        4, 5, 6,            // Front face
        7, 8, 9,            // Back face
        10, 11, 12,         // Left face
        13, 14, 15          // Right face
    };

    // Copy the indices to the provided array
    for (int i = 0; i < 18; ++i) {
        indices[i] = tempIndices[i];
    }
}

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Camera Movement", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Pyramid vertices and texture and indices
    float vertices[48];
    float center[3] = {0.0f, 0.0f, 0.0f};
    createPyramid(center, 1.0f, 1.0f, vertices);

    float texture[32]; //bottom face 4 set of texture + F/L/B/R has 3 set of texture * Each set is vec2
    getTextureCoordinates(texture);

    unsigned int indices[18]; // 6 triangles * 3 vertex each
    getIndices(indices);

    // Set up vertex data and buffers
    unsigned int VBO, VAO, EBO, TBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &TBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture), texture, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Projection matrix
    float projection[16];
    perspective(projection, 45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    Material* materials[5];
    materials[0] = new Material("../src/wall.jpg"); // bottom
    materials[1] = new Material("../src/cat.jpg"); // front
    materials[2] = new Material("../src/orange.jpg"); // back
    materials[3] = new Material("../src/cocktail.jpg"); // left
    materials[4] = new Material("../src/inn.jpg"); // right

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // update camera position
        updateCameraPosition();  // Now, this function does something!

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Camera matrix (view matrix)
        float view[16];
        float cameraX = sin(cameraAngle) * radius;
        float cameraZ = cos(cameraAngle) * radius;
        lookAt(view, cameraX, cameraHeight, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Model matrix (identity, no transformations)
        float model[16];
        loadIdentity(model);

        // Set matrices in the shaders
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

        // Draw the box
        glBindVertexArray(VAO);
        // Draw each face with its corresponding texture
        for (int i = 0; i < 1; ++i) {
            materials[i]->use();
            glUniform1i(glGetUniformLocation(shaderProgram, "faceIndex"), i);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
        }
        // only bottom is 6 vertex, the rest is 3
        for (int i = 1; i < 5; ++i) {
            materials[i]->use();
            glUniform1i(glGetUniformLocation(shaderProgram, "faceIndex"), i);
            int startOfIndices = ((i-1) * 3) + 6;
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(startOfIndices * sizeof(unsigned int)));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &TBO);
    glDeleteProgram(shaderProgram);

    for (int i = 0; i < 5; i++) {
        delete materials[i];
    }

    glfwTerminate();
    return 0;
}

// Function to process input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraAngle -= cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraAngle += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraHeight += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraHeight -= cameraSpeed;
}

// Function to update camera position (now with logic)
void updateCameraPosition() {
    // Adjust the camera's position based on user input
    if (cameraAngle > 2 * M_PI) {
        cameraAngle -= 2 * M_PI;  // Keep angle within 0 to 2π range
    }
    if (cameraAngle < 0) {
        cameraAngle += 2 * M_PI;  // Keep angle within 0 to 2π range
    }

    // Constrain camera height
    if (cameraHeight > 5.0f) {
        cameraHeight = 5.0f;  // Max height
    }
    if (cameraHeight < -5.0f) {
        cameraHeight = -5.0f;  // Min height
    }
}

// Helper function to multiply two 4x4 matrices
void matrixMultiply(float* result, const float* mat1, const float* mat2) {
    for (int i = 0; i < 16; ++i)
        result[i] = 0.0f;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result[row * 4 + col] += mat1[row * 4 + i] * mat2[i * 4 + col];
            }
        }
    }
}

// Helper function to load an identity matrix
void loadIdentity(float* matrix) {
    for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

// Function to create a perspective projection matrix
void perspective(float* matrix, float fov, float aspect, float near, float far) {
    float tanHalfFov = tan(fov / 2.0f);

    for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;

    matrix[0] = 1.0f / (aspect * tanHalfFov);
    matrix[5] = 1.0f / tanHalfFov;
    matrix[10] = -(far + near) / (far - near);
    matrix[11] = -1.0f;
    matrix[14] = -(2.0f * far * near) / (far - near);
}

// Function to create a look-at matrix
void lookAt(float* matrix, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
    float forward[3], right[3], up[3];

    forward[0] = centerX - eyeX;
    forward[1] = centerY - eyeY;
    forward[2] = centerZ - eyeZ;

    float forwardLength = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    forward[0] /= forwardLength;
    forward[1] /= forwardLength;
    forward[2] /= forwardLength;

    up[0] = upX;
    up[1] = upY;
    up[2] = upZ;

    right[0] = forward[1] * up[2] - forward[2] * up[1];
    right[1] = forward[2] * up[0] - forward[0] * up[2];
    right[2] = forward[0] * up[1] - forward[1] * up[0];

    float rightLength = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
    right[0] /= rightLength;
    right[1] /= rightLength;
    right[2] /= rightLength;

    up[0] = right[1] * forward[2] - right[2] * forward[1];
    up[1] = right[2] * forward[0] - right[0] * forward[2];
    up[2] = right[0] * forward[1] - right[1] * forward[0];

    loadIdentity(matrix);

    matrix[0] = right[0];
    matrix[1] = up[0];
    matrix[2] = -forward[0];

    matrix[4] = right[1];
    matrix[5] = up[1];
    matrix[6] = -forward[1];

    matrix[8] = right[2];
    matrix[9] = up[2];
    matrix[10] = -forward[2];

    matrix[12] = -(right[0] * eyeX + right[1] * eyeY + right[2] * eyeZ);
    matrix[13] = -(up[0] * eyeX + up[1] * eyeY + up[2] * eyeZ);
    matrix[14] = forward[0] * eyeX + forward[1] * eyeY + forward[2] * eyeZ;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
