#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;
layout (location = 2) in float vStartTime;
layout (location = 3) in vec3 vCenter;

uniform mat4 transform;
uniform mat4 billboard;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 u_CameraPos;

out float de;
out vec2 texCoords;

void main() 
{
    vec4 rotatedPos = billboard * vec4((vPos - vCenter) * 2, 1.0);
    vec4 modifiedPos = vec4(vCenter, 1.0) + rotatedPos;


    gl_Position = projection * view * transform * modifiedPos;

    texCoords = vec2(vTexPos.x , -vTexPos.y);
    de = vStartTime;
}