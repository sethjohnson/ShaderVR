#version 410
layout(location = 3) in vec3 fColor;
layout(location = 4) in vec3 fPosition;
layout(location = 5) in vec3 fNormal;

layout(location = 0) out vec4 outColor;
uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];
uniform int lightCount;
vec3 lightAmount = vec3(0,0,0);
vec3 D;
vec3 L;
vec4 Idiff;
void main () {
  for(int i = 0; i < lightCount; i++) {
    D = lightPositions[i].xyz - fPosition;
    L = normalize(D);
    Idiff = vec4(1,1,1,1)*max(dot(fNormal,L),0)*1/pow(length(D),1);
    lightAmount += lightColors[i]*Idiff.xyz;
  }
  outColor = vec4((lightAmount+ vec3(0.2,0.2,0.2))*fColor,1);
}