//
// Created by manuggz on 11/12/16.
//

#ifndef TETRIS_CSPRITECUADRO_HPP
#define TETRIS_CSPRITECUADRO_HPP

#include "CSprite.hpp"

class SpriteCuadro: public Sprite{
public:

    static int stBorderWidth;
    static int stWidth;
    static int stHeight;
    static SDL_Color stBackGroundColor;
    static SDL_Color stBorderColor;

    SpriteCuadro(int x,int y) {
        move(x,y);
        rect.w = stWidth;
        rect.h = stHeight;
        mBackGroundColor = stBackGroundColor;
        mBorderWidth = stBorderWidth;
        mBackGroundColor = stBackGroundColor;
        mBorderColor = stBorderColor;
    }

    void setBorderColor(SDL_Color nuevo){
        mBorderColor = nuevo;
    }

    SDL_Color getBorderColor(){
        return mBorderColor;
    }

    void setBackgroundColor(SDL_Color nuevo){
        mBackGroundColor = nuevo;
    }

    SDL_Color getBackgroundColor(){
        return mBackGroundColor;
    }

    void setBorderWidth(Uint8 nueva){
        mBorderWidth = nueva;
    }

    int getBorderWidth(Uint8 nueva){
        return mBorderWidth;
    }

    void update(const Uint8 *keys) override {

    }

    void draw(SDL_Renderer *gRenderer) override {
        static SDL_Rect rectTemp {0,0,0,0};

        if(mBorderWidth > 0) {

            SDL_SetRenderDrawColor(gRenderer, mBorderColor.r, mBorderColor.g, mBorderColor.b, mBorderColor.a);
            SDL_RenderFillRect(gRenderer, &rect);

            rectTemp.x = rect.x + mBorderWidth;
            rectTemp.y = rect.y + mBorderWidth;
            rectTemp.w = rect.w - mBorderWidth*2;
            rectTemp.h = rect.h - mBorderWidth*2;

            SDL_SetRenderDrawColor(gRenderer, mBackGroundColor.r, mBackGroundColor.g, mBackGroundColor.b,
                                   mBackGroundColor.a);
            SDL_RenderFillRect(gRenderer, &rectTemp);
        }else{
            SDL_SetRenderDrawColor(gRenderer, mBackGroundColor.r, mBackGroundColor.g, mBackGroundColor.b,
                                   mBackGroundColor.a);
            SDL_RenderFillRect(gRenderer, &rect);

        }

    }

    static void draw(SDL_Renderer *gRenderer,int x,int y){
        static SDL_Rect rectTemp {0,0,0,0};
        static SDL_Rect rect {0,0,0,0};

        rect.x = x;
        rect.y = y;
        rect.w = stWidth;
        rect.h = stHeight;
        if(stBorderWidth > 0) {

            SDL_SetRenderDrawColor(gRenderer, stBorderColor.r, stBorderColor.g, stBorderColor.b, stBorderColor.a);
            SDL_RenderFillRect(gRenderer, &rect);

            rectTemp.x = x + stBorderWidth;
            rectTemp.y = y + stBorderWidth;
            rectTemp.w = stWidth  - stBorderWidth*2;
            rectTemp.h = stHeight - stBorderWidth*2;

            SDL_SetRenderDrawColor(gRenderer, stBackGroundColor.r, stBackGroundColor.g, stBackGroundColor.b,
                                   stBackGroundColor.a);
            SDL_RenderFillRect(gRenderer, &rectTemp);
        }else{
            SDL_SetRenderDrawColor(gRenderer, stBackGroundColor.r, stBackGroundColor.g, stBackGroundColor.b,
                                   stBackGroundColor.a);
            SDL_RenderFillRect(gRenderer, &rect);

        }
    }

private:
    SDL_Color mBackGroundColor {0,0,0,255};
    SDL_Color mBorderColor {0,0,0,255};
    int mBorderWidth = 0;
};

int SpriteCuadro::stBorderWidth = 0;
int SpriteCuadro::stWidth = 0;
int SpriteCuadro::stHeight = 0;
SDL_Color SpriteCuadro::stBackGroundColor {0,0,0,255};
SDL_Color SpriteCuadro::stBorderColor {0,0,0,255};

#endif //TETRIS_CSPRITECUADRO_HPP
