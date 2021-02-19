#version 440 core

layout(location = 0) out vec4 color;

in vec4 v_color;
uniform float u_Color;

void main()
{
    color = vec4(((v_color * 0.5) + 0.5) + u_Color) ;
}

