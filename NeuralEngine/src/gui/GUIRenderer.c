#include "GUI.h"
#include "GUIItems.h"
#include "src/renderer/Renderer2D.h" 
#include "src/core/Application.h"


void GUIRenderBox(GUIBox* box)
{
    // Get GUI asset manager
    GUIAssetManager* AssetManager = GUIGetAssetManager();


    // Drawing the box Canvas
    Renderer2DDrawQuad(
        &(Quad2D){
            .color = V4(0.01f, 0.1f, 0.2f, 1.0f),
            .height = box->height,
            .width = box->width,
            .position = box->Position
        }
    );


    Renderer2DDrawQuad(
        &(Quad2D){
            .color = V4(0.0f, 0.0f, 0.3f, 1.0f),
            .height = box->NameHeight,
            .width = box->width,
            .position = box->Position
        }
    );

    
    Renderer2DText(
        AssetManager->Font, 
        box->BoxName,
        AssetManager->fontSize, 
        V3(1.0, 1.0, 1.0), 
        V2(box->Position.x + AssetManager->fontPadding, box->Position.y - AssetManager->fontPadding),
        1, 
        box->NameWidth, 
        box->NameHeight
    ); 



    float renderHeight = box->NameHeight + AssetManager->widgetMargin;
    GUIWidget* widget = NULL;
    for(u16 i = 0; i < box->Widgets.used; i++)
    {
        widget = VectorGet(&box->Widgets, i);
        
        v2 widgetPosition = V2(box->Position.x + AssetManager->widgetMargin, box->Position.y - (renderHeight + AssetManager->widgetMargin)); 

        if(widget->type == GUIWidgetTypeButton)
        {
            Renderer2DDrawQuad(
                &(Quad2D) {
                    .color = V4(0.3f, 0.2f * widget->isPressed, 0.8f, 1.0f),
                    .position = widgetPosition,
                    .width = widget->width,
                    .height = widget->height
                }
            );
        }
        
        Renderer2DText(
            AssetManager->Font, 
            widget->String,
            AssetManager->fontSize, 
            V3(1.0, 1.0, 1.0), 
            V2(widgetPosition.x + AssetManager->fontPadding, widgetPosition.y - AssetManager->fontPadding),
            1, 
            box->width - 2 * AssetManager->widgetMargin, 
            widget->height
        );     




        renderHeight += widget->height + 2 * AssetManager->widgetMargin;
    }

}