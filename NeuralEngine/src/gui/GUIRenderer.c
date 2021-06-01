#include "GUI.h"
#include "GUIItems.h"
#include "src/renderer/Renderer2D.h" 
#include "src/core/Application.h"


void GUIRenderBox(GUIBox* box)
{
    // Get GUI font
    Font *font = GUIGetFont();

    float widgetPadding, widgetMargin;
    float fontPadding;
    float fontSize;

    // Recieve paddings for widgets and font elements
    GUIStyleGet(WIDGET_PADDING, &widgetPadding);
    GUIStyleGet(FONT_PADDING, &fontPadding);
    GUIStyleGet(WIDGET_MARGIN, &widgetMargin);
    GUIStyleGet(FONT_SIZE, &fontSize);


    // Prepare widgets, copy text to buffer, calculate height and width
    GUIBoxPrepare(box, font, widgetPadding, fontPadding, fontSize);

    // Calculate box height
    float height = box->height;
    if(height == 0)
        height = GUIBoxCalculateHeight(box, font, widgetMargin);
    
    // Calculate the box width
    float width = box->width;
    if(width == 0)
        width = GUIBoxCalculateWidth(box, font, widgetMargin);

    Renderer2DBeginScene(NULL);

    // Drawing the box Canvas
    Renderer2DDrawQuad(
        &(Quad2D){
            .color = V4(0.01f, 0.1f, 0.2f, 1.0f),
            .height = height,
            .width = width,
            .position = box->Position
        }
    );


    Renderer2DDrawQuad(
        &(Quad2D){
            .color = V4(0.0f, 0.0f, 0.0f, 1.0f),
            .height = box->NameHeight,
            .width = width,
            .position = box->Position
        }
    );

    
    Renderer2DText(
        font, 
        box->BoxName, 
        (u16)strlen(box->BoxName),
        fontSize, 
        V3(1.0, 1.0, 1.0), 
        V2(box->Position.x + fontPadding, box->Position.y - fontPadding),
        1, 
        box->NameWidth, 
        box->NameHeight
    ); 



    float renderHeight = box->NameHeight + widgetMargin;
    GUIWidget* widget = NULL;
    for(u16 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);
        
        v2 widgetPosition = V2(box->Position.x + widgetMargin, box->Position.y - (renderHeight + widgetMargin)); 

        Renderer2DDrawQuad(
            &(Quad2D){
                .color = V4(1.0, 1.0, 1.0, 1.0),
                .height = widget->height,
                .width = width - 2 * widgetMargin,
                .position = widgetPosition,
            }
        );
            

        Renderer2DText(
            font, 
            widget->String, 
            widget->textLength,
            fontSize, 
            V3(0.0, 0.0, 0.0), 
            V2(widgetPosition.x + fontPadding, widgetPosition.y - fontPadding),
            1, 
            width - 2 * widgetMargin, 
            widget->height
        );        


        renderHeight += widget->height + 2 * widgetMargin;

    }

    Renderer2DEndScene();
}