#version 330 core

in vec2 texCoords;

uniform sampler2D u_Bloom;

out vec4 Fragcolor;

void main()
{
   
    Fragcolor = texture(u_Bloom, texCoords);  
}