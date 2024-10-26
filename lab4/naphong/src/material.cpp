#include "material.h"
#include "stb_image.h"

Material::Material(const char* filename) {

    int width, height, channels;
    // standardize to always be 4 rgba
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    // create texture after load image
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // free data
    stbi_image_free(data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Material::~Material() {
    // no of textures, pointer to beginning
    glDeleteTextures(1, &texture);
}

void Material::use() {
    glBindTexture(GL_TEXTURE_2D, texture);
}