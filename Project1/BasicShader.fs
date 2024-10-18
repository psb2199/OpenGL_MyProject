#version 330 core

in vec2             texCoords;
in vec3             WorldPosition;
in vec3             vertex_normal;
in vec3             vertex_Tangent;
in vec3             vertex_BitTangent;

uniform             vec3 u_CameraPos;

uniform samplerCube u_enviroment;

uniform sampler2D   u_BaseColor;
uniform sampler2D   u_NormalMap;
uniform sampler2D   u_Emissive;
uniform sampler2D   u_ARM;

uniform sampler2D   u_DepthMap;
in vec4             LightSpacePos;
uniform float       u_ShadowMapSize;
float               Shadow_minValue = 0.2;

uniform vec3        lightPos;
uniform vec3        lightColor;
uniform float       lightDistance;
uniform bool        u_cast_shadow;



out vec4 Fragcolor;

float       GetAO()            { return texture(u_ARM, texCoords).r; }
float       GetRoughness()     { return texture(u_ARM, texCoords).g; }
float       GetMetallic()      { return texture(u_ARM, texCoords).b; }

vec3        GetBaseColor()     { return texture(u_BaseColor, texCoords).rgb; }
vec3        GetEmissive()      { return texture(u_Emissive, texCoords).rgb; }

// 광원의 방향 계산
vec3 LightDir = normalize(lightPos - 0); //direction light
//vec3 LightDir = normalize(lightPos - WorldPosition); //point light

vec3 CameraDir = normalize(WorldPosition - u_CameraPos);


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
    return CameraDir + 2 * normal * (dot(-CameraDir, normal));
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

float Fresnel(float threshold, vec3 normal)
{
    return pow(1.0 - dot(CameraDir, -normal), threshold);
}

void Render(vec3 BaseColor, vec3 NormalMap, float AO, float Roughness, float Metallic, vec3 Emissive)
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
    float Highlight = pow(dot(NormalMap, LightDir), 64);
    //=======================================================================



    //Metallic===============================================================
    vec3 re_MetallicMaskedColor = resultColor * (1 - Metallic);
    vec3 MetallicMaskedColor = resultColor * reflectedColor * Metallic;
    resultColor = re_MetallicMaskedColor + MetallicMaskedColor;
    //=======================================================================

    

    //Shadow=================================================================
    //resultColor += Highlight;
    resultColor *= max(dot(LightDir, NormalMap) * CalShadowFactor(), Shadow_minValue);
    //=======================================================================
   


    //Emissive===============================================================
    resultColor += Emissive;
    //=======================================================================


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
}