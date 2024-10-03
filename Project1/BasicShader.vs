#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;
layout (location = 2) in vec3 vNormal;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec3 vertex_normal;
out vec2 OutTexPos;
out vec3 WorldPosition;

void main() 
{
	gl_Position = projection * view * transform * vec4(vPos, 1.0); 

	WorldPosition = vec3(transform * vec4(vPos, 1.0));

	vertex_normal =  vec3(transform * vec4(vNormal, 1.0));

	OutTexPos = vTexPos;
}


