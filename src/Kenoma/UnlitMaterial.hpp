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
    void Draw(Model& model) const;

private:
    Shader unlitShader;

};


#endif //KENOMAENGINE_UNLITMATERIAL_HPP
