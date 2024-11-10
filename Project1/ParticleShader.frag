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

    float highLight = pow(image.a, 4);
    resultColor += vec3(highLight);

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
    }
}