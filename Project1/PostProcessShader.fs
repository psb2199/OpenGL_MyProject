#version 330 core

in vec2 texCoords;

uniform sampler2D u_Bloom;
uniform sampler2D u_PostProcess;

uniform vec3 u_CameraPos;

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


void main()
{
    vec3 bloom = BlurColor(u_Bloom, texCoords);
    vec3 pp =  texture(u_PostProcess, texCoords).rgb;

    vec3 resultColor = pp + bloom;
    
    Fragcolor = vec4(resultColor, 1.0);  
}