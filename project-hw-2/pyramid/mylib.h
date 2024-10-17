#pragma once

#ifndef MYLIB
#define MYLIB

#include <glad/glad.h>

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
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

class Mesh
{
public:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    RenderMode renderMode = FILL;

    Mesh()
    {
        RenderMode renderMode = FILL;
        Shader* _fillShader = nullptr;
        Shader* _lineShader = nullptr;
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
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

    void addShaderLine(Shader *s)
    {
        _shaderLine = s;
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

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(_indices[0]), &_indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(_vertices[0]), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(NULL);

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

        glBindVertexArray(VAO);

        switch (renderMode)
        {
        case FILL:
            _shaderFill->setFloat("uTime", glfwGetTime());
            _shaderFill->use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
            break;

        case WIREFRAME:
            _shaderLine->setFloat("uTime", glfwGetTime());
            _shaderLine->use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
            break;
        
        case BOTH:
            _shaderFill->setFloat("uTime", glfwGetTime());
            _shaderFill->use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);

            _shaderLine->setFloat("uTime", glfwGetTime());
            _shaderLine->use();
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
            break;

        default:
            break;
        }
    }

private:
    bool isMeshIsInitialized = false;
    std::vector<float> _vertices;
    std::vector<int> _indices;

    Shader* _shaderFill = nullptr;
    Shader* _shaderLine = nullptr;

    bool isDirty = true;
};

#endif // MACRO
