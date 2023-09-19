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

    litShader.LoadShader("./data/shaders/main.vs.glsl", "./data/shaders/main.fs.glsl");

//     _scene.emplace_back("./data/models/SM_Deccer_Cubes_Textured_Complex.gltf");
//    _scene.emplace_back("./data/models/FireExtinguisher/FireExtinguisher.gltf");
    _scene.emplace_back("./data/models/AntiqueCamera/AntiqueCamera.gltf");
//    _scene.emplace_back("./data/models/gltfCube/BoxWithSpaces.gltf");
//    _scene.emplace_back("./data/models/Avocado/Avocado.gltf");
//    _scene.emplace_back("./data/models/ScifiHelmet/SciFiHelmet.gltf");
//    _scene.emplace_back("./data/models/DamagedHelmet/DamagedHelmet.gltf");


    camera = Camera(glm::vec3(0.0f, 0.0f, 7.0f));
    return true;
}

void KenomaEngine::Update(float deltaTime)
{
    _elapsedTime += deltaTime;
}

void KenomaEngine::RenderScene([[maybe_unused]] float deltaTime)
{
    const auto projection = glm::perspective(glm::radians(camera.Zoom), 1920.0f / 1080.0f, 0.1f, 256.0f);
    const auto view = camera.GetViewMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    litShader.Bind();
    glUniformMatrix4fv(0, 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(1, 1, false, glm::value_ptr(view));

    // local data
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // set lighting-related data
    litShader.setVec3("viewPos", camera.Position);

    litShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    litShader.setInt("material.diffuse", 0);
    litShader.setInt("material.specular", 1);
    litShader.setFloat("material.shininess", 32.0f);

    litShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    litShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    litShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    litShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    litShader.setVec3("pointLight.position", sinf((float)glfwGetTime()) * 4,  3.0f,  (cosf((float)glfwGetTime())) * 4);
    litShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
    litShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
    litShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
    litShader.setFloat("pointLight.constant", 1.0f);
    litShader.setFloat("pointLight.linear", 0.09f);
    litShader.setFloat("pointLight.quadratic", 0.032f);

    float brightness = 0.0f;
    litShader.setVec3("spotLight.position", camera.Position);
    litShader.setVec3("spotLight.direction", camera.Front);
    litShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    litShader.setVec3("spotLight.diffuse", brightness, brightness, brightness);
    litShader.setVec3("spotLight.specular", brightness, brightness, brightness);
    litShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5)));
    litShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0)));
    litShader.setFloat("spotLight.constant", 1.0f);
    litShader.setFloat("spotLight.linear", 0.09f);
    litShader.setFloat("spotLight.quadratic", 0.032f);

    for(const auto& model : _scene)
    {
        model.Draw(litShader);
    }
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
        stream << "Num Models: " << _scene.size();
        ImGui::Text("%s", stream.str().c_str());

        for(auto modelIndex = 0; modelIndex < _scene.size(); modelIndex++)
        {
            ResetStream(&stream);
            stream << "Model " << modelIndex << " info:";
            ImGui::Text(stream.str().c_str());

            ImGui::Indent();
            ResetStream(&stream);
            stream << "Num Meshes:  " << _scene[modelIndex].GetNumMeshes();
            ImGui::Text(stream.str().c_str());

            ImGui::Indent();
            for(auto meshIndex = 0; meshIndex < _scene[modelIndex].GetNumMeshes(); meshIndex++)
            {

            }
            ImGui::Unindent();

            ImGui::Unindent();
        }
        ImGui::End();
    }

//    ImGui::ShowDemoWindow();
}


void KenomaEngine::ProcessKeyboardInput(GLFWwindow *window, float deltaTime)
{

    if(exitApplication)
    {
        Close();
    }

//    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseVisible)
//    {

//    }

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

