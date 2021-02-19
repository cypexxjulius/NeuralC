#version 440 core

layout(location = 0) in vec4 a_Position;

out vec4 v_color;

void main()
{
    v_color = a_Position;
    gl_Position = a_Position;
}
