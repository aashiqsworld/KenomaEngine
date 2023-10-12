#include <set>
#include "LitMaterial.hpp"



LitMaterial::LitMaterial(Shader s) {
    _shader = &s;
}

void LitMaterial::Draw(Model &model) const {
    _shader->Bind();
    struct ObjectData
    {
        uint32_t transformIndex;
        uint32_t baseColorIndex;
        uint32_t normalIndex;
    };
    struct BatchData {
        std::vector<ObjectData> objects;
        std::vector<MeshIndirectInfo> commands;
    };
    std::vector<BatchData> objectBatches(model._cmds.size());
    std::vector<std::set<uint32_t>> textureHandles(model._cmds.size());
    // For each mesh
    for (const auto& mesh : model._meshes)
    {
        // Calculate the batch index this mesh belongs to (just divide by the "batch size")
        const auto index = mesh.BaseColorTexture() / 16;
        // Get the mesh indirect info structure
        objectBatches[index].commands.emplace_back(mesh.Info());
        // Get the mesh general information
        objectBatches[index].objects.emplace_back(ObjectData
                                                          {
                                                                  // Restrict the texture range to [0, 15], because by batching texture
                                                                  // indices must not be "global", but local to the batch group
                                                                  mesh.TransformIndex(),
                                                                  mesh.BaseColorTexture() % 16,
                                                                  // Exercise: Can you do the same for normal textures?
                                                                  mesh.NormalTexture()
                                                          });
        // Insert the texture index for this batch in a set, this is useful
        // when binding the textures because we will need unique handles
        textureHandles[index].insert(model._textures[mesh.BaseColorTexture()]);
        textureHandles[index].insert(model._textures[mesh.NormalTexture()]);
    }

    // Copy the transform data we just created to the GPU
    glNamedBufferData(
            model._transformData,
            model._worldSpaceTransforms.size() * sizeof(glm::mat4),
            model._worldSpaceTransforms.data(),
            GL_DYNAMIC_DRAW);
    // Bind the buffer to the storage buffer, location = 1
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, model._transformData);

    // Bind the shader because we will be setting uniforms now
    _shader->Bind();
    // For each batch
    for (uint32_t index = 0; const auto& batch : objectBatches)
    {
        // Write the object data to the current object uniform buffer
        glNamedBufferData(
                model._objectData[index],
                batch.objects.size() * sizeof(ObjectData),
                batch.objects.data(),
                GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, model._objectData[index]);

        // Write the indirect commands to the current indirect buffer
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, model._cmds[index]);
        glNamedBufferData(
                model._cmds[index],
                batch.commands.size() * sizeof(MeshIndirectInfo),
                batch.commands.data(),
                GL_DYNAMIC_DRAW);

        // Set all the active textures for this batch if the shader uses textures
        if(_shader->usesTextures)
        {
            for (uint32_t offset = 0; const auto texture : textureHandles[index])
            {
                _shader->setInt(2 + offset, offset);
                glActiveTexture(GL_TEXTURE0 + offset);
                glBindTexture(GL_TEXTURE_2D, texture);
                offset++;
            }
        }


        // Finally, bind the VAO and issue the draw call
        glBindVertexArray(model._vao);
        glMultiDrawElementsIndirect(
                GL_TRIANGLES,
                GL_UNSIGNED_INT,
                nullptr,
                batch.commands.size(),
                sizeof(MeshIndirectInfo));
        // Increment the index to go to the next batch
        index++;
    }
}

void LitMaterial::SetShader(Shader &s) {
    _shader = &s;
}

LitMaterial::LitMaterial() {
//    litShader.LoadShader("./data/shaders/lit.vs.glsl", "./data/shaders/lit.fs.glsl", true);
    _shader = &litShader;

}

LitMaterial::LitMaterial(const char *vertexPath, const char *fragmentPath) {

}
