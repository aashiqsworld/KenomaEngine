#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.hpp"
#include "Model.hpp"
#include "Material.hpp"

class LitMaterial : public Material
{
public:
    LitMaterial();
    explicit LitMaterial(Shader s);
    LitMaterial(const char* vertexPath, const char* fragmentPath);

    void SetShader(Shader &s);
    void Draw(Model& model) const;

private:
    Shader litShader;
};

#endif