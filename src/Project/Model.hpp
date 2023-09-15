//
// Created by aashi on 9/12/2023.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include "Shader.hpp"
#include "Mesh.hpp"

class Model
{
public:
    Model();
    Model(std::string_view path);
    ~Model();

    void Draw(const Shader& shader) const;

private:
    // holds all the meshes that compose the model
    std::vector<Mesh> _meshes;
    // holds opengl texture handles
    std::vector<uint32_t> _textures;
    // holds all the local transforms for each mesh
    std::vector<glm::mat4> _transforms;
    // OpenGL buffers
    uint32_t _vao;
    uint32_t _vbo;
    uint32_t _ebo;
    // these are vectors because we'll be batching our draws
    std::vector<uint32_t> _cmds;
    std::vector<uint32_t> _objectData;
    uint32_t _transformData;
};


#endif
