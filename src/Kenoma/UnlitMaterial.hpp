//
// Created by aashi on 10/11/2023.
//

#ifndef KENOMAENGINE_UNLITMATERIAL_HPP
#define KENOMAENGINE_UNLITMATERIAL_HPP


#include "Material.hpp"
#include "Model.hpp"


class UnlitMaterial : public Material{
public:
    UnlitMaterial();

    explicit UnlitMaterial(const vec3 &color);

    void Draw(Model& model);
    void SetShader();
    glm::vec3 color{};

    bool outline;
    float outlineScale;


private:
    Shader unlitShader;
    std::vector<glm::mat4> outlineTransforms;

};


#endif //KENOMAENGINE_UNLITMATERIAL_HPP
