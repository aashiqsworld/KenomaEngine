//
// Created by aashi on 9/12/2023.
//

#include "Mesh.hpp"
#include "glad/glad.h"

Mesh::Mesh(const MeshCreateInfo& info)
{
    _indexCount = info.indices.size();
    _vertexOffset = info.vertexOffset / sizeof(Vertex);
    _indexOffset = info.indexOffset / sizeof(uint32_t);
    _transformIndex = info.transformIndex;
    _baseColorTexture = info.baseColorTexture;
    _normalTexture = info.normalTexture;
}

MeshIndirectInfo Mesh::Info() const
{
    return {
        _indexCount,
        1,
        _indexOffset,
        _vertexOffset,
        1
    };
}

uint32_t Mesh::TransformIndex() const
{
    return _transformIndex;
}

uint32_t Mesh::BaseColorTexture() const
{
    return _baseColorTexture;
}

uint32_t Mesh::NormalTexture() const
{
    return _normalTexture;
}

Mesh::~Mesh() {

}

uint32_t Mesh::NumVertices() const {
    return _indexCount;
}

