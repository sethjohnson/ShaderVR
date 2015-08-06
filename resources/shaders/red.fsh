#version 330
in vec3 outColor;
out vec4 oColor;
uniform int thread;
uniform int viewport;

void main () {
	if (thread == 0)
 		 oColor = vec4(1,0,float(viewport),1);
 	else if (thread == 1)
 		 oColor = vec4(0,1,float(viewport),1);
	else if (thread == 2)
		 oColor = vec4(0,0,float(viewport),1);
}