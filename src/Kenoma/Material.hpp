#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.hpp"
#include "Model.hpp"

class Material
{
public:
    Shader* _shader;

    Material();
    Material(Shader s);

    void Draw(Model* _model);
};

#endif