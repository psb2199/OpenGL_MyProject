#version 330 core

in vec2 texCoords;
in vec3 WorldPosition;
in vec3 vertex_normal;
in vec3 vertex_Tangent;
in vec3 vertex_BitTangent;

uniform vec3 u_CameraPos;

uniform samplerCube u_enviroment;

uniform sampler2D u_BaseColor;
uniform sampler2D u_NormalMap;
uniform sampler2D u_Emissive;
uniform sampler2D u_ARM;

uniform sampler2D u_DepthMap;
in vec4 LightSpacePos;
uniform float u_ShadowMapSize;
float Shadow_minValue = 0.2;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightDistance;
uniform bool u_cast_shadow;


out vec4 Fragcolor;

float       GetAO()            { return texture(u_ARM, texCoords).r; }
float       GetRoughness()     { return texture(u_ARM, texCoords).g; }
float       GetMetallic()       { return texture(u_ARM, texCoords).b; }

vec3        GetBaseColor()     { return texture(u_BaseColor, texCoords).rgb; }
vec3        GetEmissive()      { return texture(u_Emissive, texCoords).rgb; }

vec3 GetWorldNormalMap()
{
    mat3 TBN = mat3(normalize(vertex_Tangent), 
                    normalize(vertex_BitTangent), 
                    normalize(vertex_normal));

    vec3 tangentNormal = texture(u_NormalMap, texCoords).rgb;
   
    tangentNormal = tangentNormal * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
}

vec3 ToGrayScale(vec3 color)
{
    float gray = dot(color, vec3(0.299, 0.587, 0.114));

    return vec3(gray);
}

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

float GetLightMask(vec3 normal)
{
    // 광원의 방향 계산
    vec3 lightDir = normalize(lightPos - 0); //direction light
    //vec3 lightDir = normalize(lightPos - WorldPosition); //point light
   
    float lightMask = dot(normal, lightDir);

    if(u_cast_shadow) return 0.0;
    else return float(max(CalShadowFactor() * lightMask, Shadow_minValue));
}

vec3 CalReflectVector(vec3 normal)
{
    vec3 CameraDir = normalize(WorldPosition - u_CameraPos);

    return CameraDir + 2 * normal * (dot(-CameraDir, normal));
}

vec3 GetReflectedColor(vec3 normal, float roughness)
{
    vec3 colorSum = vec3(0.0);
    int range = 2; 
    float weight = 1.0 / pow((range * 2 + 1), 3); 

    float offsetScale = roughness * 0.1; 

    for (int x = -range; x <= range; ++x) {
        for (int y = -range; y <= range; ++y) {
            for (int z = -range; z <= range; ++z) {

                vec3 offset = vec3(x, y, z) * offsetScale;
                
                vec3 reflectVec = CalReflectVector(normal) + offset;
                colorSum += texture(u_enviroment, reflectVec).rgb * weight;
            }
        }
    }
    vec3 result = texture(u_enviroment, CalReflectVector(normal)).rgb;

    return mix(result, colorSum, roughness);
}

void Render(vec3 BaseColor, vec3 NormalMap, float AO, float Roughness, float Metallic, vec3 Emissive)
{   
    vec3 resultColor = BaseColor;

    resultColor = vec3(1.0);
    Metallic = 0;
    Roughness = 0;

    //Roughness
    vec3 reflectedColor = GetReflectedColor(NormalMap, Roughness);
    resultColor -= vec3(ToGrayScale(reflectedColor)) * 0.5;


    //Metallic
    vec3 re_MetallicMaskedColor = resultColor * (1 - Metallic);
    vec3 MetallicMaskedColor = resultColor * reflectedColor * Metallic;
    resultColor = re_MetallicMaskedColor + MetallicMaskedColor;

    //Light
    resultColor *= GetLightMask(NormalMap);

    //Ambient Occulusion
    //resultColor *= AO;

    //Emissive
    //resultColor += Emissive

    Fragcolor = vec4(resultColor, 1.0);
}

void main()
{
    Render(
    GetBaseColor(),
    GetWorldNormalMap(),
    GetAO(),
    GetRoughness(),
    GetMetallic(),
    GetEmissive()
    );

    //vec3 DiffultColor = Fragcolor.rgb;
    //vec3 MetallicMaskedColor = DiffultColor * GetReflectedColor(GetWorldNormalMap()) * GetMetallic();
    //vec3 re_MetallicMaskedColor = DiffultColor * (1-GetMetallic());

    //Fragcolor = vec4(re_MetallicMaskedColor + MetallicMaskedColor + GetEmissive(), 1.0);
}