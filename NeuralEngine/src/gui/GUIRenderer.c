#include "GUI.h"
#include "GUIItems.h"
#include "src/renderer/Renderer2D.h" 

void GUIRenderBox(GUIBox* box)
{
    
    // Get GUI font
    Font *font = GUIGetFont();

    float widgetPadding, widgetMargin;
    float fontPadding;

    // Recieve paddings for widgets and font elements
    GUIStyleGet(&widgetPadding, &fontPadding, &widgetMargin);

    // Prepare widgets, copy text to buffer, calculate height and width
    GUIBoxPrepare(box, font, widgetPadding, fontPadding);

    // Calculate box height
    float height = box->height;
    if(height == 0)
        height = GUIBoxCalculateHeight(box, font, widgetMargin);
    
    // Calculate the box width
    float width = box->width;
    if(width == 0)
        width = GUIBoxCalculateWidth(box, font, widgetMargin);

    height *= 50;
    width *= 50;

    Renderer2DBeginScene(NULL);
    
    float renderHeight = 0;
    GUIWidget* widget = NULL;
    for(u16 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);

        Renderer2DDrawQuad(
            (Quad2D){
                .color = V4(1.0, 1.0, 1.0, 1.0),
                .height = widget->height,
                .width = width - 2 * widgetMargin,
                .position = V2(box->Position.x, box->Position.y - renderHeight),    
                .text = (TextElement []) {
                    (TextElement){
                        .string = widget->String,
                        .fontSize = 1.0f,
                        .maxLength = width - 2 * widgetMargin,
                        .color = V3(1.0, 1.0, 1.0)
                    }
                }
            }
        );
        

        renderHeight += widget->height + 2 * widgetMargin;

    }

    Renderer2DDrawQuad(
        (Quad2D){
            .color = V4(0.1, 0.0, 0.4, 1.0),
            .height = height,
            .width = width,
            .position = box->width
        }
    );

    Renderer2DEndScene();
}