//
// Created by manuggz on 13/12/16.
//

#ifndef TETRIS_ANIMACIONELIMINARLINEA_HPP
#define TETRIS_ANIMACIONELIMINARLINEA_HPP


#include <SDL.h>
#include <iostream>
#include "SimpleAnimacion.hpp"

class AnimacionDesvanecerOut: public SimpleAnimacion{
public:

    AnimacionDesvanecerOut(SDL_Texture *textureOut, int x, int y, int delay,int avanceSpeed){
        mTexture = textureOut;
        mMaxDelay = delay;

        SDL_SetTextureBlendMode(mTexture,SDL_BLENDMODE_BLEND);
        if(SDL_SetTextureAlphaMod(mTexture,mAlphaValue) < 0){
            std::cerr << "No se logró establecer el Alpha a la Out Texture en la animación. Error: " << SDL_GetError() << std::endl;
        }
        rect.x = x;
        rect.y = y;
        SDL_QueryTexture(textureOut, nullptr, nullptr,&rect.w,&rect.h);
        mAvance = avanceSpeed;
    }

    void update() override {
        if(!mIsRunning) return;

        if(++mDelay >= mMaxDelay){
            mDelay = 0;
            mAlphaValue -=  mAvance;
            if(mAlphaValue <= 0){
                mAlphaValue = 0;
                stop();
            }
            SDL_SetTextureAlphaMod(mTexture,mAlphaValue);
        }
    }

    void draw(SDL_Renderer * gRenderer) override {
        SDL_RenderCopy(gRenderer,mTexture, nullptr,&rect);
    }
    ~AnimacionDesvanecerOut(){
        SDL_DestroyTexture(mTexture);
    }

private:
	SDL_Texture *mTexture;
	SDL_Rect rect{ 0,0,0,0 };
    int mAlphaValue = 255;
    int mDelay = 0;
    int mMaxDelay = 20;
    int mAvance = 35;
};
#endif //TETRIS_ANIMACIONELIMINARLINEA_HPP
