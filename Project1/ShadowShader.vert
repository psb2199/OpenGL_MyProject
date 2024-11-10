#version 330 core

layout (location = 0) in vec3 vPos;

uniform mat4 transform;
uniform mat4 lightSpaceMatrix;

out float depth;

void main() 
{
	gl_Position = lightSpaceMatrix * transform * vec4(vPos, 1.0); 
	depth = (gl_Position.z + 1) / 2;
}


