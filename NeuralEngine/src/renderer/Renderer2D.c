#include "Renderer2D.h"
#include "shader/shader.h"
#include "src/core/error.h"
#include "vertexArray.h"
#include "renderer.h"
#include "textures/texture.h"

static Shader* FlatColorShader = NULL;
static VertexArray* QuadVertexArray = NULL; 

void Renderer2DInit()
{
    QuadVertexArray = NewVertexArray();

    // Quad Vertex Buffer 
        {
        float SquareVertices[5 * 4] = 
        {
            -0.5, -0.5, 0.0f, 
            0.5, -0.5,  0.0f,
            0.5,  0.5,  0.0f,
            -0.5,  0.5, 0.0f
        };

        VertexBuffer* vertexBuffer = NewVertexBuffer(SquareVertices, sizeof(SquareVertices));

        VertexBufferSetLayout(vertexBuffer, 1,
            BufferElement(NEURAL_FLOAT, 3)
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



    FlatColorShader = NewShader("FlatColorShader", "res/shader/FlatColor.glsl");

    Assert(FlatColorShader == NULL, "Failed to Create FlatColor Shader");
}

void Renderer2DShutdown()
{
    DeleteVertexArray(QuadVertexArray);
    DeleteShader(FlatColorShader);
}


void Renderer2DBeginScene(Camera* camera)
{
    ShaderBind(FlatColorShader);
    

    ShaderSetMat4(FlatColorShader, "u_ViewProj", orthographicCameraGetViewPosMat(camera).raw);
    ShaderSetMat4(FlatColorShader, "u_Transform", NO_TRANSFORM.raw);
}

void Renderer2DDrawColoredQuad(v3 position, v2 scale, v4 color)
{
    ShaderBind(FlatColorShader);
    ShaderSetFloat4(FlatColorShader, "u_Color", color);   // Upload Color to Shader
    
    // Calculation Transform Matrix

    // Scale Transform
    mat4s scaleTransform = GLMS_MAT4_IDENTITY_INIT; 
    scaleTransform = glms_scale(scaleTransform, vec3s(scale.x, scale.y, 1.0f ));
    
    // Position Translation
    mat4s transform_temp = glms_translate_make(vec3s(position.x, position.y, position.z));

    // Multiplicate those 2 matrices together
    mat4s resultTransform = glms_mat4_mul(transform_temp, scaleTransform);
    ShaderSetMat4(FlatColorShader, "u_Transform", resultTransform.raw);
    

    VertexArrayBind(QuadVertexArray);
    RendererDrawIndexed(QuadVertexArray);
}

void Renderer2DDrawTexturedQuad(v3 position, v2 scale, Texture* texture)
{
    ShaderBind(FlatColorShader);
    ShaderSetInt()
    
    // Calculation Transform Matrix

    // Scale Transform
    mat4s scaleTransform = GLMS_MAT4_IDENTITY_INIT; 
    scaleTransform = glms_scale(scaleTransform, vec3s(scale.x, scale.y, 1.0f ));
    
    // Position Translation
    mat4s transform_temp = glms_translate_make(vec3s(position.x, position.y, position.z));

    // Multiplicate those 2 matrices together
    mat4s resultTransform = glms_mat4_mul(transform_temp, scaleTransform);
    ShaderSetMat4(FlatColorShader, "u_Transform", resultTransform.raw);
    

    VertexArrayBind(QuadVertexArray);
    RendererDrawIndexed(QuadVertexArray);
}

void Renderer2DEndScene()
{

}
