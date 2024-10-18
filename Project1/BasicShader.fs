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

// ������ ���� ���
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
    // �׸��� ��ǥ�� NDC�� ��ȯ (0.0 ~ 1.0 ���̷� ����ȭ)
    vec3 projCoords = LightSpacePos.xyz / LightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    // �׸��� ��ǥ�� ���� ���̸� �׸��� ���� �� ��
    if (projCoords.z > 1.0)
        return 1.0;

    // PCF ���ø� ����
    float shadow = 0.0;
    float bias = 0.005;
    int samples = 5;  // ���� ����
    float texelSize = 1.0 / u_ShadowMapSize;  // �׸��� �� �ػ󵵿� ���� �ؼ� ũ��

    for (int x = -samples; x <= samples; ++x)
    {
        for (int y = -samples; y <= samples; ++y)
        {
            float pcfDepth = texture(u_DepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (2.0 * samples + 1.0) * (2.0 * samples + 1.0);

    return 1.0 - (shadow);  // �׸����� ���� ��ȯ
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
    // �ݻ� ���� ���
    vec3 reflectedDir = normalize(CalReflectVector(normal));

    // �帲�� �����ϱ� ���� �ֺ� ������ ���ø�
    vec3 color = vec3(0.0);
    int sampleCount = 8;  // ���ø��� ����

    // ���� ���⿡�� �ݻ�� ������ ���ø��Ͽ� ���ȭ
    for (int i = 0; i < sampleCount; i++) {
        // ���� ��� ������ ������ ���� (���⼭�� ������ ���ͺ��̼� ���)
        vec3 offset = normalize(vec3(
            fract(sin(float(i)) * 43758.5453), 
            fract(cos(float(i)) * 96123.3542), 
            fract(sin(float(i) * 0.5) * 21391.1325)
        ));

        // �ݻ� ���Ϳ� �������� �߰��ϰ�, blurAmount�� ����
        vec3 blurredDir = normalize(reflectedDir + offset * blurAmount);

        // ����� �������� �ؽ�ó ���ø�
        color += texture(u_enviroment, blurredDir).rgb;
    }

    // ������ ������ ����� ��ȯ (�հ踦 ���� ������ ����)
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