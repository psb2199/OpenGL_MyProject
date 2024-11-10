#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;

out vec2 texCoords;

void main() 
{
	gl_Position = vec4(vPos, 1.0); 

	texCoords = vec2(vTexPos.x , vTexPos.y);
}


