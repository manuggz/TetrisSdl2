//
// Created by manuggz on 19/01/17.
//

#ifndef TETRIS_MENUCONTEXTUALANIMADO_HPP
#define TETRIS_MENUCONTEXTUALANIMADO_HPP

#include "MenuContextual.hpp"

class MenuContextualAnimado : public MenuContextual{
public:
    typedef enum {
        NINGUNA,
        CAER_IN,
        DESVANECER_FULL_IN,
        DESVANECER_FULL_OUT,
        CAER_OUT,
    }TipoAnimacion;

    MenuContextualAnimado(ContextMenuParent *p_parent, int x, int y, int width, int height) :
            MenuContextual(p_parent,x, y) {
    }

    void createUI(SDL_Renderer *pRenderer) override {
        MenuContextual::createUI(pRenderer);

//        mpTextureBufferTarget = SDL_CreateTexture(pRenderer,
//                                                  pgameManagerInterfazUI->getWindowPixelFormat(),
//                                                  SDL_TEXTUREACCESS_TARGET,
//                                                  pgameManagerInterfazUI->getNativeWidth(),
//                                                  pgameManagerInterfazUI->getNativeHeight());
        mGRenderer = pRenderer;
    }

    void open(TipoAnimacion tipoAnimacion){
        switch(tipoAnimacion){
            case NINGUNA:
                MenuContextual::update();
                break;
            case DESVANECER_FULL_IN:
                delay = 0;
                max_delay = 1;
                alpha_step = 15;
                alpha_actual = 20;
                //SDL_SetTextureAlphaMod(mpTextureBufferTarget,alpha_actual);
                mTextureFondo->setBlendMode(SDL_BLENDMODE_BLEND);
                mTextureFondo->setAlpha(alpha_actual);

                pflechaDerecha->setBlendMode(SDL_BLENDMODE_BLEND);
                pflechaDerecha->setAlpha(alpha_actual);

                pflechaDerecha->setBlendMode(SDL_BLENDMODE_BLEND);
                pflechaDerecha->setAlpha(alpha_actual);

                for(int i = 0; i < opciones_v.size();i++){
                    opciones_v[i]->setAlpha(alpha_actual);
                }
//                SDL_SetRenderTarget(mGRenderer,mpTextureBufferTarget);
//                MenuContextual::draw(mGRenderer);
//                SDL_SetRenderTarget(mGRenderer, nullptr);
                break;
            case DESVANECER_FULL_OUT:
                break;
            case CAER_IN:
                delay = 0;
                max_delay = 1;
                y_inicial = mRectFondo.y;
                mRectFondo.y -= 10;
                break;
        }
        this->tipoAnimacionEjecutando = tipoAnimacion;
    }

    void update(){
        switch(tipoAnimacionEjecutando){
            case NINGUNA:
                MenuContextual::update();
                break;
            case DESVANECER_FULL_IN:
                if(delay++ > max_delay){
                    alpha_actual += alpha_step;
                    delay = 0;
                    if(alpha_actual >= 255){
                        tipoAnimacionEjecutando = NINGUNA;
                        alpha_actual = 255;
                    }
                    mTextureFondo->setAlpha(alpha_actual);
                    pflechaDerecha->setAlpha(alpha_actual);
                    pflechaIzquierda->setAlpha(alpha_actual);
                    for(int i = 0; i < opciones_v.size();i++){
                        opciones_v[i]->setAlpha(alpha_actual);
                    }
                }

                pflechaDerecha->update(nullptr);
                pflechaIzquierda->update(nullptr);
                break;
            case DESVANECER_FULL_OUT:
                break;
            case CAER_IN:
                if(delay++ > max_delay){
                    mRectFondo.y += 1;
                    delay = 0;
                    if(mRectFondo.y >= y_inicial){
                        mRectFondo.y = y_inicial;
                        tipoAnimacionEjecutando = NINGUNA;
                    }
                }
                break;
        }
    }

    void draw(SDL_Renderer *gRenderer) override {
        switch(tipoAnimacionEjecutando){
            case NINGUNA:
                MenuContextual::draw(gRenderer);
                break;
            case DESVANECER_FULL_IN:
                MenuContextual::draw(gRenderer);
                break;
            case DESVANECER_FULL_OUT:
                break;
            case CAER_IN:
                MenuContextual::drawFondo(gRenderer);
                break;
            case CAER_OUT:break;
        }
    }

    ~MenuContextualAnimado(){
        //delete mpTextureBufferTarget;
    }


private:


    TipoAnimacion tipoAnimacionEjecutando = NINGUNA;

    int delay;
    int max_delay;
    int y_inicial;
    //SDL_Texture *mpTextureBufferTarget;
    //GameManagerInterfazUI *pgameManagerInterfazUI;
    SDL_Renderer *mGRenderer = nullptr;
    int alpha_step;
    int alpha_actual;
};
#endif //TETRIS_MENUCONTEXTUALANIMADO_HPP
