#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vColor;
layout (location = 2) in vec3 vNormal;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec3 OutColor;

void main() 
{
	gl_Position = projection * view * transform * vec4(vPos, 1.0); 

	//OutColor = vec3(vColor,0);
	OutColor = vNormal;
}


