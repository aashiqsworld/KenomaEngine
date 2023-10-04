#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iUv;
layout (location = 3) in vec3 iTangent;

layout (location = 0) out vec2 oUvs;
layout (location = 1) out flat uint oBaseColorIndex;
layout (location = 2) out flat uint oNormalIndex;

layout (location = 3) out vec3 oNormal;
layout (location = 4) out vec3 oFragPos;
layout (location = 5) out vec3 oTangent;
layout (location = 6) out vec3 oBitangent;


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
    mat4 modelMatrix = transforms[objectData[gl_DrawID].transformIndex];
    mat4 normalMatrix = transpose(inverse(modelMatrix));

    oUvs = iUv;
    oBaseColorIndex = objectData[gl_DrawID].baseColorIndex;
    oNormalIndex = objectData[gl_DrawID].normalIndex;
    oFragPos = vec3(modelMatrix * vec4(iPosition, 1.0));
    oNormal = vec3(normalMatrix * vec4(iNormal, 1.0));
    oTangent = vec3(normalMatrix * vec4(iTangent, 1.0));
    oBitangent = cross(oNormal, oTangent);



    gl_Position = uProjection * uView * modelMatrix * vec4(iPosition, 1.0);
}