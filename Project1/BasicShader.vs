#version 330 core

in vec3 vPos;
in vec3 vColor;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec3 OutColor;

void main() 
{
	gl_Position = projection * view * transform * vec4(vPos, 1.0); 
	//gl_Position =transform * vec4(vPos, 1.0); 

	OutColor = vColor;
}


