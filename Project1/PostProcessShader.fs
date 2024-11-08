#version 330 core

in vec2 texCoords;

uniform sampler2D u_Bloom;
uniform sampler2D u_PostProcess;

uniform vec2 ViewSize;

out vec4 Fragcolor;

vec3 BlurColor(sampler2D image, vec2 coords)
{   
    
    float tex_offset = 1.0 / 700;
    float highligheValue = 8.0;
    vec3 result = vec3(0.0); 

    
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * tex_offset; 
            result += texture(image, coords + offset).rgb;


            if(result.r > highligheValue || result.g > highligheValue || result.b > highligheValue)
            {
                result += vec3(1.0);
            }
        }
    }

    // 9°³ÀÇ ÇÈ¼¿ °ªÀ» Æò±Õ
    result /= 25.0;

    //result = pow(result, vec3(64));

    return result;
}

vec3 AntiAliasing(vec2 fragCoord)
{
    vec2 texelSize = 1.0 / ViewSize;

    vec3 rgbNW = texture(u_PostProcess, fragCoord + texelSize * vec2(-1.0, -1.0)).rgb; 
    vec3 rgbNE = texture(u_PostProcess, fragCoord + texelSize * vec2(1.0, -1.0)).rgb;  
    vec3 rgbSW = texture(u_PostProcess, fragCoord + texelSize * vec2(-1.0, 1.0)).rgb;  
    vec3 rgbSE = texture(u_PostProcess, fragCoord + texelSize * vec2(1.0, 1.0)).rgb;   
    vec3 rgbM  = texture(u_PostProcess, fragCoord).rgb;                                

    float lumaNW = dot(rgbNW, vec3(0.299, 0.587, 0.114));
    float lumaNE = dot(rgbNE, vec3(0.299, 0.587, 0.114));
    float lumaSW = dot(rgbSW, vec3(0.299, 0.587, 0.114));
    float lumaSE = dot(rgbSE, vec3(0.299, 0.587, 0.114));
    float lumaM  = dot(rgbM,  vec3(0.299, 0.587, 0.114));

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    if (lumaMax - lumaMin < 0.1) {
        return rgbM; 
    }

    vec3 avgColor = (rgbNW + rgbNE + rgbSW + rgbSE + rgbM) / 5.0;

    return avgColor;
}

void main()
{
    vec3 bloom = BlurColor(u_Bloom, texCoords);
    vec3 pp =  texture(u_PostProcess, texCoords).rgb;

    vec3 resultColor = pp + bloom;
    //vec3 resultColor = pp;
    
    //resultColor = AntiAliasing(texCoords);

    Fragcolor = vec4(resultColor, 1.0);  
}