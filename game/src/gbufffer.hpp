#pragma once
#include <glad/glad.h>
#include <iostream>
#include <array>

struct GBuffer
{
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_RAW_RENDER,
        GBUFFER_TEXTURE_POSTPROCESSING,
        GBUFFER_TEXTURE_UI,
        GBUFFER_NUM_TEXTURES,
    };

    GBuffer();

    ~GBuffer();

    bool Init(unsigned int windowWidth, unsigned int windowHeight)
    {
        glGenFramebuffers(0, &FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
 
        glGenTextures(GBUFFER_NUM_TEXTURES, &textures[0]);
        glGenTextures(1, &depthTexture);

        for(int i = 0; i < GBUFFER_NUM_TEXTURES; i++)
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], NULL);
        }

        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, NULL);
        std::array<GLenum, GBUFFER_NUM_TEXTURES> DrawBuffers = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(DrawBuffers.size(), &DrawBuffers[0]);

        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "FB error, status: 0x" << Status << '\n';
            return false;
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;

    }

    void BindForWriting()
    {
        glBindFramebuffer(0, FBO);
    }

    void BindForReading()
    {

    }

private:
    GLuint FBO;
    std::array<GLuint, GBUFFER_NUM_TEXTURES> textures;
    GLuint depthTexture;
};




