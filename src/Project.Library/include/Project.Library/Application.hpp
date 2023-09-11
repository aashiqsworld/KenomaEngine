#pragma once
#include <cstdint>

struct GLFWwindow;

class Application
{
public:
    void Run();

protected:
    void Close();
    bool IsKeyPressed(int32_t key);
    void GetMousePosition(double *xpos, double *ypos);
    
    double GetDeltaTime();

    virtual void AfterCreatedUiContext();
    virtual void BeforeDestroyUiContext();
    virtual bool Initialize();
    virtual bool Load();
    virtual void Unload();
    virtual void RenderScene(float deltaTime);
    virtual void RenderUI(float deltaTime);
    virtual void Update(float deltaTime);
    virtual void ProcessKeyboardInput(GLFWwindow *window, float deltaTime);
    virtual void ProcessMousePosition(float deltaTime);


private:
    GLFWwindow* _windowHandle = nullptr;
    void Render(float deltaTime);

};