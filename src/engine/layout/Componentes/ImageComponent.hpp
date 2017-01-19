//
// Created by manuggz on 20/11/16.
//

#ifndef BOMBERMAN_IMAGECOMPONENT_HPP
#define BOMBERMAN_IMAGECOMPONENT_HPP

#include "Component.hpp"

class ImageComponent: public Component{
public:
    virtual void pack(SDL_Renderer *gRenderer) override {

        if(mTextureBackground != nullptr){
            mInternalRect.w = mTextureBackground->getWidth();
            mInternalRect.h = mTextureBackground->getHeight();
        }
    }

    void setImageTexture(LTexture * texture,bool deleteAnterior = true) {
        Component::setBackgroundTexture(texture,deleteAnterior);
    }

};
#endif //BOMBERMAN_IMAGECOMPONENT_HPP
