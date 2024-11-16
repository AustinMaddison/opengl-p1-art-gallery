#pragma once
#include <vector>
#include "model.hpp"

static void createBox(float* center, float sizeX, float sizeY, float sizeZ, Model* mesh) {
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