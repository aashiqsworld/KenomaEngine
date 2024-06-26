/**
 * KenomaEngine.cpp
 * The main class for the engine. Controls the GameObject loop and calls all the render functions.
 */

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Project/KenomaEngine.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <spdlog/spdlog.h>

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <queue>
#include <set>

namespace fs = std::filesystem;



// global var declarations
bool exitApplication = false;
bool mouseVisible = false;

// method declarations
void KeyboardInputCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void ResetStream(std::stringstream *stream);

void KenomaEngine::AfterCreatedUiContext()
{
}

void KenomaEngine::BeforeDestroyUiContext()
{
}

bool KenomaEngine::Load()
{
    if (!Application::Load())
    {
        spdlog::error("App: Unable to load");
        return false;
    }
    glfwSetKeyCallback(_windowHandle, KeyboardInputCallback);

    litMaterial.SetShader();
    unlitMaterial.SetShader();
    outlineMaterial.SetShader();
    outlineMaterial.outline = true;
    outlineMaterial.outlineScale = 1.01;

    _models.emplace_back("./data/models/AntiqueCameraTangents/AntiqueCamera.gltf", "AntiqueCamera");
    _models.emplace_back("./data/models/gltfCube/BoxWithSpaces.gltf");
    _models[1].Scale(0.05, 0.05, 0.05);
//    _scene.emplace_back("./data/models/Avocado/Avocado.gltf");
//    _scene.emplace_back("./data/models/ScifiHelmet/SciFiHelmet.gltf");
    _models.emplace_back(Model("./data/models/DamagedHelmetTangents/DamagedHelmet.gltf",
                               "DamagedHelmet"));
//    _models.emplace_back("./data/models/FireExtinguisher/FireExtinguisher.gltf");
//    _models.emplace_back("./data/models/SM_Deccer_Cubes_Textured_Complex.gltf");

//    _models.emplace_back("./data/models/Capsule/Capsule.gltf");
//    _models.emplace_back("./data/models/Plane/plane.gltf", "Plane");
//    _models.emplace_back("./data/models/TwoSidedPlane/TwoSidedPlane.gltf", "Plane");
//    _models[3].Scale(10, 10, 10);
//    _models[3].Translate(0, -1, 0);

//    _models[2].Translate()
//    _models.emplace_back(Model("./data/models/Cube/Cube.gltf"));

    camera = Camera(glm::vec3(0.0f, 0.0f, 7.0f));
    return true;
 }

void KenomaEngine::Update(float deltaTime)
{
    _elapsedTime += deltaTime;
}

void KenomaEngine::RenderScene([[maybe_unused]] float deltaTime)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDepthFunc(GL_LESS);



    const auto projection = glm::perspective(glm::radians(camera.Zoom), 1920.0f / 1080.0f, 0.1f, 256.0f);
    const auto view = camera.GetViewMatrix();

    // local data
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    Shader* activeShader = litMaterial._shader;
    activeShader->Bind();

    // set proj and view matrices
    activeShader->setMat4("uProjection", projection);
    activeShader->setMat4("uView", view);

    // set lighting-related data
    activeShader->setVec3("viewPos", camera.Position);

    activeShader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    activeShader->setInt("material.diffuse", 0);
    activeShader->setInt("material.specular", 1);
    activeShader->setFloat("material.shininess", 100.0f);
    activeShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    activeShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    activeShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    activeShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    activeShader->setVec3("pointLight.position", sinf((float)glfwGetTime()*3) * 1.5,  0.5f,  (cosf(
            (float)glfwGetTime()*3)) * 1.5);
    _models[1].transform.position = glm::vec3(sinf((float)glfwGetTime()*3) * 1.5,  0.5f,  (cosf(
            (float)glfwGetTime()*3)) * 1.5);
//    activeShader.setVec3("pointLight.position", 1.0f,  4.0f,  1.0f);
    activeShader->setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
    activeShader->setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
    activeShader->setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
    activeShader->setFloat("pointLight.constant", 1.0f);
    activeShader->setFloat("pointLight.linear", 0.09f);
    activeShader->setFloat("pointLight.quadratic", 0.032f);
    float brightness = 0.0f;
    activeShader->setVec3("spotLight.position", camera.Position);
    activeShader->setVec3("spotLight.direction", camera.Front);
    activeShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    activeShader->setVec3("spotLight.diffuse", brightness, brightness, brightness);
    activeShader->setVec3("spotLight.specular", brightness, brightness, brightness);
    activeShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5)));
    activeShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0)));
    activeShader->setFloat("spotLight.constant", 1.0f);
    activeShader->setFloat("spotLight.linear", 0.09f);
    activeShader->setFloat("spotLight.quadratic", 0.032f);

    activeShader = unlitMaterial._shader;
    activeShader->Bind();

    // set proj and view matrices
    activeShader->setMat4("uProjection", projection);
    activeShader->setMat4("uView", view);
    activeShader->setVec4("uColor", glm::vec4(0.7, 1.0, 1.0, 1.0));

    outlineMaterial._shader->Bind();
    outlineMaterial._shader->setMat4("uProjection", projection);
    outlineMaterial._shader->setMat4("uView", view);
    outlineMaterial._shader->setVec4("uColor", glm::vec4(1.0, 0.5, 0, 1.0));

    // set the lighting
    for(const auto& light : _lights)
    {

    }

    for(auto& model : _models)
    {
        model.UpdateTransforms();
    }


//    glStencilMask(0x00);
    unlitMaterial.Draw(_models[1]);

    glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
    glStencilMask(0xFF); // enable writing to the stencil buffer
    litMaterial.Draw(_models[2]);
//    litMaterial.Draw(_models[3]);


    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to the stencil buffer
    glDisable(GL_DEPTH_TEST);
    outlineMaterial._shader->Bind();
    outlineMaterial.Draw(_models[2]);

    // draw scaled up objects here

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

}

void KenomaEngine::RenderUI(float deltaTime)
{
    ImGui::Begin("OpenGL Rasterizer");
    {
        static int clicked = 0;
//        ImGui::TextUnformatted("Hello World!");
        ImGui::Text("Time in seconds since startup: %f", _elapsedTime);
        ImGui::Text("The delta time between frames: %f", deltaTime);
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << "Camera Position: " <<
        camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z;
        ImGui::Text(stream.str().c_str());
        ResetStream(&stream);
        stream << "Frames since startup: " << frameNumber;
        ImGui::Text(stream.str().c_str());

        ImGui::Separator();

        ResetStream(&stream);
        stream << "Num Models: " << _models.size();
        ImGui::Text("%s", stream.str().c_str());

        int c = 0;
        for(auto & _mod : _models)
        {
            ResetStream(&stream);
            stream << "Model " << _mod.name << " info:";
            ImGui::Text("%s", stream.str().c_str());

            ImGui::Indent();
            ResetStream(&stream);
            stream << "Num Meshes:  " << _mod.GetNumMeshes();
            ImGui::Text("%s", stream.str().c_str());

            ResetStream(&stream);
            stream << "Pos:  " << _mod.transform.position.x << ", " << _mod.transform.position.y
            << ", " << _mod.transform.position.z;
            ImGui::Text("%s", stream.str().c_str());

            ImGui::Indent();

            ImGui::PushID(c);
            if (ImGui::Button("+X"))
                _mod.Translate(0.5, 0, 0);
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::PushID(c);
            if (ImGui::Button("+Y"))
                _mod.Translate(0, 0.5, 0);
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::PushID(c);
            if (ImGui::Button("+Z"))
                _mod.Translate(0, 0, 0.5);
            ImGui::PopID();


            ImGui::PushID(c);
            if (ImGui::Button("-X"))
                _mod.Translate(-0.5, 0, 0);
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::PushID(c);
            if (ImGui::Button("-Y"))
                _mod.Translate(0, -0.5, 0);
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::PushID(c);
            if (ImGui::Button("-Z"))
                _mod.Translate(0, 0, -0.5);
            ImGui::PopID();

            ImGui::Unindent();

            ImGui::Unindent();

            c++;
        }
        ImGui::End();
    }

    ImGui::ShowDemoWindow();
}


void KenomaEngine::ProcessKeyboardInput(GLFWwindow *window, float deltaTime)
{

    if(exitApplication)
    {
        Close();
    }



    float cameraSpeed = 2.5f * deltaTime;
    if(IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
        cameraSpeed *= 5;
    else if(IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || IsKeyPressed(GLFW_KEY_RIGHT_CONTROL))
        cameraSpeed *= 0.1;
    if(IsKeyPressed(GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, cameraSpeed);
    if(IsKeyPressed(GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, cameraSpeed);
    if(IsKeyPressed(GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, cameraSpeed);
    if(IsKeyPressed(GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, cameraSpeed);
    if(IsKeyPressed(GLFW_KEY_E))
        camera.ProcessKeyboard(UP, cameraSpeed);
    if(IsKeyPressed(GLFW_KEY_Q))
        camera.ProcessKeyboard(DOWN, cameraSpeed);
}

void KenomaEngine::ProcessMousePosition(float deltaTime)
{
    double xpos, ypos;
    GetMousePosition(&xpos, &ypos);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // calculate the mouse's offset since the last frame
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // reversed since y-coords go bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

KenomaEngine::KenomaEngine() : _model() { }


void KeyboardInputCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && mouseVisible)
    {
        exitApplication = true;
    }
    else if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && !mouseVisible)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseVisible = true;
    }

}


void ResetStream(std::stringstream *stream)
{
    stream->str("");
    stream->clear();
}

