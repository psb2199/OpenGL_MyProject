#version 330 core

in   vec2               texCoords;
in   float              randomSeed; // 0.0 ~ 1.0

out  vec4               Fragcolor;

uniform sampler2D       u_BaseColor;
uniform sampler2D       u_Emissive;

uniform float           elapsedTime;
uniform float           lifeTime;
uniform float           fadeOutTime;

uniform float           randomSeedValue;

uniform vec3            particleColor;

uniform int             particle_type;
const   int             P_PopCoin = 0;
const   int             P_Leaf = 1;

vec3 GetHighlight(float mask, int threshold, vec3 color)
{
    vec3 highlight = vec3(pow(mask, threshold));
    highlight *= color;

    return highlight;
}

void PopCoin()
{
    vec3 resultColor;

    vec4 image = texture(u_BaseColor, texCoords);
    vec3 imageColor = image.rgb;
    float alpha = image.a;

    vec3 color;
    color.r = particleColor.r;
    color.g = particleColor.g;
    color.b = particleColor.b + (randomSeed - 0.5) * randomSeedValue;
    resultColor = imageColor * color;

    resultColor += GetHighlight(image.a, 4, vec3(1.0));

    float resultAlpha;
    if(elapsedTime < lifeTime - fadeOutTime)
    {
        resultAlpha = alpha;
    }
    else{
        resultAlpha = alpha - (elapsedTime - (lifeTime - fadeOutTime));
    }

    Fragcolor = vec4(resultColor, resultAlpha);
}

void Leaf()
{
    vec3 resultColor;

    vec4 image = texture(u_BaseColor, texCoords);
    vec3 imageColor = image.rgb;
    float alpha = image.a;

    vec3 color;
    color.r = particleColor.r + particleColor.r * randomSeed * randomSeedValue;
    color.g = particleColor.g + particleColor.g * randomSeed * randomSeedValue;
    color.b = particleColor.b + particleColor.b * randomSeed * randomSeedValue;
    resultColor = imageColor * color;


    resultColor += GetHighlight(image.a, 32, vec3(0.6,1,0));

    float resultAlpha;
    if(elapsedTime < lifeTime - fadeOutTime)
    {
        resultAlpha = alpha;
    }
    else{
        resultAlpha = alpha - (elapsedTime - (lifeTime - fadeOutTime));
    }

    Fragcolor = vec4(resultColor, resultAlpha);
}

void main()
{
    switch( particle_type )
    {
    case P_PopCoin: PopCoin();
        break;

    case P_Leaf: Leaf();
        break;
    }
}