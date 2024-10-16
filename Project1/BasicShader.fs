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
float       GetMetalic()       { return texture(u_ARM, texCoords).b; }

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

float GetLightMask(vec3 normal)
{
    // ������ ���� ���
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

vec3 GetReflectedColor(vec3 normal)
{
    return texture(u_enviroment, CalReflectVector(normal)).rgb;
}

void Render(vec3 BaseColor, vec3 NormalMap, float AO, float Roughness, float Metalic, vec3 Emissive)
{   
    vec3 resultColor;


    resultColor = BaseColor;
    resultColor *= GetLightMask(NormalMap);

    Fragcolor = vec4(resultColor + Emissive, 1.0);
}

void main()
{
    Render(
    GetBaseColor(),
    GetWorldNormalMap(),
    GetAO(),
    GetRoughness(),
    GetMetalic(),
    GetEmissive()
    );

    //vec3 DiffultColor = Fragcolor.rgb;
    //vec3 MetalicMaskedColor = DiffultColor * GetReflectedColor(GetWorldNormalMap()) * GetMetalic();
    //vec3 re_MetalicMaskedColor = DiffultColor * (1-GetMetalic());

    //Fragcolor = vec4(re_MetalicMaskedColor + MetalicMaskedColor + GetEmissive(), 1.0);
}