#version 330 core

in vec2 OutTexPos;

uniform sampler2D u_Texture;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 Fragcolor;

void main()
{
	float x = OutTexPos.x;
	float y = -OutTexPos.y;
	vec2 newTexPos = vec2(x, y);

	Fragcolor = texture(u_Texture, newTexPos);
	//Fragcolor = vec4(OutTexPos, 0,1);
}
