#include "Renderer2D.h"
#include "shader/shader.h"
#include "src/core/error.h"
#include "vertexArray.h"
#include "renderer.h"

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
            BufferElement(GL_FLOAT, 3, NOT_NORMALIZED)
        );
        
        VertexArrayAddVertexBuffer(QuadVertexArray, vertexBuffer);


        DeleteVertexBuffer(vertexBuffer);
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
    

    ShaderUploadUniformMat4(FlatColorShader, "u_ViewProj", orthographicCameraGetViewPosMat(camera).raw);
    ShaderUploadUniformMat4(FlatColorShader, "u_Transform", NO_TRANSFORM.raw);
}

void Renderer2DDrawQuad(v3 position, v2 scale, v4 color)
{
    ShaderBind(FlatColorShader);
    ShaderUploadUniformFloat4(FlatColorShader, "u_Color", color);   // Upload Color to Shader

    VertexArrayBind(QuadVertexArray);
    RendererDrawIndexed(QuadVertexArray);
}


void Renderer2DEndScene()
{

}
