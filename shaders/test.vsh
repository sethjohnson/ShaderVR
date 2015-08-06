#version 410
layout(location = 1) in vec3 position;
layout(location = 0) in vec3 color;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 fColor;
out vec3 fPosition;
out vec3 fNormal;
void main() {
  fColor = color;
  gl_Position = proj * view * model * vec4(position,1.0);
  fPosition = (model * vec4(position,1.0)).xyz;
  fNormal = normalize((transpose(inverse(model)) * vec4(normal,1.0)).xyz);
}