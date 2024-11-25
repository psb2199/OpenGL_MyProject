#version 330 core

in vec2             texCoords;
in vec3             WorldPosition;
in vec3             vertex_normal;
in vec3             vertex_Tangent;
in vec3             vertex_BitTangent;

uniform             vec3 actor_location;

uniform             vec3 u_CameraPos;

uniform samplerCube u_enviroment;

uniform sampler2D   u_BaseColor;
uniform sampler2D   u_NormalMap;
uniform sampler2D   u_ARM;

uniform bool        u_testRendering;
uniform vec3        t_BaseColor;
uniform float       t_Metallic;
uniform float       t_Roughness;

uniform sampler2D   u_DepthMap;
in vec4             LightSpacePos;
uniform float       u_ShadowMapSize;
float               Shadow_minValue = 0.2;
vec3                ShadowColor = vec3(0, 0.025, 0.05);

uniform vec3        LightDir;
uniform vec3        lightColor;
float               lihgt_power = 1.2;
uniform bool        u_cast_shadow;

float PI = 3.141592;

out vec4 Fragcolor;


vec3 CameraDir = normalize(WorldPosition - u_CameraPos);
float Camera_Actor_Dis = distance(WorldPosition, actor_location);

float GetAO() 
{
    if(!u_testRendering) return texture(u_ARM, texCoords).r; 
    else return 1.f;
}
float GetRoughness() 
{ 
    if(!u_testRendering) return texture(u_ARM, texCoords).g; 
    else return t_Roughness;
}
float GetMetallic() 
{ 
     if(!u_testRendering) return texture(u_ARM, texCoords).b; 
      else return t_Metallic;
}

vec3 GetBaseColor() 
{   
    if(!u_testRendering) return texture(u_BaseColor, texCoords).rgb;
    else return t_BaseColor;
}



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
    float gray = dot(color, vec3(0.333, 0.333, 0.333));

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


vec3 CalReflectVector(vec3 normal)
{
    //return CameraDir + 2 * normal * (dot(-CameraDir, normal));
    vec3 modifyDir = vec3(CameraDir.x, CameraDir.y, CameraDir.z);
    return reflect(modifyDir, normal);
}

vec3 GetReflectedColor(vec3 normal)
{
    return texture(u_enviroment, CalReflectVector(normal)).rgb;
}

vec3 GetBlurReflectedColor(vec3 normal, float blurAmount)
{
    // 반사 벡터 계산
    vec3 reflectedDir = normalize(CalReflectVector(normal));

    // 흐림을 적용하기 위해 주변 방향을 샘플링
    vec3 color = vec3(0.0);
    int sampleCount = 8;  // 샘플링할 개수

    // 여러 방향에서 반사된 색상을 샘플링하여 평균화
    for (int i = 0; i < sampleCount; i++) {
        // 난수 기반 오프셋 방향을 생성 (여기서는 간단한 퍼터베이션 사용)
        vec3 offset = normalize(vec3(
            fract(sin(float(i)) * 43758.5453), 
            fract(cos(float(i)) * 96123.3542), 
            fract(sin(float(i) * 0.5) * 21391.1325)
        ));

        // 반사 벡터에 오프셋을 추가하고, blurAmount로 조절
        vec3 blurredDir = normalize(reflectedDir + offset * blurAmount);

        // 흐려진 방향으로 텍스처 샘플링
        color += texture(u_enviroment, blurredDir).rgb;
    }

    // 샘플한 값들의 평균을 반환 (합계를 샘플 개수로 나눔)
    color /= float(sampleCount);

    return color;
}



void Render(vec3 BaseColor, vec3 NormalMap, float AO, float Roughness, float Metallic)
{   
    vec3 resultColor = BaseColor;

    //resultColor = vec3(1.0);
    //Metallic = 1;
    //Roughness = 0;


    //Ambient Occulusion=====================================================
    resultColor *= AO;
    //=======================================================================



    //Roughness==============================================================
    vec3 reflectedColor = GetBlurReflectedColor(NormalMap, Roughness);
    resultColor *= reflectedColor;
    float Highlight = pow(dot(NormalMap, LightDir), 64);
    //=======================================================================



    //Metallic===============================================================
    vec3 re_MetallicMaskedColor = resultColor * (1 - Metallic);
    vec3 MetallicMaskedColor = resultColor * reflectedColor * Metallic;
    resultColor = re_MetallicMaskedColor + MetallicMaskedColor;
    //=======================================================================

    

    //Shadow=================================================================
    resultColor += Highlight;
    resultColor *= max(dot(LightDir, NormalMap) * CalShadowFactor(), Shadow_minValue);
    //=======================================================================
   

    Fragcolor = vec4(resultColor, 1.0);
}

// PBR 관련 수학적 함수들 
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

void PBR_Render(vec3 BaseColor, vec3 NormalMap, float AO, float Roughness, float Metallic)
{
    //BaseColor = vec3(1.0);
    //Roughness = 0.0;
    //Metallic = 1.0;

    vec3 N = normalize(NormalMap);
    vec3 V = normalize(u_CameraPos - WorldPosition);
    vec3 L = LightDir;
    vec3 H = normalize(V + L);
    

    vec3 F0 = mix(vec3(0.04), BaseColor, Metallic); 
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    

    float NDF = DistributionGGX(N, H, Roughness); 
    float G = GeometrySmith(N, V, L, Roughness); 
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;
    

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS; 
    kD *= 1.0 - Metallic; 


    float NdotL = max(dot(N, L), 0.0);
    vec3 Lo = (kD * BaseColor / PI + specular) * NdotL;

    vec3 LightValue = lightColor * lihgt_power;
    vec3 ambient = LightValue * BaseColor * AO;

    vec3 reflectedColor = GetBlurReflectedColor(N, Roughness);
    reflectedColor *= Metallic; 

   

    vec3 color = ambient + Lo;

    vec3 re_MetallicMaskedColor = color * (1 - Metallic);
    vec3 MetallicMaskedColor = color * reflectedColor * Metallic;
    color = re_MetallicMaskedColor + MetallicMaskedColor;

    float Shadow = CalShadowFactor();
    vec3 coloredShadow = (1.0 - Shadow) * ShadowColor;

    color *= max(Shadow * dot(LightDir, NormalMap), Shadow_minValue) + coloredShadow;

    Fragcolor = vec4(color, 1.0);
}

void main()
{
    PBR_Render(
    GetBaseColor(),
    GetWorldNormalMap(),
    GetAO(),
    GetRoughness(),
    GetMetallic()
    );
}