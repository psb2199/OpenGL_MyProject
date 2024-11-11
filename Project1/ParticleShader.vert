#version 330 core

layout (location = 0) in vec3   vPos;
layout (location = 1) in vec2   vTexPos;
layout (location = 2) in float  vrandomSeed;
layout (location = 3) in vec3   vCenter;
layout (location = 4) in vec3   vVelocity;

uniform mat4            transform;
uniform mat4            billboard;
uniform mat4            projection;
uniform mat4            view;
uniform vec3            u_CameraPos;


uniform float           elapsedTime;
uniform float           lifeTime;

uniform vec3            follow_location;
uniform vec3            follow_velocity;
uniform vec3            afterVelocity;
uniform vec3            actor_location;

out vec2 texCoords;
out float randomSeed;


uniform int             particle_type;
const   int             P_PopCoin = 0;
const   int             P_Leaf = 1;

mat4 rotationX(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    mat4 translateToCenter = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -vCenter.x, -vCenter.y, -vCenter.z, 1.0
    );

    mat4 rotation = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, c, -s, 0.0,
        0.0, s, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 translateBack = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        vCenter.x, vCenter.y, vCenter.z, 1.0
    );

    return translateBack * rotation * translateToCenter;
}
mat4 rotationY(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    mat4 translateToCenter = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -vCenter.x, -vCenter.y, -vCenter.z, 1.0
    );

    mat4 rotation = mat4(
        c, 0.0, s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -s, 0.0, c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 translateBack = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        vCenter.x, vCenter.y, vCenter.z, 1.0
    );

    return translateBack * rotation * translateToCenter;
}
mat4 rotationZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    mat4 translateToCenter = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -vCenter.x, -vCenter.y, -vCenter.z, 1.0
    );

    mat4 rotation = mat4(
        c, -s, 0.0, 0.0,
        s, c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    mat4 translateBack = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        vCenter.x, vCenter.y, vCenter.z, 1.0
    );

    return translateBack * rotation * translateToCenter;
}

vec4 DoBillBoard()
{
    vec4 Pos = vec4(vCenter, 1.0) + billboard * vec4((vPos - vCenter) * 2, 1.0);

    return Pos;
}

void PopCoin()
{
    vec4 Pos = DoBillBoard();

    vec3 addMovement = elapsedTime * vVelocity;
    float gravity = 50.0;
    addMovement.y -= 0.5 * gravity * elapsedTime * elapsedTime;
    Pos.xyz += addMovement;

    gl_Position = projection * view * transform * Pos;
}

void Leaf()
{   
    float speed = 0.75;
    float T = elapsedTime * speed;
    mat4 rotate = rotationX(vVelocity.x * T) * rotationY(vVelocity.y * T) * rotationZ(vVelocity.z);

    vec4 Pos = rotate * vec4(vPos, 1.0);

    vec3 addMovement = elapsedTime * (vVelocity + afterVelocity * 30);
    float gravity = 50.0;
    addMovement.y -= 0.5 * gravity * elapsedTime * elapsedTime;
    Pos.xyz += addMovement;

    gl_Position = projection * view * transform * Pos;
}

void main() 
{
    

    switch( particle_type )
    {
    case P_PopCoin: PopCoin();
        break;

    case P_Leaf: Leaf();
        break;
    }
    
    texCoords = vec2(vTexPos.x , -vTexPos.y);
    randomSeed = vrandomSeed;
}