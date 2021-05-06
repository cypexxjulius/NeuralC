#include "guimodules.h"
#include "src/renderer/Renderer2D.h"
#include "src/utils/types.h"
#include "src/platform/memory.h"

static Vector* widgets = NULL;
static u32 id = 0;

void InitGUI()
{
    widgets = NewVector(10, sizeof(void *), VECTOR_POINTER | VECTOR_FREE | VECTOR_NOREFILL);
};

void DeinitGUI()
{
    DeleteVector(widgets);
};


u32 NewTextBoxWidget(char **buffer, u32 options)
{
    Widget* this = CreateObject(Widget);
    this->id = id++;
    this->TextBoxWidget.buffer = buffer;
    this->TextBoxWidget.options = options;


    VectorAdd(widgets, this);
    return this->id;
}

void WidgetsRender()
{   
    Renderer2DBeginScene(NULL);


    Widget* this = NULL;
    for(u32 i = 0; i < widgets->used; i++)
    {
        this = VectorGet(widgets, i);
        
        if(this == NULL)
            continue;

        switch (this->type)
        {
            case BoxWidgetType:
                Renderer2DDrawQuad((Quad2D)
                {
                    .position = this->BoxWidget.position,
                    .color = this->BoxWidget.color,
                    .height = this->BoxWidget.height,
                    .width = this->BoxWidget.width
                });
                break;

            case TextBoxWidgetType:
                Renderer2DDrawQuad((Quad2D)
                {
                    .position = this->TextBoxWidget.position,
                    .color = this->TextBoxWidget.color,
                });
                break;
            
            default:
                continue;
        }
    }


    Renderer2DEndScene();
}

void DeleteWidget(u32 handle)
{
    VectorRemove(widgets, handle);
}