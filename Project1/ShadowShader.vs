#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;

uniform mat4 transform;
uniform mat4 lightSpaceMatrix;

out vec2 OutTexPos;

void main() 
{
	gl_Position = lightSpaceMatrix * transform * vec4(vPos, 1.0); 
	OutTexPos = vTexPos;
}


