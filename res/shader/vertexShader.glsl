#version 440 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_viewProj;

void main()
{
    gl_Position = u_viewProj * vec4(a_Position, 1.0f);
}
