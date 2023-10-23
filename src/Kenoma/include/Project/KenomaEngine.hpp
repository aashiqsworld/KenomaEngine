#pragma once


#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <string_view>
#include <vector>
#include <memory>
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "GameObject.hpp"
#include "../../../Kenoma.Engine/include/Project.Library/Application.hpp"
#include "Light.hpp"
#include "LitMaterial.hpp"
#include "UnlitMaterial.hpp"


class KenomaEngine final : public Application
{
protected:
    void AfterCreatedUiContext() override;
    void BeforeDestroyUiContext() override;
    bool Load() override;
    void RenderScene(float deltaTime) override;
    void RenderUI(float deltaTime) override;
    void Update(float deltaTime) override;
    void ProcessKeyboardInput(GLFWwindow *window, float deltaTime) override;
    void ProcessMousePosition(float deltaTime) override;

public:
    KenomaEngine();

private:
    Model _model;
    std::vector<GameObject> _scene;
    std::vector<Model> _models;
    std::vector<Light> _lights;
    Shader litShader, unlitShader;
    LitMaterial litMaterial;
    UnlitMaterial unlitMaterial, outlineMaterial;

    unsigned int cubemapID;

    float _elapsedTime = 0.0f;

    Camera camera;
    bool firstMouse = true;
    float lastX = 100, lastY = 100;

    void LoadModel(std::string_view filePath);
//    void KeyboardInputCallback(GLFWwindow *window, int key, int scancode, int action, int mods);


};