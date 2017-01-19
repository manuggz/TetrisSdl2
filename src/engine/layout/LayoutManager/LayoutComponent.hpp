//
// Created by manuggz on 19/11/16.
//

#ifndef BOMBERMAN_LAYOUTCOMPONENT_HPP
#define BOMBERMAN_LAYOUTCOMPONENT_HPP


#include "../Componentes/Component.hpp"

#define  LAYOUT_PARAM_FILL_PARENT_WIDTH "FillParentWidth"
#define  LAYOUT_PARAM_FILL_PARENT_HEIGHT "FillParentHeight"

#define  LAYOUT_PARAM_WRAP_WIDTH "WrapWidth"
#define  LAYOUT_PARAM_WRAP_HEIGHT "WrapHeight"

#define  LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL "CenterParentHorizontal"
#define  LAYOUT_PARAM_CENTER_PARENT_VERTICAL "CenterParentVertical"

#define  LAYOUT_PARAM_WIDTH "LAYOUT_PARAM_WIDTH"
#define  LAYOUT_PARAM_HEIGHT "LAYOUT_PARAM_HEIGHT"

#define  LAYOUT_PARAM_TRUE "true"
#define  LAYOUT_PARAM_FALSE "false"

#define  LAYOUT_PARAM_X "x"
#define  LAYOUT_PARAM_Y "y"

class LayoutComponent : public Component{

public:


    /**
     * Dibuja el layout y sus componentes de arriba hacia abajo / verticalmente
     * @param renderer
     */
    void draw(SDL_Renderer * gRenderer) override {
        Component::draw(gRenderer);

        // Iterador de los componentes del layout
        std::deque<Component*>::iterator pComponente= mComponentes.begin();
        while(pComponente != mComponentes.end()){
            (*pComponente)->draw(gRenderer);
            (*pComponente)->setDisabled(false);
            ++pComponente;
        }
        setDisabled(false);
    }

    virtual ~LayoutComponent(){
        mComponentes.clear();
    }

protected:
    std::deque <Component * > mComponentes;
};
#endif //BOMBERMAN_LAYOUTCOMPONENT_HPP
