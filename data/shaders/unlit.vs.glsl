#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iUv;
layout (location = 3) in vec3 iTangent;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform mat4 uView;

struct ObjectData
{
    uint transformIndex;
};

layout (binding = 0) buffer BObjectData
{
    ObjectData[] objectData;
};

layout (binding = 1) buffer BTransforms
{
    mat4[] transforms;
};

void main()
{
    mat4 modelMatrix = transforms[objectData[gl_DrawID].transformIndex];
    mat4 normalMatrix = transpose(inverse(modelMatrix));



    gl_Position = uProjection * uView * modelMatrix * vec4(iPosition, 1.0);
}