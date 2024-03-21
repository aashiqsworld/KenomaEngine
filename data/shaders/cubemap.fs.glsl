#version 120

in vec3 textureDir; // direction vector representing 3D texture coordinate
uniform samplerCube cubemap; // cubemap texture sampler

void main() {
    FragColor = texture(cubemap, textureDir);
}
