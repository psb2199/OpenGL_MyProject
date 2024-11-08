#version 330 core

in   vec2               texCoords;
in   float              fadeOut_time;

out  vec4               Fragcolor;

uniform sampler2D       u_BaseColor;
uniform sampler2D       u_Emissive;

uniform float           elapsedTime;

void main()
{
    vec3 resultColor;
    float resultAlpha;

    vec4 image = texture(u_BaseColor, texCoords);
    vec3 imageColor = image.rgb;
    float alpha = image.a;

    vec3 color = vec3(1.0, 1.0, fadeOut_time);
    resultColor = imageColor * color;

    float highLight = pow(image.a, 4);
    resultColor += vec3(highLight);

    Fragcolor = vec4(resultColor, alpha);
}