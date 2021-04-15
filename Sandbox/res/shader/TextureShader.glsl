
#type fragment
#version 440 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_TillingFactor;

void main()
{
    color = texture(u_Texture, v_texCoord * u_TillingFactor) * u_Color;
}

#type vertex

#version 440 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

void main()
{ 
    gl_Position = u_ViewProj * u_Transform * a_Position;
    v_texCoord = texCoord;
}
