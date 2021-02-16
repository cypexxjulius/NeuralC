
#define GLEW_STATIC


#include <stdio.h>

#include "src/core/opengl.h"
#include "src/core/window.h"
#include "src/core/system.h"
#include "src/utils/fileio.h"
#include "src/utils/types.h"
#include "src/core/error.h"

#include "src/shader/shader.h"


int main()
{
    n_window *window = n_createWindow(1200, 800, "Test Window");

    n_initError();

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

    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);


    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    n_createShader(window, "res/shader/vertexShader.vs", "res/shader/fragmentShader.fs");


    glUseProgram(window->shader);

    while(!glfwWindowShouldClose(window->windowHandle))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_INT, NULL);
        
        // Swap Buffers 
        glfwSwapBuffers(window->windowHandle);
        glfwPollEvents();
    }
    glDeleteProgram(window->shader);
    glfwTerminate();
    return 0;
}
