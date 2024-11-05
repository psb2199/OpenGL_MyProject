#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;
layout (location = 2) in float vStartTime;
layout (location = 3) in vec3 vCenter;

uniform mat4 camera_rotation_matrix;
uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 u_CameraPos;

out float de;
out vec2 texCoords;

void main() 
{
    vec3 relativePos = vPos - vCenter;
    
    vec3 rotatedPos = (camera_rotation_matrix * vec4(relativePos, 1.0)).xyz;
    
    vec3 finalPos = rotatedPos + vCenter;
    
    gl_Position = projection * view * transform * vec4(finalPos, 1.0);

    texCoords = vec2(vTexPos.x , -vTexPos.y);
    de = vStartTime;
}