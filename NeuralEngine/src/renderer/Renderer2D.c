#include "Renderer2D.h"
#include "shader/ShaderLibrary.h"
#include "src/core/error.h"
#include "vertexArray.h"
#include "renderer.h"
#include "src/utils/types.h"
#include "src/utils/Logger.h"
#include "src/renderer/textures/TextureAtlas.h"
#include "src/renderer/camera/OrthographicCameraController.h"
#include "src/core/Application.h"
#include "src/renderer/font/font.h"


static Shader* TextureShader = NULL;

static VertexArray* QuadVertexArray = NULL; 
static Texture2D* IdentityTexture = NULL;


#define MaxQuads 30000
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

static QuadVertex QuadVertexBufferBase[MaxVertices] = { 0 };
static QuadVertex* QuadVertexBufferPtr = NULL;

static const vec4s QuadVertexPositions[4] = {
    {{  0.0f,  0.0f, 0.0f, 1.0f }},
	{{  1.0f,  0.0f, 0.0f, 1.0f }},
	{{  1.0f,  -1.0f, 0.0f, 1.0f }},
	{{  0.0f,  -1.0f, 0.0f, 1.0f }},
};

// TextureSlot Array
#define MaxTextureSlots 30
int samplers[MaxTextureSlots];


static Texture2D* TextureSlots[MaxTextureSlots];
static u32 TextureSlotIndex = 0;

static float ZINDEX_STEPS = 1.0f / (float)MaxQuads;
static float RendererScenezIndex = 0; 

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

    
    IdentityTexture = NewTexture2DEmpty(1, 1, Image_TypeRGBA);

    // Set Identity (white) Image for pure Color Quads
    uint32_t WhiteTextureData = 0xffffffff;
    Texture2DSetData(IdentityTexture, &WhiteTextureData, sizeof(uint32_t));


    // Setup sampler array
    for(u32 i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;

    TextureShader   = NewShaderFromFile(String("TextureShader"), "res/shader/TextureShader.glsl");
    
    ShaderBind(TextureShader);
    ShaderSetIntArray(TextureShader, "u_Textures", samplers, MaxTextureSlots);
    TextureShader->flags |= SHADER_FLAGS_imageArraySet;
}

void Renderer2DShutdown()
{
    DeleteVertexArray(QuadVertexArray);
    DeleteShader(TextureShader);
    DeleteTexture2D(IdentityTexture);
}

void Renderer2DBeginScene(Camera* camera, Shader* shader, Font* font)
{
    if(shader == NULL)
        shader = TextureShader; 
    
    FontBind(font, 1);
    
    ShaderBind(shader);
    
    if(!(shader->flags & SHADER_FLAGS_imageArraySet))
       ShaderSetIntArray(TextureShader, "u_Textures", samplers, MaxTextureSlots); 

    ShaderSetMat4(
        shader, 
        "u_ViewProj", 
        ((camera != NULL) ? OrthographicCameraGetViewProjMat(camera).raw : GLMS_MAT4_IDENTITY.raw)
    );

    QuadIndexCount = 0;
    QuadVertexBufferPtr = QuadVertexBufferBase;

    TextureSlotIndex = 0;
}

static inline void Renderer2DUploadBatch()
{
    // Bindung all Textures
    Texture2DBind(IdentityTexture, 0);

    for(u32 i = 0; i < TextureSlotIndex; i++)
        Texture2DBind(TextureSlots[i], i + 2);

    VertexBufferSetData(
        VertexArrayGetVertexBuffer(QuadVertexArray, 0), // VertexBuffer 
        QuadVertexBufferBase,   // VertexBuffer buffer start
        (u32)((u8 *)QuadVertexBufferPtr - (u8 *)QuadVertexBufferBase) // Length of the buffer
    );

    RendererDrawIndexed(QuadVertexArray, (u32)QuadIndexCount);

    // Resetting Buffer Indexes
    QuadIndexCount = 0;
    QuadVertexBufferPtr = QuadVertexBufferBase;

    TextureSlotIndex = 0;

}

void Renderer2DEndScene()
{
    if(QuadIndexCount != 0)
        Renderer2DUploadBatch();
}

static inline v3 Mat4MulVec4(mat4s matrix, vec4s vec);

void Renderer2DDrawQuad(Quad2D* initializer)
{

    if((QuadIndexCount + 6 >= MaxIndices) || (initializer->texture != NULL && TextureSlotIndex == 31))
        Renderer2DUploadBatch();

    // Set position

    // Set color
    v4 color = initializer->color;

    // Set Image if defined
    float TextureID = 0;
    if(initializer->texture != NULL)
    {
        for(u32 i = 0; i < TextureSlotIndex; i++)
        {
            if(TextureSlots[i]->id == initializer->texture->id)
            {
                TextureID = (float)i + 2;
                goto out;
            }
        }

        TextureID = (float)TextureSlotIndex + 2;
        TextureSlots[TextureSlotIndex++] = initializer->texture;
    }
    out:;

    float width = initializer->width;
    float height = initializer->height;

    v2 position = V2(initializer->position.x, initializer->position.y);



    float tiling = initializer->tiling ? initializer->tiling : 1.0f;
    float zIndex = RendererScenezIndex + initializer->zIndex;
    RendererScenezIndex += ZINDEX_STEPS;
    
    mat4s transform =  glms_mat4_mul(
        glms_translate_make(vec3s(position.x, position.y, zIndex)), // Position transform
        glms_mat4_mul(  
            glms_rotate_make(glm_rad(initializer->rotation), vec3s(0.0f, 0.0f, 1.0f)), // Rotation Transform
            glms_scale_make(vec3s(width, height, 1.0f)) // Scale Transform
        )
    );

    Renderer2DPushVertices( (v3 [4]) {
        Mat4MulVec4(transform, QuadVertexPositions[0]),
        Mat4MulVec4(transform, QuadVertexPositions[1]),
        Mat4MulVec4(transform, QuadVertexPositions[2]),
        Mat4MulVec4(transform, QuadVertexPositions[3])
    }, color, (v2 [4]) {
        V2(0, 0),
        V2(1, 0),
        V2(1, 1),
        V2(0, 1)
    }, TextureID, tiling);    
} 

void Renderer2DPushVertices(v3 ipositions[4], v4 icolor, v2 itextureCoords[4], float iTextureID, float itiling) {                                                                         
    
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

v3 Mat4MulVec4(mat4s matrix, vec4s vec)
{
    vec3s temp = glms_mat4_mulv3(matrix, vec3s(vec.x, vec.y, vec.z), vec.w);
    return V3(temp.x, temp.y, temp.z);
}

void Renderer2DEndSceneCallback()
{
}

void Renderer2DStartSceneCallback()
{
    RendererScenezIndex = 0;
}
