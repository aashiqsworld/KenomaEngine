//
// Created by aashi on 9/12/2023.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "GameObject.hpp"

class Model : public GameObject
{
public:
    std::string name;
    // holds all the meshes that compose the model
    std::vector<Mesh> _meshes;
    // holds opengl texture handles
    std::vector<uint32_t> _textures;
    // holds all the local transforms for each mesh
    std::vector<glm::mat4> _transforms;
    std::vector<glm::mat4> _worldSpaceTransforms;
    // OpenGL buffers
    uint32_t _vao;
    uint32_t _vbo;
    uint32_t _ebo;
    // these are vectors because we'll be batching our draws
    std::vector<uint32_t> _cmds;
    std::vector<uint32_t> _objectData;
    uint32_t _transformData;
    Model();
    Model(std::string_view file);
    Model(std::string_view file, std::string name);
    ~Model();

    void LoadModel(std::string_view file);
    void Draw(const Shader& shader);
    uint32_t GetNumMeshes();
    uint32_t GetNumVertices();
    uint32_t GetNumMaterials();

private:

};


#endif
