#version 330 core

in vec2 texCoords;

uniform sampler2D u_Bloom;

out vec4 Fragcolor;

void main()
{
    Fragcolor = texture(u_Bloom, texCoords);
   //Fragcolor = vec4(Fragcolor.r, Fragcolor.g, Fragcolor.b, 1.0);
    //Fragcolor = vec4(texCoords,0.0,1.0);
}
