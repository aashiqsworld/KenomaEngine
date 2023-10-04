#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.hpp"
#include "Model.hpp"

class Material
{
public:
    Shader* _shader;

    Material();
    explicit Material(Shader s);
    Material(const char* vertexPath, const char* fragmentPath);

    void SetShader(Shader &s);
    void Draw(Model& model) const;
};

#endif