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

    void Draw(Model& model) const;
    void SetShader();
    glm::vec3 color{};
private:
    Shader unlitShader;

};


#endif //KENOMAENGINE_UNLITMATERIAL_HPP
