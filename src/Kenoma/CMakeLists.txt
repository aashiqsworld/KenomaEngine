cmake_minimum_required(VERSION 3.14)
project(KenomaEngine)

if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_BINARY_DIR})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_BINARY_DIR})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_CURRENT_BINARY_DIR})
    endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
endif()

add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)   

set(sourceFiles
    Main.cpp
        KenomaEngine.cpp
        Camera.hpp
        Shader.hpp
        Mesh.hpp
        Mesh.cpp
        Model.hpp
        Model.cpp
        Light.hpp
        Light.hpp
        GameObject.hpp
        GameObject.cpp
        Light.cpp
        Light.cpp
        LitMaterial.hpp
        LitMaterial.cpp
        Material.cpp
        Material.hpp
        UnlitMaterial.cpp
        UnlitMaterial.hpp
        EngineTools.cpp
        EngineTools.hpp
        EngineTools.hpp
        EngineTools.cpp
        Asset.hpp
        Asset.hpp
        Asset.hpp
        Cubemap.hpp
        Cubemap.cpp
)

add_executable(KenomaEngine ${sourceFiles})
add_dependencies(KenomaEngine copy_data)

target_include_directories(KenomaEngine PRIVATE include)

target_link_libraries(KenomaEngine PRIVATE glad glfw imgui glm cgltf stb_image spdlog Kenoma.Engine)
