#version 330 core

in vec2 OutTexPos;
in vec3 WorldPosition;
in vec3 vertex_normal;
in vec3 vertex_Tangent;
in vec3 vertex_BitTangent;

uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;
uniform sampler2D u_Emissive;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightDistance;

out vec4 Fragcolor;

vec3 GetWorldNormalMap_texture(vec2 texCoords, mat3 TBN)
{
    vec3 tangentNormal = texture(u_NormalMap, texCoords).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
}

vec3 GetBaseColor_texture(vec2 texCoords)
{
    return texture(u_BaseColor, texCoords).rgb;
}

vec3 GetEmissive_texture(vec2 texCoords)
{
    return texture(u_Emissive, texCoords).rgb;
}


float GetLightMask(vec3 worldNormal, vec3 lightDir, vec3 fragPosition, vec3 lightPos, float lightDistance)
{
    float lightMask = max(dot(worldNormal, lightDir), 0) 
                    * max((1 - distance(fragPosition, lightPos) / lightDistance), 0);

    float Highlight = pow(lightMask, 64);

    return lightMask + Highlight;
}

void RenderMaterial()
{
    vec2 newTexPos = vec2(OutTexPos.x, -OutTexPos.y); // 텍스처 좌표 변환

    // TBN 매트릭스 계산
    mat3 TBN = mat3(normalize(vertex_Tangent), 
                    normalize(vertex_BitTangent), 
                    normalize(vertex_normal));

    vec3 worldNormal = GetWorldNormalMap_texture(newTexPos, TBN);

    // 광원의 방향 계산
    //vec3 lightDir = normalize(lightPos - 0); //direction light
    vec3 lightDir = normalize(lightPos - WorldPosition); //point light

    // 조명 마스크 계산
    float lightMask = GetLightMask(worldNormal, lightDir, WorldPosition, lightPos, lightDistance);

    // 최종 색상 계산
    Fragcolor = vec4(lightMask * GetBaseColor_texture(newTexPos) + GetEmissive_texture(newTexPos), 1.0);
}

void main()
{
    RenderMaterial();
}
