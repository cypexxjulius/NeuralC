#version 440 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;
uniform mat4 u_viewProj;
uniform mat4 u_Transform;

void main()
{ 
    gl_Position = u_viewProj * u_Transform * a_Position;
    v_texCoord = texCoord;
}