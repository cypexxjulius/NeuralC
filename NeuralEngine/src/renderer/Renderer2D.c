#include "Renderer2D.h"
#include "shader/ShaderLibrary.h"
#include "src/core/error.h"
#include "vertexArray.h"
#include "renderer.h"
#include "src/utils/types.h"
#include "src/utils/Logger.h"

static Shader* TextureShader = NULL;
static VertexArray* QuadVertexArray = NULL; 
static Texture2D* IdentityTexture = NULL;
static Texture2D* TextTextureAtlas = NULL;

#define MaxQuads 20000
#define MaxVertices MaxQuads * 4
#define MaxIndices MaxQuads * 6

typedef struct QuadVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float TextureSlot;
    float Tiling;
} QuadVertex;

static u64 QuadIndexCount = 0;

static QuadVertex QuadVertexBufferBase[MaxVertices * sizeof(QuadVertex)] = { 0 };
static QuadVertex* QuadVertexBufferPtr = NULL;

static const vec4s QuadVertexPositions[4] = {
    { -0.5f, -0.5f, 0.0f, 1.0f },
	{  0.5f, -0.5f, 0.0f, 1.0f },
	{  0.5f,  0.5f, 0.0f, 1.0f },
	{ -0.5f,  0.5f, 0.0f, 1.0f },
};

// TextureSlot Array

#define MaxTextureSlots 32
static Texture2D* TextureSlots[MaxTextureSlots];
static u32 TextureSlotIndex = 1;



void Renderer2DInit()
{
    QuadVertexArray = NewVertexArray();

    // Quad Vertex Buffer 
    
    VertexBuffer* vertexBuffer = NewVertexBufferEmpty(MaxVertices * sizeof(QuadVertex));

    VertexBufferSetLayout(vertexBuffer, 5, (VertexBufferElement []){
        BufferElement("a_Position", NEURAL_FLOAT, 3),
        BufferElement("a_Color", NEURAL_FLOAT, 4),
        BufferElement("a_TexCoord", NEURAL_FLOAT, 2),
        BufferElement("a_TextureSlot", NEURAL_FLOAT, 1),
        BufferElement("a_TilingFactor", NEURAL_FLOAT, 1)
    });
        
        
    VertexArrayAddVertexBuffer(QuadVertexArray, vertexBuffer);


    // Index Buffer

    u32 QuadIndices[MaxIndices];

    u32 offset = 0;
    for(u32 i = 0; i < MaxIndices; i += 6)
    {
            QuadIndices[i + 0] = offset + 0;
            QuadIndices[i + 1] = offset + 1;
            QuadIndices[i + 2] = offset + 2;

            QuadIndices[i + 3] = offset + 2;
            QuadIndices[i + 4] = offset + 3;
            QuadIndices[i + 5] = offset + 0;

            offset += 4;
    }

    IndexBuffer* quadIndexBuffer = NewIndexBuffer(QuadIndices, MaxIndices);
    VertexArraySetIndexBuffer(QuadVertexArray, quadIndexBuffer);

    
    IdentityTexture = NewTexture2DEmpty(1, 1);

    // Set Identity (white) Image for pure Color Quads
    uint32_t WhiteTextureData = 0xffffffff;
    Texture2DSetData(IdentityTexture, &WhiteTextureData, sizeof(uint32_t));
    TextureSlots[0] = IdentityTexture;

    // Setup text texture atlas
    TextTextureAtlas = NewTexture2D("res/textures/CharSet2.png");
    TextureSlots[1] = TextTextureAtlas;

    u32 samplers[MaxTextureSlots];
    for(u32 i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;


    TextureShader = NewShaderFromFile(String("TextureShader"), "res/shader/TextureShader.glsl");
    Assert(TextureShader == NULL, "Failed to create Shader");
    
    ShaderBind(TextureShader);
    ShaderSetIntArray(TextureShader, "u_Textures", samplers, MaxTextureSlots);

}

void Renderer2DShutdown()
{
    DeleteVertexArray(QuadVertexArray);
    DeleteShader(TextureShader);
    DeleteTexture2D(IdentityTexture);
}


void Renderer2DBeginScene(Camera* camera)
{
    
    ShaderSetMat4(TextureShader, "u_ViewProj", camera ? orthographicCameraGetViewProjMat(camera).raw : GLMS_MAT4_IDENTITY.raw );

    QuadIndexCount = 0;
    QuadVertexBufferPtr = QuadVertexBufferBase;

    TextureSlotIndex = 2;
}

static inline void Renderer2DUploadBatch()
{
    // Bindung all Textures
    for(u32 i = 0; i < TextureSlotIndex; i++)
        Texture2DBind(TextureSlots[i], i);

    VertexBufferSetData(
        VertexArrayGetVertexBuffer(QuadVertexArray, 0), // VertexBuffer 
        QuadVertexBufferBase,   // VertexBuffer buffer start
        (u32)((u8 *)QuadVertexBufferPtr - (u8 *)QuadVertexBufferBase) // Length of the buffer
    );

    RendererDrawIndexed(QuadVertexArray, (u32)QuadIndexCount);

    // Resetting Buffer Indexes
    QuadIndexCount = 0;
    QuadVertexBufferPtr = QuadVertexBufferBase;

    TextureSlotIndex = 2;

}

void Renderer2DEndScene()
{
    if(QuadIndexCount != 0)
        Renderer2DUploadBatch();
}

static inline v3 Mat4MulVec4(mat4s matrix, vec4s vec)
{
    vec3s temp = glms_mat4_mulv3(matrix, vec3s(vec.x, vec.y, vec.z), vec.w);
    return v3(temp.x, temp.y, temp.z);
};

static inline void PushVertices(v3 ipositions[4], v4 icolor, v2 itextureCoords[4], float iTextureID, float itiling);

void Renderer2DRenderTest(char* string)
{
    u16 Strlen = (u16)strlen(string);
    for(u16 i = 0; i < Strlen; i++)
    {
        static const float scale = 0.1f;

        v4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v2 position = { i * scale,  0.0 };
        static const float zIndex = 0;
        static const float tiling = 1;

        static const float characterWidth = 16;

        float TextureID = 1;
        static const float charSetWidth = 256;

        static const float unit = 16.0f / 256.0f;

        float column =  (string[i] % 16);
        float row =  ((string[i] - column) / 16.0f) - 1.0f; 
        column--;

        v2 textureCoords[4] = {
            v2(unit * (column - 0.0f),   1.0f - unit * (row + 1.0f)),
            v2(unit * (column + 1.0f),   1.0f - unit * (row + 1.0f)),
            v2(unit * (column + 1.0f),   1.0f - unit * (row - 0.0f)),
            v2(unit * (column - 0.0f),   1.0f - unit * (row - 0.0f)),
        };

        PushVertices((v3 [4]){
            v3(position.x, position.y, zIndex),
            v3(position.x + scale, position.y, zIndex),
            v3(position.x + scale, position.y + scale, zIndex),
            v3(position.x, position.y + scale, zIndex),
        }, color, textureCoords, TextureID, tiling);
    }
}

void Renderer2DDrawQuad(Quad2D initializer)
{

    if(QuadIndexCount + 6 >= MaxIndices || initializer.texture != NULL && TextureSlotIndex == 31)
        Renderer2DUploadBatch();

    // Set position
    v2 position = initializer.position;

    // Check color
    v4 color = (initializer.color.x != 0 || initializer.color.y != 0 || initializer.color.z != 0 || initializer.color.w != 0) // Check for a non defined Color
         ? color = initializer.color :  v4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Set Image if defined
    float TextureID = 0;
    if(initializer.texture != NULL)
    {
        for(u32 i = 0; i < TextureSlotIndex; i++)
        {
            if(TextureSlots[i]->id == initializer.texture->id)
            {
                TextureID = (float)i;
                goto out;
            }
        }

        TextureID = (float)TextureSlotIndex;
        TextureSlots[TextureSlotIndex++] = initializer.texture;
    }
    out:

    v2 scale = {
        .x = initializer.scale.x ? initializer.scale.x : 1.0f,
        .y = initializer.scale.y ? initializer.scale.y : 1.0f
    };

    float tiling = initializer.tiling ? initializer.tiling : 1.0f;
    float zIndex = -(initializer.zIndex) * 0.01f;


    if(initializer.rotation == 0)
    {
        PushVertices( (v3 [4]) {
            v3(position.x, position.y, zIndex),
            v3(position.x + scale.x, position.y, zIndex),
            v3(position.x + scale.x, position.y + scale.y, zIndex),
            v3(position.x, position.y + scale.y, zIndex)
        }, color, (v2 [4]) {
            v2(0, 0),
            v2(1, 0),
            v2(1, 1),
            v2(0, 1)
        }, TextureID, tiling);
        return;
    }

    
    mat4s transform =   glms_mat4_mul(
                            glms_translate_make(vec3s(position.x, position.y, zIndex)), // Position transform
                            glms_mat4_mul(  
                                glms_scale_make(vec3s(scale.x, scale.y, 1.0f)), // Scale Transform
                                glms_rotate_make(initializer.rotation, vec3s(0.0f, 0.0f, 1.0f)) // Rotation Transform
                            )
                        );

    PushVertices( (v3 []) {
        Mat4MulVec4(transform, QuadVertexPositions[0]),
        Mat4MulVec4(transform, QuadVertexPositions[1]),
        Mat4MulVec4(transform, QuadVertexPositions[2]),
        Mat4MulVec4(transform, QuadVertexPositions[3])
    }, color, (v2 []) {
        v2(0, 0),
        v2(1, 0),
        v2(1, 1),
        v2(0, 1)
    }, TextureID, tiling);    

    return;    
} 

void PushVertices(v3 ipositions[4], v4 icolor, v2 itextureCoords[4], float iTextureID, float itiling) {                                                                         
    
    // Dont put this in a for loop it will slow down the rendering process
    // Bottom left 
    QuadVertexBufferPtr->Position = ipositions[0];                               
    QuadVertexBufferPtr->Color = icolor;                                         
    QuadVertexBufferPtr->TexCoord = itextureCoords[0];                           
    QuadVertexBufferPtr->TextureSlot = iTextureID;        
    QuadVertexBufferPtr->Tiling = itiling;                         
    QuadVertexBufferPtr++;                                                     

    // Bottom right                                                              
    QuadVertexBufferPtr->Tiling = itiling;                                       
    QuadVertexBufferPtr->Position = ipositions[1];                               
    QuadVertexBufferPtr->Color = icolor;                                         
    QuadVertexBufferPtr->TexCoord = itextureCoords[1];                           
    QuadVertexBufferPtr->TextureSlot = iTextureID;                               
    QuadVertexBufferPtr->Tiling = itiling;                                       
    QuadVertexBufferPtr++;                                                       

    // Top left                                                                                 
    QuadVertexBufferPtr->Position = ipositions[2];                               
    QuadVertexBufferPtr->Color = icolor;                                         
    QuadVertexBufferPtr->TexCoord = itextureCoords[2];                           
    QuadVertexBufferPtr->TextureSlot = iTextureID;                               
    QuadVertexBufferPtr->Tiling = itiling;                                       
    QuadVertexBufferPtr++;                                                       

    // Top right                                                                  
    QuadVertexBufferPtr->Position = ipositions[3];                               
    QuadVertexBufferPtr->Color = icolor;                                         
    QuadVertexBufferPtr->TexCoord = itextureCoords[3];                           
    QuadVertexBufferPtr->TextureSlot = iTextureID;                               
    QuadVertexBufferPtr->Tiling = itiling;                                       
    QuadVertexBufferPtr++;                                                         
                                                                                 
    QuadIndexCount += 6;                                                                                                                
}
