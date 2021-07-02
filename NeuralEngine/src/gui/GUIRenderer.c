#include "GUI.h"
#include "GUIItems.h"
#include "src/renderer/Renderer2D.h" 
#include "src/core/Application.h"


void GUIRenderBox(GUIBox* box, GUIAssetManager* AssetManager, GUIController* Controller)
{
    // Drawing the box Canvas
    Renderer2DDrawQuad(
        &(Quad2D){
            .color = V4(0.01f, 0.1f, 0.2f, 1.0f),
            .height = box->height,
            .width = box->width,
            .position = box->Position
        }
    );

    // Box heading background
    Renderer2DDrawQuad(
        &(Quad2D){
            .color = V4(0.2f, 0.1f, 0.3f, 1.0f),
            .height = box->NameHeight,
            .width = box->width,
            .position = box->Position
        }
    );

    
    Renderer2DText(
        AssetManager->Font, 
        box->BoxName,
        AssetManager->fontSizeHeading, 
        V3(1.0, 1.0, 1.0), 
        V2(
            box->Position.x + AssetManager->fontPadding + (box->width - box->NameWidth) / 2, 
            box->Position.y - AssetManager->fontPadding
        ),
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
        float maxWidth = 0.0f; 

        switch(widget->type)
        {
            case GUIWidgetTypeButton:

                Renderer2DDrawQuad(
                    &(Quad2D) {
                        .color = V4(0.3f, 0.2f * widget->isPressed, 0.8f, 1.0f),
                        .position = widgetPosition,
                        .width = widget->width,
                        .height = widget->height
                    }
                );

                Renderer2DText(
                    AssetManager->Font, 
                    widget->Data,
                    AssetManager->fontSize, 
                    V3(1.0, 1.0, 1.0), 
                    V2(widgetPosition.x + AssetManager->fontPadding, widgetPosition.y - AssetManager->fontPadding),
                    1, 
                    box->width - 2 * AssetManager->widgetMargin, 
                    widget->height
                );   

                break;
            
            case GUIWidgetTypeText:

                Renderer2DText(
                    AssetManager->Font, 
                    widget->Data,
                    AssetManager->fontSize, 
                    V3(1.0, 1.0, 1.0), 
                    V2(widgetPosition.x + AssetManager->fontPadding, widgetPosition.y - AssetManager->fontPadding),
                    1, 
                    box->width - 2 * AssetManager->widgetMargin, 
                    widget->height
                ); 

                break;

            case GUIWidgetTypeTextEdit:

                Renderer2DDrawQuad(
                    &(Quad2D) {
                        .color = V4(1.0f, 1.0f, 1.0f, 0.7f + 0.3f * widget->isPressed),
                        .position = widgetPosition,
                        .width = widget->width,
                        .height = widget->height
                    }
                );
                
                maxWidth = Renderer2DText(
                    AssetManager->Font, 
                    widget->Data,
                    AssetManager->fontSize, 
                    V3(0.0, 0.0, 0.0), 
                    V2(widgetPosition.x + AssetManager->fontPadding, widgetPosition.y - AssetManager->fontPadding),
                    1, 
                    widget->width - AssetManager->cursorWidth, 
                    widget->height
                ); 

                if(widget->isPressed)
                {
                    float transparency = (sinf(Controller->startTime * 6) + 1.0f) * 0.5f;

                    Renderer2DDrawQuad(
                        &(Quad2D) {
                            .color = V4(0.0f, 0.0f, 0.0f, transparency),
                            .position = V2(
                                widgetPosition.x + maxWidth + AssetManager->fontPadding,
                                widgetPosition.y - AssetManager->fontPadding * 0.75f
                            ),
                            .width = AssetManager->cursorWidth,
                            .height = widget->height - AssetManager->fontPadding,
                            .zIndex = 10
                        }
                    );
                }

                break;
            
            case GUIWidgetTypeColorEdit3f:

                Renderer2DDrawQuad(
                    &(Quad2D) {
                        .color = V4(1.0f, 1.0f, 1.0f, 1.0f),
                        .position = widgetPosition,
                        .width = widget->height,
                        .height = widget->height,
                        .texture = AssetManager->ColorEdit3fTexture
                    }
                );

                v3 color = *(v3 *)widget->Data;
                Renderer2DDrawQuad(
                    &(Quad2D) {
                        .color = V4(color.x, color.y, color.z, 1.0f),
                        .position = V2(widgetPosition.x + widget->height + AssetManager->widgetMargin, widgetPosition.y),
                        .width = widget->height / 5,
                        .height = widget->height,
                    }
                );

                

                break;

        }

        renderHeight += widget->height + 2 * AssetManager->widgetMargin;
    }

}