
#define GLEW_STATIC


#include <stdio.h>

#include "src/core/window.h"
#include "src/core/system.h"
#include "src/utils/fileio.h"
#include "src/core/error.h"
#include "src/events/event.h"

#include "src/renderer/renderer.h"

#include "src/core/opengl.h"
#include "src/shader/shader.h"


#include "src/events/keycode.h"

int main()
{
    n_Window *window = n_createWindow(1200, 800, "Test Window");

    n_printSystemInformation();

    float positions[] = {
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5

    };
    unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };


    n_VertexArray* vertexArray = newVertexArray();


    n_VertexBuffer vertexBuffer = newVertexBuffer(positions, sizeof(positions));

    n_VertexBufferLayout* layout = newVertexBufferLayout();

    vertexBufferLayoutPush(layout, GL_FLOAT, 2);

    vertexArrayAddBuffer(vertexArray, &vertexBuffer, layout);



    n_IndexBuffer* indexBuffer = newIndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));

    n_createShader(window, "res/shader/vertexShader.vs", "res/shader/fragmentShader.fs");

    // Bind shader
    glUseProgram(window->shader);

    int location = glGetUniformLocation(window->shader, "u_Color");
    if(location != -1)
    {
        glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);
    }

    float r = 0.0f;
    float increment = 0.05f;

    glUseProgram(0);
    vertexBufferUnbind();
    indexBufferUnbind();

    while(!window->shouldClose)
    {
        if(n_isButtonPressed(window, NL_KEY_W))
            r += increment;
            

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(window->shader);

        vertexArrayBind(vertexArray);
        indexBufferBind(indexBuffer);


        if(location != -1)
            glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        
        if(r > 1.0)
            increment = -0.05f;
        else if(r < 0)
            increment = 0.05f;


        // Swap Buffers 
        glfwSwapBuffers(window->windowHandle);
        glfwPollEvents();

        GLenum errorID;
        while((errorID = glGetError()) != GL_NO_ERROR) 
        { 
            fprintf(stderr, "%s %d\n","[OPENGL ERROR]", errorID);
        };

    }
    
    deleteIndexBuffer(indexBuffer);
    deleteVertexBuffer(vertexBuffer);
    deleteVertexBufferLayout(layout);
    deleteVertexArray(vertexArray);

    glDeleteProgram(window->shader);
    glfwTerminate();
    return 0;
}
