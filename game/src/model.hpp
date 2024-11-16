#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.hpp"

class Model
{
public:
    Model()
    {
        _VAO = -1;
        _VBO = -1;
        _EBO = -1;
        _TBO = -1;
        Shader* _fillShader = nullptr;
        Shader* _lineShader = nullptr;
    }

    ~Model()
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

    void addTextureCoord(float v)
    {
        if (_texture_coords.size() > 0 && _texture_coords.empty())
            _texture_coords = std::vector<float>();

        _texture_coords.push_back(v);
        isDirty = true;
    }

    void addIndices(unsigned int v)
    {
        if (_indices.size() > 0 && _indices.empty())
            _indices = std::vector<int>();

        _indices.push_back(v);

        isDirty = true;
    }

    void addShaderFill(Shader* s)
    {
        _shaderFill = s;
    }

    void addTexture(const char* file_path)
    {
        unsigned int TEXTURED_ID;
        glGenTextures(1, &TEXTURED_ID);
        glBindTexture(GL_TEXTURE_2D, TEXTURED_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(file_path, &width, &height, &nrChannels, 0);

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

        _TEXTURE_IDS.push_back(TEXTURED_ID);
    }

    void addShaderLine(Shader* s)
    {
        _shaderLine = s;
    }

    void addShaderPoint(Shader* s)
    {
        _shaderPoint = s;
    }

    void initialize()
    {
        try
        {
            if (_vertices.size() > 0 && _vertices.empty()) throw std::runtime_error("VERTICES_NOT_ADDED");
            if (_indices.size() > 0 && _indices.empty()) throw std::runtime_error("INDICES_NOT_ADDED");
            if (_texture_coords.size() > 0 && _texture_coords.empty()) throw std::runtime_error("TEXCOORDS_NOT_ADDED");
        }
        catch (std::exception e)
        {
            std::cout << "ERROR::MESH_INTITIALISATION_FAILED: " << e.what() << std::endl;
            return;
        }

        if (!isDirty) return;

        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_TBO);
        glGenBuffers(1, &_EBO);

        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, _TBO);
        glBufferData(GL_ARRAY_BUFFER, _texture_coords.size() * sizeof(_texture_coords[0]), &_texture_coords[0], GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(_indices[0]), &_indices[0], GL_STATIC_DRAW);

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

        auto _texture_id_stack = _TEXTURE_IDS;

        glBindTexture(GL_TEXTURE_2D, _texture_id_stack[0]);

        glBindVertexArray(_VAO);
        _shaderFill->use();
        _shaderFill->setFloat("uTime", glfwGetTime());
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int _VBO;
    unsigned int _VAO;
    unsigned int _EBO;
    unsigned int _TBO;
    std::vector<unsigned int> _TEXTURE_IDS;

    bool isMeshIsInitialized = false;
    bool isDirty = true;

    std::vector<float> _vertices;
    std::vector<float> _texture_coords;
    std::vector<int> _indices;

    RenderMode _renderMode = BOTH;

    Shader* _shaderFill = nullptr;
    Shader* _shaderLine = nullptr;
    Shader* _shaderPoint = nullptr;
};

#endif 
