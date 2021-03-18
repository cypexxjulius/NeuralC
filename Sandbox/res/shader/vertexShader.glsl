#version 440 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;
uniform mat4 u_viewProj;

void main()
{ 
    gl_Position = u_viewProj * a_Position;
    v_texCoord = texCoord;
}