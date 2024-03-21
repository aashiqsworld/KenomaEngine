/**
 * Application.cpp
 * The base class that the engine is built on. Contains low-level functions for maintaining the
 * application window and input events and such.
 */

#include <Project.Library/Application.hpp>

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

#include <iostream>
#include <string>

void Application::Run()
{
    FrameMarkStart("App Run");
    if (!Initialize())
    {
        return;
    }

    spdlog::info("App: Initialized");

    if (!Load())
    {
        return;
    }

    spdlog::info("App: Loaded");

    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(_windowHandle))
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - previousTime);
        previousTime = currentTime;

        glfwPollEvents();
        ProcessKeyboardInput(_windowHandle, deltaTime);
        ProcessMousePosition(deltaTime);
        Update(deltaTime);
        Render(deltaTime);
    }

    spdlog::info("App: Unloading");

    Unload();

    spdlog::info("App: Unloaded");
    FrameMarkEnd("App Run");
}

void Application::Close()
{
    glfwSetWindowShouldClose(_windowHandle, 1);
}

bool Application::IsKeyPressed(int32_t key)
{
    return glfwGetKey(_windowHandle, key) == GLFW_PRESS;

}

void Application::GetMousePosition(double *xpos, double *ypos)
{
    glfwGetCursorPos(_windowHandle, xpos, ypos);
}

bool Application::Initialize()
{
    if (!glfwInit())
    {
        spdlog::error("Glfw: Unable to initialize");
        return false;
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    const auto primaryMonitor = glfwGetPrimaryMonitor();
    const auto primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);

    _windowHandle = glfwCreateWindow(windowWidth, windowHeight, "Kenoma Engine", nullptr, nullptr);
    if (_windowHandle == nullptr)
    {
        spdlog::error("Glfw: Unable to create window");
        glfwTerminate();
        return false;
    }

    const auto screenWidth = primaryMonitorVideoMode->width;
    const auto screenHeight = primaryMonitorVideoMode->height;
    glfwSetWindowPos(_windowHandle, screenWidth / 2 - windowWidth / 2, screenHeight / 2 - windowHeight / 2);

    glfwMakeContextCurrent(_windowHandle);
    glfwSetInputMode(_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    ImGui::CreateContext();
    AfterCreatedUiContext();
    ImGui_ImplGlfw_InitForOpenGL(_windowHandle, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    return true;
}

bool Application::Load()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDebugMessageCallback([](GLenum source,
                              GLenum type,
                              GLuint,
                              GLenum severity,
                              GLsizei,
                              const GLchar* message,
                              const void*)
    {
        if (type == GL_DEBUG_TYPE_ERROR)
        {
            spdlog::error("GL CALLBACK: type = {}, severity = error, message = {}\n", type, message);
        }
    }, nullptr);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    // --- Framebuffer initialization ---
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // generate texture
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           textureColorBuffer, 0);
    // create the render buffer object
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check if framebuffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    glfwSwapInterval(1);
    return true;
}

void Application::Unload()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    BeforeDestroyUiContext();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Application::Render(float dt)
{
    ZoneScopedC(tracy::Color::Red2);

    RenderScene(dt);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        RenderUI(dt);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();
    }

    glfwSwapBuffers(_windowHandle);
    frameNumber++;
}

void Application::RenderScene([[maybe_unused]] float dt)
{
}

void Application::RenderUI([[maybe_unused]] float dt )
{

}

void Application::Update([[maybe_unused]] float dt )
{

}

void Application::ProcessKeyboardInput(GLFWwindow *window, [[maybe_unused]] float dt) {

}

void Application::AfterCreatedUiContext()
{
}

void Application::BeforeDestroyUiContext()
{
}

void Application::ProcessMousePosition(float deltaTime) {

}
