//
// Created by aashi on 3/20/2024.
//

#include "Cubemap.hpp"
#include "glad/glad.h"
#include "stb_image.h"

Cubemap::Cubemap() {
    texturePaths[0] = "./data/textures/cubemap/right.jpg";
    texturePaths[1] = "./data/textures/cubemap/left.jpg";
    texturePaths[2] = "./data/textures/cubemap/top.jpg";
    texturePaths[3] = "./data/textures/cubemap/bottom.jpg";
    texturePaths[4] = "./data/textures/cubemap/front.jpg";
    texturePaths[5] = "./data/textures/cubemap/back.jpg";
}

/***
 * Cubemap constructor
 * @param filePaths Array of file paths to the cubemap faces. Order: right, left, top, bottom,
 * font, back
 */
Cubemap::Cubemap(std::string filePaths[6])
{
    for(int i = 0; i < 6; i++)
        texturePaths[i] = filePaths[i];
}

void Cubemap::LoadCubemap()
{
    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    int width, height, nChannels;
    unsigned char *data;

    for(int i = 0; i < 6; i++)
    {
        data = stbi_load(texturePaths[i].c_str(), &width, &height, &nChannels, 0);
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


