#version 330 core

in vec3 OutColor;

out vec4 Fragcolor;

void main()
{
	

	Fragcolor = vec4(OutColor, 1.0);
}
