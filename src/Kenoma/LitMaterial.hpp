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
//    void LoadShader();
    void Draw(Model& model) const;
    void SetShader();
    void SetShader(Shader &s);

private:
    Shader litShader;
//    char* vertexShaderPath = "./data/shaders/lit.vs.glsl";
//    char* fragmentShaderPath = "./data/shaders/lit.fs.glsl";

};

#endif