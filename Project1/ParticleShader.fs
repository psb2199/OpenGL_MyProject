#version 330 core

in   vec2               texCoords;
in   float               de;

out  vec4               Fragcolor;

uniform sampler2D       u_BaseColor;
uniform sampler2D       u_Emissive;

void main()
{
    vec4 result = texture(u_BaseColor, texCoords);

    //Fragcolor = vec4(vec3(de), 1.0);
    Fragcolor = result;
}