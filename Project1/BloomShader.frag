#version 330 core

in vec2 texCoords;

uniform sampler2D u_Emissive;

out vec4 FragColor;

void main()
{
    FragColor = texture(u_Emissive, texCoords);
}
