

#type fragment
#version 440 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}
#type vertex
#version 440 core

layout(location = 0) in vec4 a_Position;

uniform mat4 u_viewProj;
uniform mat4 u_Transform;
uniform vec4 u_Color;

out vec4 v_Color;

void main()
{ 
    v_Color = u_Color;
    gl_Position = u_viewProj * u_Transform * a_Position;
}