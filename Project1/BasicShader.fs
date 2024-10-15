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

vec3 GetAO() { return vec3(texture(u_ARM, texCoords).r); }
vec3 GetRoughness() { return vec3(texture(u_ARM, texCoords).g); }
vec3 GetMetalic() { return vec3(texture(u_ARM, texCoords).b); }

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

vec3 GetWorldNormalMap_texture(vec2 Coords)
{
   // TBN ��Ʈ���� ���
    mat3 TBN = mat3(normalize(vertex_Tangent), 
                    normalize(vertex_BitTangent), 
                    normalize(vertex_normal));

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

vec3 CalReflectVector(vec3 normal)
{
    vec3 CameraDir = normalize(WorldPosition - u_CameraPos);

    return CameraDir + 2 * normal * (dot(-CameraDir, normal));
}

vec3 GetReflectedColor(vec3 normal)
{
    return texture(u_enviroment, CalReflectVector(normal)).rgb;
}

void RenderMaterial()
{
    vec3 worldNormalMap = GetWorldNormalMap_texture(texCoords);
    //vec3 worldNormalMap = vertex_normal;

    // ������ ���� ���
    vec3 lightDir = normalize(lightPos - 0); //direction light
    //vec3 lightDir = normalize(lightPos - WorldPosition); //point light

    // ���� ����ũ ���
    float lightMask = GetLightMask(worldNormalMap, lightDir, WorldPosition, lightPos, lightDistance);

    //�ݻ�� �̹���
    vec3 refrected_image = vec3(dot(GetReflectedColor(worldNormalMap), vec3(0.5, 0.5, 0.5)));

    // ���� ���� ���
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

    vec3 DiffultColor = Fragcolor.rgb;
    vec3 MetalicMaskedColor = DiffultColor * GetReflectedColor(GetWorldNormalMap_texture(texCoords)) * GetMetalic();
    vec3 re_MetalicMaskedColor = DiffultColor * (1-GetMetalic());

    Fragcolor = vec4(re_MetalicMaskedColor + MetalicMaskedColor, 1.0);
}