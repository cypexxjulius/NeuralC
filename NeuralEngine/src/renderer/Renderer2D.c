#include "Renderer2D.h"
#include "shader/ShaderLibrary.h"
#include "src/core/error.h"
#include "vertexArray.h"
#include "renderer.h"
#include "src/utils/types.h"

static Shader* TextureShader = NULL;
static VertexArray* QuadVertexArray = NULL; 
static Texture2D* IdentityTexture = NULL;

#define MaxQuads 10000
#define MaxVertices MaxQuads * 4
#define MaxIndices MaxQuads * 6

struct QuadVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float TextureSlot;
    float Tiling;
};

static u64 QuadIndexCount = 0;

static struct QuadVertex* QuadVertexBufferBase = NULL;
static struct QuadVertex* QuadVertexBufferPtr = NULL;

// TextureSlot Array

#define MaxTextureSlots 32
static Texture2D* TextureSlots[MaxTextureSlots];
u32 TextureSlotIndex = 1;
u16 zIndexStackedLayer = 0;



void Renderer2DInit()
{
    QuadVertexArray = NewVertexArray();

    // Quad Vertex Buffer 
    
    VertexBuffer* vertexBuffer = NewVertexBufferEmpty(MaxVertices * sizeof(struct QuadVertex));

    VertexBufferSetLayout(vertexBuffer, 5,
        BufferElement("a_Position", NEURAL_FLOAT, 3),
        BufferElement("a_Color", NEURAL_FLOAT, 4),
        BufferElement("a_TexCoord", NEURAL_FLOAT, 2),
        BufferElement("a_TextureSlot", NEURAL_FLOAT, 1),
        BufferElement("a_TilingFactor", NEURAL_FLOAT, 1)
    );
        
        
    VertexArrayAddVertexBuffer(QuadVertexArray, vertexBuffer);


    // Index Buffer
    QuadVertexBufferBase = Memory.Calloc(MaxVertices, sizeof(struct QuadVertex));

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
    Texture2DBind(TextureSlots[0], 0);

    u32 samplers[MaxTextureSlots];
    for(u32 i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;


    TextureShader = NewShader("TextureShader", "res/shader/TextureShader.glsl");
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

    TextureSlotIndex = 1;
    zIndexStackedLayer = 0;
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

}

void Renderer2DEndScene()
{
    if(QuadIndexCount != 0)
        Renderer2DUploadBatch();
}

extern void Renderer2DDrawQuad(Quad2D initializer)
{

    if(QuadIndexCount + 6 >= MaxIndices || initializer.texture != NULL && TextureSlotIndex == 31)
    {
        Renderer2DUploadBatch();

        // Resetting Buffer Indexes
        QuadIndexCount = 0;
        QuadVertexBufferPtr = QuadVertexBufferBase;

        TextureSlotIndex = 1;
    }
    
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
    float zIndex = -(initializer.zIndex + zIndexStackedLayer) * 0.01f;

    if(initializer.rotation == 0)
        {
        // Dont put this in a for loop it will slow down the rendering process

        // Bottom left
        QuadVertexBufferPtr->Position = v3(position.x, position.y, zIndex);
        QuadVertexBufferPtr->Color = color;
        QuadVertexBufferPtr->TexCoord = v2(0.0f, 0.0f);
        QuadVertexBufferPtr->TextureSlot = TextureID;
        QuadVertexBufferPtr->Tiling = tiling;
        QuadVertexBufferPtr++;

        // Bottom Right
        QuadVertexBufferPtr->Position = v3(position.x + scale.x, position.y, zIndex);
        QuadVertexBufferPtr->Color = color;
        QuadVertexBufferPtr->TexCoord = v2(1.0f, 0.0f);
        QuadVertexBufferPtr->TextureSlot = TextureID;
        QuadVertexBufferPtr->Tiling = tiling;
        QuadVertexBufferPtr++;
        
        // Top Left
        QuadVertexBufferPtr->Position = v3(position.x + scale.x, position.y + scale.y, zIndex);
        QuadVertexBufferPtr->Color = color;
        QuadVertexBufferPtr->TexCoord = v2(1.0f, 1.0f);
        QuadVertexBufferPtr->TextureSlot = TextureID;
        QuadVertexBufferPtr->Tiling = tiling;
        QuadVertexBufferPtr++;
        
        // Top Right
        QuadVertexBufferPtr->Position = v3(position.x, position.y + scale.y, zIndex);
        QuadVertexBufferPtr->Color = color;
        QuadVertexBufferPtr->TexCoord = v2(0.0f, 1.0f);
        QuadVertexBufferPtr->TextureSlot = TextureID;
        QuadVertexBufferPtr->Tiling = tiling;
        QuadVertexBufferPtr++;


        QuadIndexCount += 6;
        return;
    }
    /*
    mat4s transform =   glms_mat4_mul(
                            glms_translate_make(vec3s(position.x, position.y, zIndex)), // Position transform
                            glms_mat4_mul(  
                                glms_scale_make(vec3s(scale.x, scale.y, 1.0f)), // Scale Transform
                                glms_rotate_make(initializer.rotation, vec3s(0.0f, 0.0f, 1.0f)) // Rotation Transform
                            )
                        );

    // Dont put this in a for loop it will slow down the rendering process

    // Bottom left
    QuadVertexBufferPtr->Position = glms_vec4_ transform;
    QuadVertexBufferPtr->Color = color;
    QuadVertexBufferPtr->TexCoord = v2(0.0f, 0.0f);
    QuadVertexBufferPtr->TextureSlot = TextureID;
    QuadVertexBufferPtr->Tiling = tiling;
    QuadVertexBufferPtr++;

    // Bottom Right
    QuadVertexBufferPtr->Position = transform;
    QuadVertexBufferPtr->Color = color;
    QuadVertexBufferPtr->TexCoord = v2(1.0f, 0.0f);
    QuadVertexBufferPtr->TextureSlot = TextureID;
    QuadVertexBufferPtr->Tiling = tiling;
    QuadVertexBufferPtr++;
    
    // Top Left
    QuadVertexBufferPtr->Position = transform;
    QuadVertexBufferPtr->Color = color;
    QuadVertexBufferPtr->TexCoord = v2(1.0f, 1.0f);
    QuadVertexBufferPtr->TextureSlot = TextureID;
    QuadVertexBufferPtr->Tiling = tiling;
    QuadVertexBufferPtr++;
    
    // Top Right
    QuadVertexBufferPtr->Position = transform;
    QuadVertexBufferPtr->Color = color;
    QuadVertexBufferPtr->TexCoord = v2(0.0f, 1.0f);
    QuadVertexBufferPtr->TextureSlot = TextureID;
    QuadVertexBufferPtr->Tiling = tiling;
    QuadVertexBufferPtr++;


    QuadIndexCount += 6;
    */

} 


