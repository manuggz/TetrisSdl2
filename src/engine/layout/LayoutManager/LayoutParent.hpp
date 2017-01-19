//
// Created by manuggz on 20/11/16.
//

#ifndef BOMBERMAN_LAYOUTPARENT_HPP
#define BOMBERMAN_LAYOUTPARENT_HPP

#include <SDL2/SDL.h>
#include "LayoutComponent.hpp"

class LayoutParent{
public:
    
    LayoutParent(LayoutComponent * layout){
        mLayout = layout;
    }

    /**
     * Dibuja el mLayout y sus componentes de arriba hacia abajo / verticalmente
     * @param renderer
     */
    void establecerRectDibujo(SDL_Rect & gRenderer){
        /*
        SDL_Rect drawRect = {drawRectParent.x,drawRectParent.y,0,0};
        

        if(mLayout->getLayoutParam(LAYOUT_PARAM_WRAP_WIDTH) == LAYOUT_PARAM_TRUE){
            drawRect.w = mLayout->getInternalWidth();
        }else if(mLayout->getLayoutParam(LAYOUT_PARAM_FILL_PARENT_WIDTH) == LAYOUT_PARAM_TRUE){
            // Solo deberia haber un componente agregado
            drawRect.w = drawRectParent.w;
        }

        // Solo deberia haber un componente agregado si se usa una de estas variables
        if(mLayout->getLayoutParam(LAYOUT_PARAM_FILL_PARENT_HEIGHT) == LAYOUT_PARAM_TRUE){
            drawRect.h = drawRectParent.h;
        }else if(mLayout->getLayoutParam(LAYOUT_PARAM_WRAP_HEIGHT) == LAYOUT_PARAM_TRUE){
            drawRect.h = mLayout->getInternalHeight();
        }

        if(mLayout->getLayoutParam(LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL) == LAYOUT_PARAM_TRUE){
            drawRect.x = drawRect.x +  drawRectParent.w/2  - drawRect.w/2;
        }

        if(mLayout->getLayoutParam(LAYOUT_PARAM_CENTER_PARENT_VERTICAL) == LAYOUT_PARAM_TRUE){
            drawRect.y = drawRect.y +  drawRectParent.h/2  - drawRect.h/2;
        }
        // Dibujamos el componente
        mLayout->draw(gRenderer,drawRect);
        mLayout->setDisabled(false);*/
    }

private:
    LayoutComponent *mLayout;
};
#endif //BOMBERMAN_LAYOUTPARENT_HPP
