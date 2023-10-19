#version 460 core

layout (location = 0) out vec4 oPixel;

layout (location = 0) in vec2 iUvs;
layout (location = 1) in flat uint iBaseColorIndex;
layout (location = 2) in flat uint iNormalIndex;
layout (location = 3) in vec3 iNormal;
layout (location = 4) in vec3 iFragPos;
layout (location = 5) in vec3 iTangent;
layout (location = 6) in vec3 iBitangent;

void main()
{
    oPixel = vec4(1.0, 1.0, 1.0, 1.0);
}
