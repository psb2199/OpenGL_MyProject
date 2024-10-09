#version 330 core

in vec2 OutTexPos;

uniform sampler2D u_Emissive;

out vec4 Fragcolor;

void main()
{
    Fragcolor = texture(u_Emissive, OutTexPos);
    Fragcolor = vec4(1.0);
}
