cmake_minimum_required(VERSION 3.14)
project(Kenoma.Engine)

add_subdirectory(lib)

set(sourceFiles
    Application.cpp
)

add_library(Kenoma.Engine ${sourceFiles})

find_package(OpenGL REQUIRED)

target_include_directories(Kenoma.Engine PUBLIC include)

target_link_libraries(Kenoma.Engine PRIVATE glfw glad glm TracyClient spdlog imgui assimp)
