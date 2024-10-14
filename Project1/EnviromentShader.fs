#version 330 core

in vec3 TexCoords;



out vec4 Fragcolor;


uniform samplerCube u_enviroment;

void main()
{    
    //Fragcolor = texture(u_enviroment, TexCoords);
    //Fragcolor = vec4(fract(TexCoords), 1);
    Fragcolor = vec4(1.0);
}