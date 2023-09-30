#version 460 core

layout (location = 0) in vec3 iBaseColor;

layout (location = 0) out vec4 oPixel;

void main()
{
    oPixel = vec4(iBaseColor, 1.0);
}
