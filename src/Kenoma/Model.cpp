//
// Created by aashi on 9/12/2023.
//

#include <filesystem>
#include <unordered_map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include "cgltf.h"
#include "Model.hpp"
#include "stb_image.h"

namespace fs = std::filesystem;

// helper function to find the actual texture path given a CGLTF image
static std::string FindTexturePath(const fs::path& basePath, const cgltf_image* image)
{
    std::string texturePath;

    if(!image->uri)
    {
        auto newPath = basePath / image->name;
        if(!newPath.has_extension())
        {
            if(std::strcmp(image->mime_type, "image/png") == 0)
            {
                newPath.replace_extension("png");
            }
            else if (std::strcmp(image->mime_type, "image/jpg") == 0)
            {
                newPath.replace_extension("jpg");
            }
        }
        texturePath = newPath.generic_string();
    }
    else
    {
        texturePath = (basePath / image->uri).generic_string();
    }
    return texturePath;
}

Model::Model(std::string_view file)
{
    LoadModel(file);
}

Model::Model(std::string_view file, std::string _name) {
    LoadModel(file);
    name = std::move(_name);
}


Model::Model() {

}

Model::~Model() {

}

uint32_t Model::GetNumMeshes() {
    return _meshes.size();
}

uint32_t Model::GetNumVertices() {
    int i = 0;
    for(const auto mesh : _meshes)
    {
//        i += mesh.
    }
    return 0;
}

void Model::LoadModel(std::string_view file) {
    cgltf_options options = {};
    cgltf_data* model = nullptr;
    // Read GLTF, no additional options are required
    cgltf_parse_file(&options, file.data(), &model);
    // Load all GLTF buffers
    cgltf_load_buffers(&options, model, file.data());

    // Get the base path (useful when loading textures)
    fs::path path(file.data());
    const auto basePath = path.parent_path();
    // This is our texture cache, to make sure we don't load the same texture twice
    std::unordered_map<std::string, size_t> textureIds;
    // Reserves space for our texture vector
    _textures.reserve(model->materials_count == 0 ? 0 : model->materials_count);
    // Since we'll be batching our draws based on the textures it has, we need to calculate
    // how many batches this model needs, this is done by dividing by 16, which is the "batch size"
    // and rounding up, because we always need at least one batch.
    const uint32_t maxBatches = model->materials_count / 16 + 1;
    for (uint32_t i = 0; i < model->materials_count; ++i) // For each material
    {
        const auto& material = model->materials[i];

        cgltf_image* image;
        std::string texturePath;
        // Get the material's base color texture
        if(material.pbr_metallic_roughness.base_color_texture.texture != nullptr) {
            image = material.pbr_metallic_roughness.base_color_texture.texture->image;
            // Find its texture path
            texturePath =  FindTexturePath(basePath, image);
        }
        else {
            // Find its texture path
            texturePath = "./data/textures/DefaultWhite.png";
        }

        if (!textureIds.contains(texturePath)) // check if the texture is already loaded
        {
            LoadTexture(texturePath);
            // Register this texture index in our cache
            textureIds[texturePath] = _textures.size() - 1;
        }

        // --- load normal texture ---
        if(material.normal_texture.texture != NULL)
        {
            image = material.normal_texture.texture->image;
            // Find its texture path
            texturePath = FindTexturePath(basePath, image);
        }
        else
        {
            texturePath = "./data/textures/DefaultNormal.png";
        }

        if (!textureIds.contains(texturePath)) // check if the texture is already loaded
        {
            LoadTexture(texturePath);
            // Register this texture index in our cache
            textureIds[texturePath] = _textures.size() - 1;
        }
    }

    // if necessary, load default material
    if(model->materials_count == 0) {
        std::string defaultTexturePath = "./data/textures/DefaultWhite.png";


        defaultTexturePath = "./data/textures/DefaultNormal.png";


    }

    uint32_t transformIndex = 0;
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    std::vector<MeshCreateInfo> meshCreateInfos;
    meshCreateInfos.reserve(1024);

    // For each node in the scene
    for (uint32_t i = 0; i < model->scene->nodes_count; ++i)
    {
        std::queue<cgltf_node*> nodes;
        // Add the node to the queue TODO: use recursion instead of a queue
        nodes.push(model->scene->nodes[i]);
        while (!nodes.empty())
        {
            // Get a node from the queue
            const auto* node = nodes.front();
            nodes.pop();
            // If there are no meshes
            if (!node->mesh)
            {
                // Then for each node (if any)
                for (uint32_t j = 0; j < node->children_count; ++j)
                {
                    // Add it to the queue again
                    nodes.push(node->children[j]);
                }
                continue;
            }


            // For each primitive in the node
            for (uint32_t j = 0; j < node->mesh->primitives_count; ++j)
            {
                const auto& primitive = node->mesh->primitives[j];
                const glm::vec3* positionPtr = nullptr;
                const glm::vec3* normalPtr = nullptr;
                const glm::vec2* uvPtr = nullptr;
                const glm::vec3* tangentPtr = nullptr;
                uint64_t vertexCount = 0;
                // Get its vertex data
                for (uint32_t k = 0; k < primitive.attributes_count; ++k)
                {
                    // Get the attribute information (position, normal, ...)
                    const auto& attribute = primitive.attributes[k];
                    const auto* accessor = attribute.data;
                    // Get the buffer view associated with this attribute
                    const auto* view = accessor->buffer_view;
                    const auto* dataPtr = (const char*)view->buffer->data;
                    // If this is confusing you can refer to the glTF main scheme by Khronos, it should clear up some things
                    switch (attribute.type)
                    {
                        case cgltf_attribute_type_position:
                            vertexCount = accessor->count;
                            // Set the `positionPtr`
                            positionPtr = (const glm::vec3*)(dataPtr + view->offset + accessor->offset);
                            break;

                        case cgltf_attribute_type_normal:
                            // Set the `normalPtr`
                            normalPtr = (const glm::vec3*)(dataPtr + view->offset + accessor->offset);
                            break;

                        case cgltf_attribute_type_texcoord:
                            // Set the `uvPtr`
                            uvPtr = (const glm::vec2*)(dataPtr + view->offset + accessor->offset);
                            break;

                        case cgltf_attribute_type_tangent:
                            // Set the `tangentPtr`
                            tangentPtr = (const glm::vec3*)(dataPtr + view->offset +
                                    accessor->offset);
                            break;

                        default: break;
                    }
                }

                // Reserve space for the vertices in our own vertex format
                std::vector<Vertex> vertices;
                vertices.resize(vertexCount);
                {
                    // Get the pointer to the base of the vector
                    auto* ptr = vertices.data();
                    // For each vertex


                    for (uint32_t v = 0; v < vertexCount; ++v, ++ptr)
                    {
                        // Copy the attribute (if available) to the current pointer (will increment every iteration)
                        if (positionPtr)
                        {
                            std::memcpy(&ptr->position, positionPtr + v, sizeof(glm::vec3));
                        }
                        if (normalPtr)
                        {
                            std::memcpy(&ptr->normal, normalPtr + v, sizeof(glm::vec3));
                        }
                        if (uvPtr)
                        {
                            std::memcpy(&ptr->uv, uvPtr + v, sizeof(glm::vec2));
                        }
                        if (tangentPtr)
                        {
                            std::memcpy(&ptr->tangent, tangentPtr + v, sizeof(glm::vec3));
                        }
                    }
                }


                std::vector<uint32_t> indices;
                {
                    // Get the indices information for the primitive
                    const auto* accessor = primitive.indices;
                    const auto* view = accessor->buffer_view;
                    const char* dataPtr = (const char*)view->buffer->data;
                    // Reserve space for our indices buffer
                    indices.reserve(accessor->count);
                    // Check the index type (uint8, uint16 or uin32)
                    switch (accessor->component_type)
                    {
                        // Copy the whole index buffer to our vector
                        case cgltf_component_type_r_8:
                        case cgltf_component_type_r_8u:
                        {
                            const auto* ptr = (const uint8_t*)(dataPtr + view->offset + accessor->offset);
                            std::copy(ptr, ptr + accessor->count, std::back_inserter(indices));
                        }
                            break;

                        case cgltf_component_type_r_16:
                        case cgltf_component_type_r_16u:
                        {
                            const auto* ptr = (const uint16_t*)(dataPtr + view->offset + accessor->offset);
                            std::copy(ptr, ptr + accessor->count, std::back_inserter(indices));
                        }
                            break;

                        case cgltf_component_type_r_32f:
                        case cgltf_component_type_r_32u:
                        {
                            const auto* ptr = (const uint32_t*)(dataPtr + view->offset + accessor->offset);
                            std::copy(ptr, ptr + accessor->count, std::back_inserter(indices));
                        }
                            break;

                        default: break;
                    }
                }


                // Get the primitive's material base color texture path
                std::string baseColorURI;
                if(primitive.material != nullptr)
                {
                    baseColorURI= FindTexturePath(basePath, primitive
                    .material->pbr_metallic_roughness.base_color_texture.texture->image);
                }
                else
                {
                    // load default white texture
                    baseColorURI = "./data/textures/DefaultWhite.png";
                }

                std::string normalMapURI;
                if(primitive.material != nullptr)
                {
                     normalMapURI = FindTexturePath(basePath, primitive.material->normal_texture
                     .texture->image);
                }
                else
                {
                    // load default normal map texture
                    normalMapURI = "./data/textures/DefaultNormal.png";
                }


                const auto indexCount = indices.size();

                // Emplace a `MeshCreateInfo` (we will use this later)
                meshCreateInfos.emplace_back(MeshCreateInfo
                                                     {
                                                             vertices,
                                                             indices,
                                                             transformIndex++,
                                                             // Exercise: this doesn't handle missing textures, it's possible that a mesh may not have any color
                                                             // texture, can you change this behavior and display a default texture of your choice when this happens?
                                                             (uint32_t)textureIds[baseColorURI],
                                                             // Exercise: We don't load normal textures, can you load the normal textures (when available)
                                                             // and apply some basic normal mapping?
                                                             (uint32_t)textureIds[normalMapURI],
                                                             vertexOffset,
                                                             indexOffset,
                                                     });

                // Apply the node transformation and emplace it to the vector
                cgltf_node_transform_world(node, glm::value_ptr(_transforms.emplace_back()));
                // Increment the vertex and index byte offset
                vertexOffset += vertexCount * sizeof(Vertex);
                indexOffset += indexCount * sizeof(uint32_t);
            }
            // Push children nodes
            for (uint32_t j = 0; j < node->children_count; ++j)
            {
                nodes.push(node->children[j]);
            }



        }
    }

    // Resize the indirect commands vector and the object data vector.
    _cmds.resize(maxBatches);
    _objectData.resize(maxBatches);
    // Allocate GL buffers
    // This is the Vertex Array Object, it specifies how the vertex data should be read by the GPU
    glCreateVertexArrays(1, &_vao);
    // This is the Vertex Buffer Object, it holds on all the mesh vertex data in this model
    glCreateBuffers(1, &_vbo);
    // This is the Element Buffer Object
    // it holds all the indices for all the meshes
    glCreateBuffers(1, &_ebo);
    // This is the transform data buffer, it holds the local transform for each mesh
    glCreateBuffers(1, &_transformData);
    // Create the object data buffers, they are useful when drawing because they associate all the
    // necessary indices (transform index, base color index, ...) to a mesh, allowing the shader
    // to fetch the information from the other buffers, keep in mind this is per batch.
    glCreateBuffers(_objectData.size(), _objectData.data());
    // Finally this is the indirect data buffer, it holds all the information required for OpenGL to draw the mesh,
    // this is also per batch.
    glGenBuffers(_cmds.size(), _cmds.data());// First, we need to figure out how big our vertex and index buffer should be
    size_t vertexSize = 0;
    size_t indexSize = 0;
    for (const auto& info : meshCreateInfos)
    {
        vertexSize += info.vertices.size() * sizeof(Vertex);
        indexSize += info.indices.size() * sizeof(uint32_t);
    }

    // Allocate the storage
    glNamedBufferStorage(_vbo, vertexSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(_ebo, indexSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

    // Associate the vertex array object, with our vertex and index buffer
    glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(_vao, _ebo);

    // Tell OpenGL which vertex location index we want to use
    // it maps to the "layout (location = N) in vecN position/normal/..." in our vertex shader
    glEnableVertexArrayAttrib(_vao, 0);
    glEnableVertexArrayAttrib(_vao, 1);
    glEnableVertexArrayAttrib(_vao, 2);
    glEnableVertexArrayAttrib(_vao, 3);


    // Tell OpenGL how to interpret each vertex attribute
    //                              location          components  type      transpose  offset
    glVertexArrayAttribFormat(_vao, /*location = */0, /*vec*/3,   GL_FLOAT, GL_FALSE,  offsetof(Vertex, position));
    glVertexArrayAttribFormat(_vao, /*location = */1, /*vec*/3,   GL_FLOAT, GL_FALSE,  offsetof(Vertex, normal));
    glVertexArrayAttribFormat(_vao, /*location = */2, /*vec*/2,   GL_FLOAT, GL_FALSE,  offsetof(Vertex, uv));
    glVertexArrayAttribFormat(_vao, /*location = */3, /*vec*/3,   GL_FLOAT, GL_FALSE,  offsetof
    (Vertex, tangent));

    // Finally, bind each vertex attribute to a vertex buffer,
    // the 0th buffer, which is the only one we bound
    glVertexArrayAttribBinding(_vao, 0, 0);
    glVertexArrayAttribBinding(_vao, 1, 0);
    glVertexArrayAttribBinding(_vao, 2, 0);
    glVertexArrayAttribBinding(_vao, 3, 0);

    // For each mesh
    for (auto& info : meshCreateInfos)
    {
        // Upload at a given vertex and index offset the data
        glNamedBufferSubData(
                _vbo,
                info.vertexOffset,
                info.vertices.size() * sizeof(Vertex),
                info.vertices.data());
        glNamedBufferSubData(
                _ebo,
                info.indexOffset,
                info.indices.size() * sizeof(uint32_t),
                info.indices.data());
        _meshes.emplace_back(info);


    }
}

void Model::UpdateTransforms()
{
    // multiply the mesh transforms with the base GameObject transform
    _worldSpaceTransforms.clear();
    for(auto& trans : _transforms)
    {
        _worldSpaceTransforms.push_back(GetTransformationMatrix() * trans);
    }
}

bool Model::LoadTexture(const std::string& texturePath) {

// If we already loaded the texture, go onto the next material
    uint32_t texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    // Sets the texture's sampler's parameters
    // if you are not familiar with these, LearnOpenGL.com has a great tutorial
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loads the texture data with STB_Image
    int32_t width = 0;
    int32_t height = 0;
    int32_t channels = STBI_rgb_alpha;
    const auto* textureData = stbi_load(texturePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    // Calculate how many mip levels we need to generate for the texture.
    const auto levels = (uint32_t)std::floor(std::log2(std::max(width, height)));
    // Actually allocate the texture
    glTextureStorage2D(texture, levels, GL_RGBA8, width, height);
    // Copy our texture data to the GPU
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    // Generate mipmaps
    glGenerateTextureMipmap(texture);
    // Free texture memory on our end
    stbi_image_free((void*)textureData);
    // Add the new texture handle to the texture vector
    _textures.emplace_back(texture);
    return true;
}

