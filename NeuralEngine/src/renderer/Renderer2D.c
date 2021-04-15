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

extern void Renderer2DDrawQuad(Quad2D initializer)
{
    Shader* shader = ShaderLibraryGetShader(Shaders, "TextureShader");
    ShaderBind(shader);
    // Color and Texture

    // Upload Image if defined
    if(initializer.texture == NULL)
        ShaderSetInt(shader, "u_Texture", 1);
    else 
    {
        Texture2DBind(initializer.texture, 0);
        ShaderSetInt(shader, "u_Texture", 0);
    }

    // Upload tilling Factor
    ShaderSetFloat(shader, "u_TillingFactor", initializer.tilling != 0 ? initializer.tilling : 1.0f);

    // Upload Color
    if( initializer.color.x == 0 && 
        initializer.color.y == 0 && 
        initializer.color.z == 0 && 
        initializer.color.w == 0) // Check for a non defined Color
        ShaderSetFloat4(shader, "u_Color", (v4){1.0f, 1.0f, 1.0f, 1.0f});
    else    
        ShaderSetFloat4(shader, "u_Color", initializer.color);


    // Calculation Transform Matrix

    // Scale Transform
    mat4s scaleTransform = GLMS_MAT4_IDENTITY_INIT; 
    
    if(initializer.scale.x == 0 && initializer.scale.y == 0)
    {
        scaleTransform = glms_scale(scaleTransform, vec3s(1.0f, 1.0f, 1.0f ));
    } else 
    {
        scaleTransform = glms_scale(scaleTransform, vec3s(initializer.scale.x, initializer.scale.y, 1.0f ));
    }

    // Position Translation

    mat4s transform_temp = glms_translate_make(vec3s(initializer.position.x, initializer.position.y, initializer.position.z));

    if(initializer.rotation != 0)
    {
        mat4s rotation_matrix = glms_rotate_make(initializer.rotation, (vec3s){ 0.0f, 0.0f, 1.0f});
        transform_temp = glms_mat4_mul(transform_temp, rotation_matrix);
    }

    // Multiplicate those 2 matrices together
    mat4s resultTransform = glms_mat4_mul(transform_temp, scaleTransform);
    ShaderSetMat4(shader, "u_Transform", resultTransform.raw);
    

    VertexArrayBind(QuadVertexArray);
    RendererDrawIndexed(QuadVertexArray);
} 

void Renderer2DEndScene()
{

}
