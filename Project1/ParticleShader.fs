#version 330 core

in   vec2               texCoords;

out  vec4               Fragcolor;

uniform sampler2D       u_BaseColor;
uniform sampler2D       u_Emissive;

void main()
{
    vec4 result = texture(u_BaseColor, texCoords);

    Fragcolor = result;
}