#version 410
in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

void main () {
			float depth = (-1*texture(depthTexture,fragTexCoord).x+1)*40;

if((fragTexCoord.y * 100) %10 < 3)
discard; 
	if (fragTexCoord.y > 0.5)
	{
		outColor = vec4(depth, depth, depth, 1);
	}
	else
	{
		outColor = texture(colorTexture,fragTexCoord);

	}
}
