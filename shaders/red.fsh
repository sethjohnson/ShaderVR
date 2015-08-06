#version 330
in vec3 outColor;
out vec4 oColor;
uniform int thread;
void main () {
	if (thread == 0)
 		 oColor = vec4(1,0,0,1);
 	else if (thread == 1)
 		 oColor = vec4(0,1,0,1);
	else if (thread == 2)
		 oColor = vec4(0,0,1,1);
}