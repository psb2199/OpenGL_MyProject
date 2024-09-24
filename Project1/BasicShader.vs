#version 330 core

in vec3 vPos;
in vec3 vColor;

uniform mat4 transform;

out vec3 OutColor;

void main() 
{
	gl_Position =  transform * vec4(vPos, 1.0); 

	OutColor = vColor;
}


