#version 330 core

in vec2 OutTexPos;
in vec3 WorldPosition;
in vec3 vertex_normal;

uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightDistance;

out vec4 Fragcolor;

void RenderMaterial()
{
	float x = OutTexPos.x;
	float y = -OutTexPos.y;
	vec2 newTexPos = vec2(x, y);
	vec4 BaseColor = texture(u_BaseColor, newTexPos);

	vec4 NormalMap = texture(u_NormalMap, newTexPos) * 2 - 1;

	NormalMap = vec4(vertex_normal, 1) * vec4(NormalMap.g, NormalMap.b, NormalMap.r, 1);

	vec3 lightDir = normalize(lightPos - WorldPosition); //Point Light
	//vec3 lightDir = normalize(lightPos - 0); //Direction Light

	float lightMask = max(dot(vec3(NormalMap), lightDir), 0) 
					* max((1 - distance(WorldPosition, lightPos) / lightDistance), 0);

	float Highlight = pow(lightMask, 64);

	vec4 ColorLight = vec4(lightColor * vec3(lightMask + Highlight), 1.0);
	
	
	//Fragcolor = ColorLight;
	Fragcolor = ColorLight * BaseColor;
}

void Preview_BaseColor()
{
	float x = OutTexPos.x;
	float y = -OutTexPos.y;
	vec2 newTexPos = vec2(x, y);
	vec4 BaseColor = texture(u_BaseColor, newTexPos);

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
	RenderMaterial();

}

