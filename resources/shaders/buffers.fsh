#version 410
in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform int thread;
uniform int viewport;

void main () {
			float depth = (-1*texture(depthTexture,fragTexCoord).x+1)*40;

if((int(fragTexCoord.y * 100) %10)/5 == 0)
discard; 
	if (fragTexCoord.y > 0.99 )
	{
		outColor = vec4(depth, depth, depth, 1);
	}
	else
	{
		outColor = texture(colorTexture,fragTexCoord);

	}
}
