//
// Created by manuggz on 22/11/16.
//

#ifndef BOMBERMAN_SPRITESHEET_HPP
#define BOMBERMAN_SPRITESHEET_HPP

#include "LTexture.hpp"

class SpriteSheet{
public:

    SpriteSheet()= default;

    SpriteSheet(SDL_Renderer * gRenderer,std::string ruta, int nFilas, int nColumnas,bool colorClave = false){
        cargarDesdeArchivo(gRenderer,ruta,nFilas,nColumnas,colorClave);
    }

    SpriteSheet(LTexture * texture, int nFilas, int nColumnas){
        mRectSource.w = texture->getWidth()/nColumnas;
        mRectSource.h = texture->getHeight()/nFilas;
        mFilas = nFilas;
        mColumnas =nColumnas;
        mTexture = texture;
    }

    bool cargarDesdeArchivo(SDL_Renderer * gRenderer,std::string ruta,int nFilas,int nColumnas,bool colorClave = false){

        if(mTexture != nullptr){
            delete mTexture;
            mTexture = nullptr;
        }
        mTexture = new LTexture();
        if(mTexture->cargarDesdeArchivo(ruta, gRenderer, colorClave)){
            SDL_Log("Cargada SpriteSheet : %s.",ruta.c_str());
        }else{
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"No se logro cargar SpriteSheet: %s.",ruta.c_str());
            return false;
        }
        mRectSource.x = 0;
        mRectSource.y = 0;
        mRectSource.w = mTexture->getWidth()/nColumnas;
        mRectSource.h = mTexture->getHeight()/nFilas;
        mFilas = nFilas;
        mColumnas =nColumnas;
        return true;
    }

    int getWidthCuadro() {
        return mRectSource.w;
    }

    void setCurrentCuadro(int nCuadro){
        mRectSource.x = (nCuadro % mColumnas) * mRectSource.w;
        mRectSource.y = (nCuadro / mColumnas) * mRectSource.h;
        mIndiceCuadro = nCuadro;
    }
    int getCurrentCuadroIndice(){
        return mIndiceCuadro;
    }
    void setAlpha(Uint8 nuevoValor){
        //mAlpha = nuevoValor;
        mTexture->setBlendMode(SDL_BLENDMODE_BLEND);
        mTexture->setAlpha(nuevoValor);
    }
    int getHeightCuadro() {
        return mRectSource.h;
    }

    void draw(SDL_Renderer * gRenderer,int x,int y){
        mTexture->draw(gRenderer, x, y, &mRectSource);
    }

    int getNColumnas() {
        return mColumnas;
    }
    int getNFilas() {
        return mFilas;
    }

    ~SpriteSheet(){
        delete mTexture;
    }

    std::string getPath(){
        return mTexture->getPath();
    }
private:
    LTexture * mTexture = nullptr;
    SDL_Rect mRectSource {0,0,0,0};
    int mFilas = 0;
    int mColumnas = 0;

    //int mAlpha = -1;
    int mIndiceCuadro = 0;
};
#endif //BOMBERMAN_SPRITESHEET_HPP
