#type vertex
#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureID;
layout(location = 4) in float a_Tiling;

out vec4 v_Color;
out vec2 v_texCoord;
out flat float v_TextureID;
out float v_TilingFactor;

uniform mat4 u_ViewProj;

void main()
{ 
    v_Color = a_Color;
    v_texCoord = a_TexCoord;
    v_TextureID = a_TextureID;
    v_TilingFactor = a_Tiling;
    gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}

#type fragment
#version 440 core

layout(location = 0) out vec4 GLcolor;

in vec4 v_Color;
in vec2 v_texCoord;
in flat float v_TextureID;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 color = v_Color;
    switch(int(v_TextureID))
    {
        case 0: color *= texture2D(u_Textures[0], v_texCoord * v_TilingFactor); break;
        case 1: color *= vec4(0.0, 0.0, 0.0, texture2D(u_Textures[1], v_texCoord).r); break;
        case 2: color *= texture2D(u_Textures[2], v_texCoord * v_TilingFactor); break;
        case 3: color *= texture2D(u_Textures[3], v_texCoord * v_TilingFactor); break;
        case 4: color *= texture2D(u_Textures[4], v_texCoord * v_TilingFactor); break;
        case 5: color *= texture2D(u_Textures[5], v_texCoord * v_TilingFactor); break;
        case 6: color *= texture2D(u_Textures[6], v_texCoord * v_TilingFactor); break;
        case 7: color *= texture2D(u_Textures[7], v_texCoord * v_TilingFactor); break;
        case 8: color *= texture2D(u_Textures[8], v_texCoord * v_TilingFactor); break;
        case 9: color *= texture2D(u_Textures[9], v_texCoord * v_TilingFactor); break;
        case 10: color *= texture2D(u_Textures[10], v_texCoord * v_TilingFactor); break;
        case 11: color *= texture2D(u_Textures[11], v_texCoord * v_TilingFactor); break;
        case 12: color *= texture2D(u_Textures[12], v_texCoord * v_TilingFactor); break;
        case 13: color *= texture2D(u_Textures[13], v_texCoord * v_TilingFactor); break;
        case 14: color *= texture2D(u_Textures[14], v_texCoord * v_TilingFactor); break;
        case 15: color *= texture2D(u_Textures[15], v_texCoord * v_TilingFactor); break;
        case 16: color *= texture2D(u_Textures[16], v_texCoord * v_TilingFactor); break;
        case 17: color *= texture2D(u_Textures[17], v_texCoord * v_TilingFactor); break;
        case 18: color *= texture2D(u_Textures[18], v_texCoord * v_TilingFactor); break;
        case 19: color *= texture2D(u_Textures[19], v_texCoord * v_TilingFactor); break;
        case 20: color *= texture2D(u_Textures[20], v_texCoord * v_TilingFactor); break;
        case 21: color *= texture2D(u_Textures[21], v_texCoord * v_TilingFactor); break;
        case 22: color *= texture2D(u_Textures[22], v_texCoord * v_TilingFactor); break;
        case 23: color *= texture2D(u_Textures[23], v_texCoord * v_TilingFactor); break;
        case 24: color *= texture2D(u_Textures[24], v_texCoord * v_TilingFactor); break;
        case 25: color *= texture2D(u_Textures[25], v_texCoord * v_TilingFactor); break;
        case 26: color *= texture2D(u_Textures[26], v_texCoord * v_TilingFactor); break;
        case 27: color *= texture2D(u_Textures[27], v_texCoord * v_TilingFactor); break;
        case 28: color *= texture2D(u_Textures[28], v_texCoord * v_TilingFactor); break;
        case 29: color *= texture2D(u_Textures[29], v_texCoord * v_TilingFactor); break;
        case 30: color *= texture2D(u_Textures[30], v_texCoord * v_TilingFactor); break;
        case 31: color *= texture2D(u_Textures[31], v_texCoord * v_TilingFactor); break;
    }

    GLcolor = color;
}