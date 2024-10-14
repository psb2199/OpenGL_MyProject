#version 330 core

in vec2 texCoords;
in vec3 WorldPosition;
in vec3 vertex_normal;
in vec3 vertex_Tangent;
in vec3 vertex_BitTangent;

uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;
uniform sampler2D u_Emissive;

uniform sampler2D u_DepthMap;
in vec4 LightSpacePos;
uniform float u_ShadowMapSize;
float Shadow_minValue = 0.2;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightDistance;
uniform bool u_cast_shadow;

out vec4 Fragcolor;


float CalShadowFactor()
{
    // 그림자 좌표를 NDC로 변환 (0.0 ~ 1.0 사이로 정규화)
    vec3 projCoords = LightSpacePos.xyz / LightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    // 그림자 좌표가 범위 밖이면 그림자 적용 안 함
    if (projCoords.z > 1.0)
        return 1.0;

    // PCF 샘플링 범위
    float shadow = 0.0;
    float bias = 0.005;
    int samples = 5;  // 샘플 갯수
    float texelSize = 1.0 / u_ShadowMapSize;  // 그림자 맵 해상도에 따른 텍셀 크기

    for (int x = -samples; x <= samples; ++x)
    {
        for (int y = -samples; y <= samples; ++y)
        {
            float pcfDepth = texture(u_DepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (2.0 * samples + 1.0) * (2.0 * samples + 1.0);

    return 1.0 - (shadow);  // 그림자의 강도 반환
}

vec3 GetWorldNormalMap_texture(vec2 Coords, mat3 TBN)
{
    vec3 tangentNormal = texture(u_NormalMap, Coords).rgb;
   
    tangentNormal = tangentNormal * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
    
}

vec3 GetBaseColor_texture(vec2 Coords)
{
    return texture(u_BaseColor, Coords).rgb;
}

vec3 GetEmissive_texture(vec2 Coords)
{
    return texture(u_Emissive, Coords).rgb;
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
    // TBN 매트릭스 계산
    mat3 TBN = mat3(normalize(vertex_Tangent), 
                    normalize(vertex_BitTangent), 
                    normalize(vertex_normal));

    vec3 worldNormalMap = GetWorldNormalMap_texture(texCoords, TBN);
    //vec3 worldNormalMap = vertex_normal;

    // 광원의 방향 계산
    vec3 lightDir = normalize(lightPos - 0); //direction light
    //vec3 lightDir = normalize(lightPos - WorldPosition); //point light

    // 조명 마스크 계산
    float lightMask = GetLightMask(worldNormalMap, lightDir, WorldPosition, lightPos, lightDistance);

    // 최종 색상 계산
    if(u_cast_shadow) {
        Fragcolor = vec4(GetBaseColor_texture(texCoords), 1.0);
    }
    else {
        Fragcolor = vec4( max(CalShadowFactor() * lightMask, Shadow_minValue) * GetBaseColor_texture(texCoords) + GetEmissive_texture(texCoords), 1.0);
    }
}


void main()
{
    RenderMaterial();
}