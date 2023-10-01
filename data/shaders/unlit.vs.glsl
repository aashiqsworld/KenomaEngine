#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iBaseColor;

layout (location = 0) out vec3 oBaseColor;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform mat4 uView;

struct ObjectData
{
    uint transformIndex;
    uint baseColorIndex;
    uint normalIndex;
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
    oBaseColor = iBaseColor;

    mat4 modelMatrix = transforms[objectData[gl_DrawID].transformIndex];
    gl_Position = uProjection * uView * modelMatrix * vec4(iPosition, 1.0);
}