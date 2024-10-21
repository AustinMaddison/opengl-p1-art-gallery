#pragma once

#ifndef MYLIB
#define MYLIB



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

enum RenderMode {
    FILL,
    WIREFRAME,
    BOTH
};

// Shader class heavily inspired from learnopengl.com
class Shader
{
public:
    unsigned int ID;
    Shader(std::string vertexPath, std::string fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }

        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

  


private:

    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR:: " << type << ": " << infoLog;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR:: " << type << ": " << infoLog; 
            }
        }
    }
};

class Mesh
{
public:
    Mesh()
    {
        _VAO = -1;
        _VBO = -1;
        _EBO = -1;
        RenderMode renderMode = FILL;
        Shader* _fillShader = nullptr;
        Shader* _lineShader = nullptr;
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteBuffers(1, &_EBO);
    }

    void addVertex(float v)
    {
        if (_vertices.size() > 0 && _vertices.empty())
            _vertices = std::vector<float>();

        _vertices.push_back(v);
        isDirty = true;
    }

    void addIndices(unsigned int v)
    {
        if (_indices.size() > 0 && _indices.empty())
            _indices = std::vector<int>();

       _indices.push_back(v); 

       isDirty = true;
    }

    void addShaderFill(Shader *s)
    {
        _shaderFill = s;
    }

    void addTexture()
    {
        glGenTextures(1, &_TEXTURE_ID);
        glBindTexture(GL_TEXTURE_2D, _TEXTURE_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load("container.JPG", &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR::ADDING_TEXTURE: Failed to load texture.\n" << std::endl;
        }

        stbi_image_free(data);
    }

    void addShaderLine(Shader *s)
    {
        _shaderLine = s;
    }

    void addShaderPoint(Shader *s)
    {
        _shaderPoint = s;
    }

    void initialize()
    {
        try 
        {
            if (_vertices.size() > 0 && _vertices.empty()) throw std::runtime_error("VERTICES_NOT_ADDED");
            if (_indices.size() > 0 && _indices.empty()) throw std::runtime_error("INDICES_NOT_ADDED");
        }
        catch (std::exception e)
        {
            std::cout << "ERROR::MESH_INTITIALISATION_FAILED: " << e.what() << std::endl;
            return;
        }

        if (!isDirty) return;


        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);

        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(_indices[0]), &_indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(_vertices[0]), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(_vertices[0]), (void*)(sizeof(_vertices[0]) * 3));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        isMeshIsInitialized = true;
        isDirty = false;
    }

    void draw()
    {
        try 
        {
            if (_shaderFill == nullptr) throw std::runtime_error("FILL_SHADER_NOT_ADDED");
            if (_shaderLine == nullptr) throw std::runtime_error("LINE_SHADER_NOT_ADDED");
            if (!isMeshIsInitialized) throw std::runtime_error("MESH_NOT_INITIALIZED");
        }
        catch (std::exception e)
        {
            std::cout << "ERROR::MESH_DRAW: " << e.what() << std::endl;
            return;
        }

        glBindTexture(GL_TEXTURE_2D, _TEXTURE_ID);
        glBindVertexArray(_VAO);

        
        switch (_renderMode)
        {
        case FILL:
            _shaderFill->use();
            _shaderFill->setFloat("uTime", glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
            break;

        case WIREFRAME:
            
            glDisable(GL_DEPTH_TEST); // disable depth test so that the points are rendered on top of everything

            _shaderLine->use();
            _shaderLine->setFloat("uTime", glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);

            _shaderPoint->use();
            _shaderPoint->setFloat("uTime", glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
 
            glEnable(GL_DEPTH_TEST); // enable depth test again
            
            break;
        
        case BOTH:

            _shaderFill->use();
            _shaderFill->setFloat("uTime", glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);

            glDisable(GL_DEPTH_TEST); // disable depth test so that the points are rendered on top of everything

            _shaderLine->use();
            _shaderLine->setFloat("uTime", glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);

            _shaderPoint->use();
            _shaderPoint->setFloat("uTime", glfwGetTime());
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
 
            glEnable(GL_DEPTH_TEST); // enable depth test again
            break;

        default:
            break;
        }
    }

    void setRenderMode(RenderMode renderMode)
    {
        _renderMode = renderMode;
    }

private:
    unsigned int _VBO;
    unsigned int _VAO;
    unsigned int _EBO;
    unsigned int _TEXTURE_ID;

    bool isMeshIsInitialized = false;
    bool isDirty = true;

    std::vector<float> _vertices;
    std::vector<int> _indices;

    RenderMode _renderMode = BOTH;

    Shader* _shaderFill = nullptr;
    Shader* _shaderLine = nullptr;
    Shader* _shaderPoint = nullptr;
};

#endif 
