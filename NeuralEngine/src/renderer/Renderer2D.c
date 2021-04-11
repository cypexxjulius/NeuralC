#include "Renderer2D.h"
#include "shader/ShaderLibrary.h"
#include "src/core/error.h"
#include "vertexArray.h"
#include "renderer.h"

static ShaderLibrary* Shaders = NULL;
static VertexArray* QuadVertexArray = NULL; 
static Texture2D* WhiteTexture = NULL;

void Renderer2DInit()
{
    QuadVertexArray = NewVertexArray();

    // Quad Vertex Buffer 
        {
        float SquareVertices[] = 
        {
            -0.5, -0.5, 0.0f, 0.0f, 0.0f,
            0.5, -0.5,  0.0f, 1.0f, 0.0f,
            0.5,  0.5,  0.0f, 1.0f, 1.0f,
            -0.5,  0.5, 0.0f, 0.0f, 1.0f
        };

        VertexBuffer* vertexBuffer = NewVertexBuffer(SquareVertices, sizeof(SquareVertices));

        VertexBufferSetLayout(vertexBuffer, 2,
            BufferElement(NEURAL_FLOAT, 3),
            BufferElement(NEURAL_FLOAT, 2)
        );
        
        VertexArrayAddVertexBuffer(QuadVertexArray, vertexBuffer);
    }


    // Index Buffer
    {
        u32 SquareIndices[6] = {
            0, 1, 2, 
            2, 3, 0
        };

        IndexBuffer* ib = NewIndexBuffer(SquareIndices, sizeof(SquareIndices) / sizeof(u32));


        VertexArraySetIndexBuffer(QuadVertexArray, ib);
    }

    WhiteTexture = NewTexture2DEmpty(1, 1);
        
    uint32_t WhiteTextureData = 0xffffffff;
    Texture2DSetData(WhiteTexture, &WhiteTextureData, sizeof(uint32_t));
    Texture2DBind(WhiteTexture, 1);


    Shaders = NewShaderLibrary(3);
    Shader* TextureShader = ShaderLibraryLoadShader(Shaders, "TextureShader", "res/shader/TextureShader.glsl");

    Assert(TextureShader == NULL, "Failed to create Shader");
}

void Renderer2DShutdown()
{
    DeleteVertexArray(QuadVertexArray);
    DeleteShaderLibrary(Shaders);
    DeleteTexture2D(WhiteTexture);
}


void Renderer2DBeginScene(Camera* camera)
{
    Shader *TextureShader = ShaderLibraryGetShader(Shaders, "TextureShader");

    mat4s viewProjmat = orthographicCameraGetViewPosMat(camera);

    ShaderBind(TextureShader);
    ShaderSetMat4(TextureShader, "u_ViewProj", viewProjmat.raw);
    ShaderSetInt(TextureShader, "u_Texture", 0);
}

void Renderer2DDrawColoredQuad(v3 position, v2 scale, v4 color)
{
    Shader* TextureShader = ShaderLibraryGetShader(Shaders, "TextureShader");

    // Upload Color to Shader and neutral Image
    ShaderSetInt(TextureShader, "u_Texture", 1);
    ShaderSetFloat4(TextureShader, "u_Color", color);   
    
    // Calculation Transform Matrix

    // Scale Transform
    mat4s scaleTransform = GLMS_MAT4_IDENTITY_INIT; 
    scaleTransform = glms_scale(scaleTransform, vec3s(scale.x, scale.y, 1.0f ));
    
    // Position Translation
    mat4s transform_temp = glms_translate_make(vec3s(position.x, position.y, position.z));

    // Multiplicate those 2 matrices together
    mat4s resultTransform = glms_mat4_mul(transform_temp, scaleTransform);
    ShaderSetMat4(TextureShader, "u_Transform", resultTransform.raw);
    

    VertexArrayBind(QuadVertexArray);
    RendererDrawIndexed(QuadVertexArray);
}


void Renderer2DDrawTexturedQuad(v3 position, v2 scale, Texture2D* texture)
{
    Shader* TextureShader = ShaderLibraryGetShader(Shaders, "TextureShader");
    

    // Setting Texture and Color for the TextureShader
    ShaderSetInt(TextureShader, "u_Texture", 0);
    ShaderSetFloat4(TextureShader, "u_Color", v4(1.0f, 1.0f, 1.0f, 1.0f));   
    


    // Calculation Transform Matrix 
    // Scale Transform
    mat4s scaleTransform = GLMS_MAT4_IDENTITY_INIT; 
    scaleTransform = glms_scale(scaleTransform, vec3s(scale.x, scale.y, 1.0f ));
    
    // Position Translation
    mat4s transform_temp = glms_translate_make(vec3s(position.x, position.y, position.z));

    // Multiplicate those 2 matrices together
    mat4s resultTransform = glms_mat4_mul(transform_temp, scaleTransform);
    ShaderSetMat4(TextureShader, "u_Transform", resultTransform.raw);
    
    Texture2DBind(texture, 0);

    VertexArrayBind(QuadVertexArray);
    RendererDrawIndexed(QuadVertexArray);
}

void Renderer2DEndScene()
{

}
