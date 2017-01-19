//
// Created by manuggz on 19/11/16.
//

#ifndef BOMBERMAN_MENUTEXTOPTION_HPP
#define BOMBERMAN_MENUTEXTOPTION_HPP


#include <iostream>
#include "../../util/CFont.hpp"

class LabelComponent: public Component{
public:
    LabelComponent(){
        mTexto = "";
        mFuente = nullptr;
        mTextureTexto = nullptr;
        mColor.r = 0;
        mColor.g = 0;
        mColor.b = 0;
        mColor.a = 255;
    }
    void setText(string nuevoTexto){
        mTexto = nuevoTexto;
        setDisabled(true);
    }

    void setFont(CFont * nuevaFuente) {
        if(mFuente != nullptr){
            delete mFuente;
            mFuente = 0x0;
        }
        mFuente = nuevaFuente;
    }

    void pack(SDL_Renderer * gRenderer){

        if(mTextureTexto != nullptr){
            mTextureTexto->free();
            delete mTextureTexto;
            mTextureTexto = nullptr;
        }

        if(mTexto != ""){
            mTextureTexto = mFuente->createTextureFromText(gRenderer,mTexto);
            mInternalRect.w = mTextureTexto->getWidth();
            mInternalRect.h = mTextureTexto->getHeight();
        }else{
            mDrawRect.w = 0;
            mDrawRect.h = 0;
            mInternalRect.w = 0;
            mInternalRect.h = 0;
        }
    }

    void draw(SDL_Renderer *gRenderer) override {
        Component::draw(gRenderer);
        if(mTextureTexto == nullptr) return;

        // Dibujamos el componente
        SDL_Rect clip ={0,0,mDrawRect.w,mDrawRect.h};
        mTextureTexto->draw(gRenderer, mDrawRect.x, mDrawRect.y, &clip);

    }

    void setTextColor(SDL_Color nuevo) {
        if(mFuente != nullptr){
            mFuente->setTextColor(nuevo);
            mColor = nuevo;
        }
        setDisabled(true);
    }
    void setTextSize(int nuevaSize) {
        setFont(mFuente->getPathFont(),nuevaSize);
        setDisabled(true);
    }

    void setFont(string rutaFuente, int size) {
        if(mFuente != nullptr){
            delete mFuente;
        }
        mFuente = new CFont();
        mFuente->loadFont(rutaFuente,size);
        mFuente->setTextColor(mColor);
        setDisabled(true);
    }

    ~LabelComponent(){
        if(mFuente != nullptr){
            delete mFuente;
        }
    }


private:
    string mTexto;
    CFont * mFuente;
    LTexture * mTextureTexto;
    SDL_Color mColor;
};
#endif //BOMBERMAN_MENUTEXTOPTION_HPP
