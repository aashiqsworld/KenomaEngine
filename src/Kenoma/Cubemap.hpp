//
// Created by aashi on 3/20/2024.
//

#ifndef KENOMAENGINE_CUBEMAP_HPP
#define KENOMAENGINE_CUBEMAP_HPP

#include <vector>
#include <string>

class Cubemap
{
public:
    std::string texturePaths[6]; // 6 strings that point to the 6 sides of the cubemap

    Cubemap();
    Cubemap(std::string filePaths[6]);
    void LoadCubemap();

protected:
    unsigned int cubemapID;

};

#endif //KENOMAENGINE_CUBEMAP_HPP
