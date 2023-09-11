#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iUv;
layout (location = 3) in vec4 iTangent;

layout (location = 0) out vec2 oUvs;
layout (location = 1) out flat uint oBaseColorIndex;
layout (location = 2) out flat uint oNormalIndex;

out vec3 Normal;
out vec3 FragPos;

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
    oUvs = iUv;
    oBaseColorIndex = objectData[gl_DrawID].baseColorIndex;
    Normal = mat3(transpose(inverse(transforms[objectData[gl_DrawID].transformIndex]))) * iNormal;
    FragPos = vec3(transpose(inverse(transforms[objectData[gl_DrawID].transformIndex])) * vec4(iPosition, 1.0));
    gl_Position = uProjection * uView * transforms[objectData[gl_DrawID].transformIndex] * vec4(iPosition, 1.0);
}