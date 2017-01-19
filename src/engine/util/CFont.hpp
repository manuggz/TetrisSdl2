//
// Created by manuggz on 19/11/16.
//

#ifndef BOMBERMAN_CFONTIMAGE_HPP
#define BOMBERMAN_CFONTIMAGE_HPP
#include <cstdarg>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "LTexture.hpp"

using namespace std;

/**
 * Escribe un texto en pantalla usando una imagen que contiene las letras en un orden
 */
class CFont {

    public:
        CFont();
        bool loadFont(string pathFont,int size);
        LTexture *  createTextureFromText(SDL_Renderer * gRenderer,string texto);
        void setTextColor(SDL_Color color);
        std::string getPathFont(){
        return mPathFont;
    };
        ~CFont();
    private:
        string mPathFont;
        TTF_Font * mFont ;
        int mSize;
        SDL_Color mColor;
};


#endif //BOMBERMAN_CFONTIMAGE_H
