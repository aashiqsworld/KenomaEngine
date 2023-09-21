//
// Created by aashi on 9/11/2023.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <string>
#include "glm/glm.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

struct MeshCreateInfo
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t transformIndex;
    uint32_t baseColorTexture;
    uint32_t normalTexture;
    size_t vertexOffset;
    size_t indexOffset;
};

struct MeshCreateInfoAlt
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;
    uint32_t transformIndex;
    size_t vertexOffset;
    size_t indexOffset;
};

struct MeshIndirectInfo
{
    uint32_t count;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t baseVertex;
    uint32_t baseInstance;
};



class Mesh
{
public:
    Mesh(const MeshCreateInfo& info);
    Mesh(const MeshCreateInfoAlt& info);
    ~Mesh();

    MeshIndirectInfo Info() const;
    uint32_t TransformIndex() const;
    uint32_t BaseColorTexture() const;
    uint32_t NormalTexture() const;

    uint32_t NumVertices() const;


private:
    uint32_t _indexCount = 0;
    int32_t _vertexOffset = 0;
    uint32_t _indexOffset = 0;
    // not OpenGL handles, just indices
    uint32_t _transformIndex = NAN;
    uint32_t _baseColorTexture = NAN;
    uint32_t _normalTexture = NAN;
};

#endif
