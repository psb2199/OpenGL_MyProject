#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 u_CameraPos;

out vec3 TexCoords;

void main()
{
     TexCoords = aPos;
     vec4 pos = projection * view * vec4(aPos + u_CameraPos, 1.0);
     gl_Position = pos.xyww;
}  