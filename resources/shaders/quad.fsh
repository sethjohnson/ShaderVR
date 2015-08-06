
#version 410
in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D colorTexture;

void main () {
	outColor = texture(colorTexture,fragTexCoord);
}