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
static CameraController* ViewPortCamera = NULL;
static Font* FontTexture;


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
    { -0.5f, -0.5f, 0.0f, 1.0f },
	{  0.5f, -0.5f, 0.0f, 1.0f },
	{  0.5f,  0.5f, 0.0f, 1.0f },
	{ -0.5f,  0.5f, 0.0f, 1.0f },
};

// TextureSlot Array

#define MaxTextureSlots 30
static Texture2D* TextureSlots[MaxTextureSlots];
static u32 TextureSlotIndex = 0;

static double ZINDEX_STEPS = 1.0f / (float)MaxQuads;
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

    // Setup text texture atlas
    FontTexture = NewFontTexture("res/fonts/Roboto-Regular.ttf");


    // Setup sampler array
    u32 samplers[MaxTextureSlots];
    for(u32 i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;


    // Setup ViewPortCamera
    ViewPortCamera = NewOrthographicCameraController(CameraNoControls);

    TextureShader   = NewShaderFromFile(String("TextureShader"), "res/shader/TextureShader.glsl");
    
    ShaderBind(TextureShader);
    ShaderSetIntArray(TextureShader, "u_Textures", samplers, MaxTextureSlots);

}

void Renderer2DShutdown()
{
    DeleteVertexArray(QuadVertexArray);
    DeleteShader(TextureShader);
    DeleteTexture2D(IdentityTexture);
    DeleteFont(FontTexture);
}


void Renderer2DBeginScene(Camera* camera)
{
    
    ShaderSetMat4(TextureShader, "u_ViewProj", ((camera != NULL) ? OrthographicCameraGetViewProjMat(camera).raw : OrthographicCameraGetViewProjMat(ViewPortCamera->camera).raw));

    QuadIndexCount = 0;
    QuadVertexBufferPtr = QuadVertexBufferBase;

    TextureSlotIndex = 0;
}

static inline void Renderer2DUploadBatch()
{
    // Bindung all Textures
    Texture2DBind(IdentityTexture, 0);
    FontBind(FontTexture, 1);

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

static inline void PushVertices(v3 ipositions[4], v4 icolor, v2 itextureCoords[4], float iTextureID, float itiling);

static void Renderer2DRenderText(char* string, float scale, v3 color, v2 position, float zIndex, float maxWidth)
{
    u16 Strlen = (u16)strlen(string);
    float lineLength = 0;

    for(u16 i = 0; i < Strlen; i++)
    {
        if(string[i] == '\n')
        {
            position.y -= 0.5f * scale;
            lineLength = 0;
            continue;
        }


        v2 textureCoords[4];
        v2 Size;
        FontGetCharVertices(FontTexture, string[i], textureCoords, &Size);

        if(Size.width * scale > maxWidth)
            return;

        float posx = lineLength * scale + position.x;
        float posy = position.y - scale;


        PushVertices((v3 [4]){
            v3(posx,                            posy, zIndex),
            v3(posx + Size.width * scale    , posy, zIndex),
            v3(posx + Size.width * scale    , posy + Size.height * scale, zIndex),
            v3(posx,                          posy + Size.height * scale, zIndex),
            }, 
            v4(color.x, color.y, color.z, 1.0f), 
            textureCoords, 
            1, 
            1.0f
        );

        lineLength += Size.width;

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
                TextureID = (float)i + 2;
        
                goto out;TextureID += 2;
            }
        }

        TextureID = (float)TextureSlotIndex;
        TextureSlots[TextureSlotIndex++] = initializer.texture;
        TextureID += 2;
    }
    out:

    float width = (initializer.width ? initializer.width : 1.0f)   / 1000.0f;
    float height = (initializer.height ? initializer.height : 1.0f) / 1000.0f;


    float tiling = initializer.tiling ? initializer.tiling : 1.0f;
    float zIndex = RendererScenezIndex + initializer.zIndex;
    RendererScenezIndex += (float)ZINDEX_STEPS;

    if(initializer.rotation == 0)
    {
        PushVertices( (v3 [4]) {
            v3(position.x,          position.y, zIndex),
            v3(position.x + width,  position.y, zIndex),
            v3(position.x + width,  position.y + height, zIndex),
            v3(position.x,          position.y + height, zIndex)
        }, color, (v2 [4]) {
            V2(0, 0),
            V2(1, 0),
            V2(1, 1),
            V2(0, 1)
        }, TextureID, tiling);
    } else {

    
        mat4s transform =   glms_mat4_mul(
                                glms_translate_make(vec3s(position.x, position.y, zIndex)), // Position transform
                                glms_mat4_mul(  
                                    glms_scale_make(vec3s(width, height, 1.0f)), // Scale Transform
                                    glms_rotate_make(initializer.rotation, vec3s(0.0f, 0.0f, 1.0f)) // Rotation Transform
                                )
                            );

        PushVertices( (v3 []) {
            Mat4MulVec4(transform, QuadVertexPositions[0]),
            Mat4MulVec4(transform, QuadVertexPositions[1]),
            Mat4MulVec4(transform, QuadVertexPositions[2]),
            Mat4MulVec4(transform, QuadVertexPositions[3])
        }, color, (v2 []) {
            V2(0, 0),
            V2(1, 0),
            V2(1, 1),
            V2(0, 1)
        }, TextureID, tiling);    
        
    }


    if(initializer.text != NULL && initializer.text->string != NULL)
    {
        v3 textcolor = v3(0.0f, 0.0f, 0.0f);

        if( initializer.text->color.x != 0 ||
            initializer.text->color.y != 0 ||
            initializer.text->color.z != 0)
        {
            textcolor = initializer.text->color;
        }
        
        Renderer2DRenderText(
            initializer.text->string,
            (initializer.text->fontSize ? initializer.text->fontSize : 1.0f),
            textcolor,
            V2(position.x, position.y + height),
            zIndex + 0.01f,
            width
        );
    }

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

v3 Mat4MulVec4(mat4s matrix, vec4s vec)
{
    vec3s temp = glms_mat4_mulv3(matrix, vec3s(vec.x, vec.y, vec.z), vec.w);
    return v3(temp.x, temp.y, temp.z);
};

void Renderer2DEndSceneCallback()
{
}

void Renderer2DStartSceneCallback()
{
    RendererScenezIndex = 0;
}

