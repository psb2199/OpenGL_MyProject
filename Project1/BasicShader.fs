#version 330 core

in vec2 OutTexPos;
in vec3 WorldPosition;
in vec3 vertex_normal;

uniform sampler2D u_Texture;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 Fragcolor;

void main()
{
	float x = OutTexPos.x;
	float y = -OutTexPos.y;
	vec2 newTexPos = vec2(x, y);

	vec3 lightDir = normalize(lightPos - WorldPosition); //Point Light
	//vec3 lightDir = normalize(lightPos - 0); //Direction Light

	float lgihtMask = max(dot(vertex_normal, lightDir), 0);
	

	//Fragcolor = texture(u_Texture, newTexPos);
	//Fragcolor = vec4(vertex_normal, 1.0);
	Fragcolor = vec4(vec3(lgihtMask), 1.0);
}
