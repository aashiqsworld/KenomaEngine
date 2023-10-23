//
// Created by aashi on 10/11/2023.
//

#include <set>
#include "UnlitMaterial.hpp"

UnlitMaterial::UnlitMaterial() {
    color = glm::vec3(1.0, 1.0, 1.0);
    outline = false;
    outlineScale = 1.04;
}

UnlitMaterial::UnlitMaterial(const vec3 &color) : color(color) {}

void UnlitMaterial::Draw(Model &model) {

    std::vector<glm::mat4>* transforms;

    if(!outline)
    {
        transforms = &model._worldSpaceTransforms;
    }
    else
    {
        outlineTransforms.clear();
        for(auto& trans : model._worldSpaceTransforms)
        {

            outlineTransforms.push_back(glm::scale(trans, glm::vec3(outlineScale, outlineScale,
                                                                    outlineScale)));
        }
        transforms = &outlineTransforms;
    }

    _shader->Bind();
    struct ObjectData
    {
        uint32_t transformIndex;
    };
    struct BatchData {
        std::vector<ObjectData> objects;
        std::vector<MeshIndirectInfo> commands;
    };
    std::vector<BatchData> objectBatches(model._cmds.size());
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
                                                                mesh.TransformIndex(),
                                                          });
    }

    // Copy the transform data we just created to the GPU
    glNamedBufferData(
            model._transformData,
            transforms->size() * sizeof(glm::mat4),
            transforms->data(),
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

void UnlitMaterial::SetShader() {
    unlitShader.LoadShader("./data/shaders/unlit.vs.glsl", "./data/shaders/unlit.fs.glsl", false);
    _shader = &unlitShader;
}
