//
// Created by manuggz on 19/11/16.
//

#ifndef BOMBERMAN_LAYOUTVERTICAL_HPP
#define BOMBERMAN_LAYOUTVERTICAL_HPP

#include <deque>
#include "LayoutComponent.hpp"


class LayoutVertical: public LayoutComponent{
public:

    void addComponent(Component * nuevoComponente){


        nuevoComponente->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_WIDTH,LAYOUT_PARAM_FALSE);
        nuevoComponente->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_HEIGHT,LAYOUT_PARAM_FALSE);

        nuevoComponente->setLayoutParam(LAYOUT_PARAM_WRAP_WIDTH,LAYOUT_PARAM_TRUE);
        nuevoComponente->setLayoutParam(LAYOUT_PARAM_WRAP_HEIGHT,LAYOUT_PARAM_TRUE);

        nuevoComponente->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL,LAYOUT_PARAM_FALSE);
        nuevoComponente->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_VERTICAL,LAYOUT_PARAM_FALSE);

        nuevoComponente->addLayoutParam(LAYOUT_PARAM_WIDTH,"0");
        nuevoComponente->addLayoutParam(LAYOUT_PARAM_HEIGHT,"0");

        mComponentes.push_back(nuevoComponente);
    }


    /**
     * Chequea si hay algun componente con disable = true o si el layout en si esta en ese estado
     * @return True si hay alguno con ese estado
     */
    bool isDisabled() {
        std::deque<Component*>::iterator pComponente= mComponentes.begin();

        while(pComponente != mComponentes.end()){
            if((*pComponente)->isDisabled()){
                return true;
            }
            pComponente++;
        }

        return mDisabled;
    }

    /**
     * Llama a los compactadores de los componentes agregados
     * @param gRenderer
     */
    void pack(SDL_Renderer * gRenderer) override {

        std::deque<Component*>::iterator pComponente= mComponentes.begin();

        mInternalRect.w = 0;
        mInternalRect.h = 0;

        while(pComponente != mComponentes.end()){
            (*pComponente)->pack(gRenderer);

            if((*pComponente)->getInternalWidth() > mInternalRect.w){
                mInternalRect.w = (*pComponente)->getInternalWidth();
            }

            mInternalRect.h+= (*pComponente)->getInternalHeight();

            pComponente++;
        }
    }

    virtual void setRectDibujo(SDL_Rect &rect) override {
        Component::setRectDibujo(rect);
        // Iterador de los componentes del layout
        std::deque<Component*>::iterator pComponente= mComponentes.begin();

        SDL_Rect drawRect = {rect.x,rect.y,0,0};

        while(pComponente != mComponentes.end()){

            drawRect.x = rect.x;
            drawRect.w = std::stoi((*pComponente)->getLayoutParam(LAYOUT_PARAM_WIDTH));
            drawRect.h = std::stoi((*pComponente)->getLayoutParam(LAYOUT_PARAM_HEIGHT));

            if((*pComponente)->getLayoutParam(LAYOUT_PARAM_WRAP_WIDTH) == LAYOUT_PARAM_TRUE){
                drawRect.w = (*pComponente)->getInternalWidth();
            }else if((*pComponente)->getLayoutParam(LAYOUT_PARAM_FILL_PARENT_WIDTH) == LAYOUT_PARAM_TRUE){
                // Solo deberia haber un componente agregado
                drawRect.w = rect.w;
            }

            // Solo deberia haber un componente agregado si se usa una de estas variables
            if((*pComponente)->getLayoutParam(LAYOUT_PARAM_FILL_PARENT_HEIGHT) == LAYOUT_PARAM_TRUE){
                drawRect.h = rect.h;
            }else if((*pComponente)->getLayoutParam(LAYOUT_PARAM_WRAP_HEIGHT) == LAYOUT_PARAM_TRUE){
                drawRect.h = (*pComponente)->getInternalHeight();
            }

            if((*pComponente)->getLayoutParam(LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL) == LAYOUT_PARAM_TRUE){
                drawRect.x = drawRect.x +  rect.w/2  - drawRect.w/2;
            }

            if((*pComponente)->getLayoutParam(LAYOUT_PARAM_CENTER_PARENT_VERTICAL) == LAYOUT_PARAM_TRUE){
                drawRect.y = drawRect.y +  rect.h/2  - drawRect.h/2;
            }
            // Dibujamos el componente
            (*pComponente)->setRectDibujo(drawRect);
            drawRect.y += drawRect.h;

            pComponente++; // siguiente cmoponente
        }
    }

};
#endif //BOMBERMAN_LAYOUTVERTICAL_HPP
