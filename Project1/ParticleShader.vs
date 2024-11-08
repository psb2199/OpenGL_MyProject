#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexPos;
layout (location = 2) in float vfadeOut_time;
layout (location = 3) in vec3 vCenter;
layout (location = 4) in vec3 vVelocity;

uniform mat4 transform;
uniform mat4 billboard;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 u_CameraPos;

uniform float elapsedTime;

uniform vec3 follow_location;


out vec2 texCoords;
out float fadeOut_time;

void main() 
{
    vec4 Pos = vec4(vCenter, 1.0) + billboard * vec4((vPos - vCenter) * 2, 1.0);

    vec3 addMovement = elapsedTime * vVelocity;
    float gravity = 50.0;
    addMovement.y -= 0.5 * gravity * elapsedTime * elapsedTime;
    Pos.xyz += addMovement;

     //Pos.xyz = mix(Pos.xyz, follow_location, elapsedTime);

    gl_Position = projection * view * transform * Pos;

    texCoords = vec2(vTexPos.x , -vTexPos.y);
    fadeOut_time = vfadeOut_time;
}