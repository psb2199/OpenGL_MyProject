#version 330 core

in vec2 OutTexPos;
in vec3 WorldPosition;
in vec3 vertex_normal;

uniform sampler2D u_Texture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightDistance;

out vec4 Fragcolor;

void RenderMaterial()
{
	float x = OutTexPos.x;
	float y = -OutTexPos.y;
	vec2 newTexPos = vec2(x, y);
	vec4 BaseColor = texture(u_Texture, newTexPos);

	vec3 lightDir = normalize(lightPos - WorldPosition); //Point Light
	//vec3 lightDir = normalize(lightPos - 0); //Direction Light

	float lgihtMask = max(dot(vertex_normal, lightDir), 0) 
					* max((1 - distance(WorldPosition, lightPos) / lightDistance), 0);

	float Highlight = pow(lgihtMask, 64);

	vec4 ColorLight = vec4(lightColor * vec3(lgihtMask + Highlight), 1.0);
	
	
	Fragcolor = ColorLight * BaseColor;
}

void Preview_BaseColor()
{
	float x = OutTexPos.x;
	float y = -OutTexPos.y;
	vec2 newTexPos = vec2(x, y);
	vec4 BaseColor = texture(u_Texture, newTexPos);

	Fragcolor = BaseColor;
}

void Preview_LightMask()
{
}

void Preview_Normal()
{
}

void main()
{
	Preview_BaseColor();
}

