//
// Created by aashi on 10/9/2023.
//

#ifndef KENOMAENGINE_MATERIAL_HPP
#define KENOMAENGINE_MATERIAL_HPP


#include "Shader.hpp"

class Material {
public:
    Shader* _shader;
    Shader _tempShader;
    void SetShader(Shader &s);



};


#endif //KENOMAENGINE_MATERIAL_HPP
