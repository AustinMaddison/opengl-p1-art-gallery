#pragma once
#include <vector>
#include "model.hpp"

static void createPlane(float* center, float sizeX, float sizeY, Model* mesh) {
    float hSizeX = sizeX / 2.0f;
    float hSizeY = sizeY / 2.0f;
    float centerX = center[0];
    float centerY = center[1];
    float centerZ = center[2];

    std::vector<float> vertices = {
        centerX - hSizeX, centerY - hSizeY, 0.0f, 
        centerX + hSizeX, centerY - hSizeY, 0.0f, 
        centerX + hSizeX, centerY + hSizeY, 0.0f, 
        centerX - hSizeX, centerY + hSizeY, 0.0f, 
     
    };

    for (auto v : vertices)
    {
        mesh->addVertex(v);
    }

    std::vector<float> texture_coords = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    for (auto v : texture_coords) {
        mesh->addTextureCoord(v);
    }

    const std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,   // Back face
    };

    for (auto v : indices) {
        mesh->addIndices(v);
    }

}