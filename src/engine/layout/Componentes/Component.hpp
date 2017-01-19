//
// Created by manuggz on 20/11/16.
//

#ifndef BOMBERMAN_COMPONENT_HPP
#define BOMBERMAN_COMPONENT_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_map>
#include "../../util/LTexture.hpp"

class Component{
public:

    /**
     * Inicializa el componente
     * @return
     */
    Component(){


        mTextureBackground = nullptr;

        mDisabled = true;

        mInternalRect.x = mInternalRect.y = mInternalRect.w = mInternalRect.h = 0;
//        mWidth = 0;
  //      mHeight = 0;

        mBackgroundColor.r = mBackgroundColor.g =mBackgroundColor.b = mBackgroundColor.a = 0;
        mEstablecidoColorFondo = false;
    }
    virtual void procesarEvento(SDL_Event *pEvent) {
    }

    void setLayoutParam(std::string nameParam,std::string valueParam){
        mLayoutParams[nameParam] = valueParam;
    }
    void addLayoutParam(std::string nameParam,std::string valueParam){
        if(mLayoutParams[nameParam].empty())
            mLayoutParams[nameParam] = valueParam;
    }

    std::string getLayoutParam(std::string nameParam){
        return mLayoutParams[nameParam];
    }
    /**
     * Establece la textura a dibujar en el fondo del componente
     * @param lTexture
     */
    void setBackgroundTexture(LTexture * lTexture,bool deleteAnterior = true){
        if(mTextureBackground != nullptr && deleteAnterior){
            mTextureBackground->free();
            delete lTexture;
            mTextureBackground = nullptr;
        }
        mTextureBackground = lTexture;
    }

    /**
     * Establece un estado que hace que redibuje este componente al mismo tiempo
     * que se recalculen los estados del layout que contiene este componente.
     *
     * Después de modificar un/varios estado de este componente se debe llamar a esta funcion para que
     * se puedan ver en la pantalla.
     */
    void setDisabled(bool nuevo){
        mDisabled = nuevo;
    }

    /**
     * Retorna el estado de la variable mDisabled.
     * @return
     */
    virtual bool isDisabled(){
        return mDisabled;
    }


    int getInternalWidth(){
        return mInternalRect.w;
    }

    int getInternalHeight(){
        return mInternalRect.h;
    }

    /**
     * Se encarga de crear las texturas del componente asi como otras acciones asociadas con el renderer
     * Notar que despues de esta funcion se debe llamar a calculateRect, es decir, realizar una nueva
     * calculacion del rectangulo de dibujo del componente
     * @param gRenderer Render de la pantalla
     */
    virtual void pack(SDL_Renderer * gRenderer) = 0;

    virtual void setRectDibujo(SDL_Rect &rect){
        mDrawRect = rect;
    }

    /**
     * Establece un color de fondo para el componente
     * @param color
     */
    void setBackgroundColor(SDL_Color color){
        mBackgroundColor = color;
        mEstablecidoColorFondo = true;
    }

    /**
     * Dibuja el componente
     * @param gRenderer
     */
    virtual void draw(SDL_Renderer * gRenderer){


        // Dibuja el color de fondo
        if(mEstablecidoColorFondo){
            Uint8 r,g,b,a;
            SDL_GetRenderDrawColor(gRenderer,&r,&g,&b,&a);
            SDL_SetRenderDrawColor(gRenderer,mBackgroundColor.r,mBackgroundColor.g,mBackgroundColor.b,mBackgroundColor.a);
            SDL_RenderFillRect(gRenderer,&mDrawRect);
            SDL_SetRenderDrawColor(gRenderer,r,g,b,a);
        }

        // Dibuja la imagen de background
        if(mTextureBackground != nullptr){
            SDL_Rect clip = {0,0,mDrawRect.w,mDrawRect.h};
            mTextureBackground->draw(gRenderer, mDrawRect.x, mDrawRect.y, &clip);
        }



    };


    void setBackgroundTexture(SDL_Renderer *pRenderer, std::string ruta, bool tieneColorClave) {
        if(!mTextureBackground){
            mTextureBackground = new LTexture();
        }
        mTextureBackground->cargarDesdeArchivo(ruta, pRenderer, false);
    }

protected:

    std::unordered_map<std::string,std::string> mLayoutParams;

    LTexture * mTextureBackground;

    bool mDisabled;

    // Este es el rectangulo que representa el contenido real del componente
    // Se distingue del de dibujo porque puede ser mas grande
    SDL_Rect mInternalRect;

    SDL_Color mBackgroundColor;
    bool mEstablecidoColorFondo; // Controla si se debe dibujar el color de fondo

    SDL_Rect mDrawRect;


};
#endif //BOMBERMAN_COMPONENT_HPP
