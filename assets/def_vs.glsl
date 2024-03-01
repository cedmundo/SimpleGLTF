#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inUvs;
layout (location = 3) in vec4 inCol;

out vec4 vCol;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
  gl_Position = proj * view * model * vec4(inPos, 1.0);
  vCol = inCol;
}
