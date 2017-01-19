//
// Created by manuggz on 25/11/16.
//

#ifndef BOMBERMAN_MOSTRARMENSAJETEXTO_HPP
#define BOMBERMAN_MOSTRARMENSAJETEXTO_HPP

#include <iostream>
#include "PopUpInterfaz.hpp"
#include "../layout/LayoutManager/LayoutAbsolute.hpp"
#include "../layout/Componentes/LabelComponent.hpp"
#include "../util/LTimer.hpp"
#include "../GameManager/GameManager.hpp"

/**
 * PopUp que Muestra un Texto Centrado en la pantalla al usuario por cierto tiempo
 */
class PopUpMostrarMensajeTextoTimer : public PopUpInterfaz{

public:

    /**
     * Inicia la clase
     * @param mensaje MEnsaje a mostrar
     * @param tiempoDeMuestraSegundos  segundos que durará el mensaje
     */
    PopUpMostrarMensajeTextoTimer(GameManagerPopUpInterfaz *gameManager1, std::string mensaje,
                             Uint8 tiempoDeMuestraSegundos) : PopUpInterfaz(gameManager1) {
        mMensajeMostrar          = mensaje;
        mTiempoDeMuestraSegundos = tiempoDeMuestraSegundos;
    }

    void setText(std::string nuevoTexto){
        mMensajeMostrar = nuevoTexto;
        if(mpMensajeTexto != nullptr){
            mpMensajeTexto->setText(mMensajeMostrar);
        }
    }

    void setColorText(SDL_Color nuevoColor){
        mColorTexto = nuevoColor;
        if(mpMensajeTexto != nullptr){
            mpMensajeTexto->setTextColor(mColorTexto);
        }
    }

    void setSizeText(int nuevaSize) {
        mSizeText = nuevaSize;
        if(mpMensajeTexto != nullptr){
            mpMensajeTexto->setTextSize(mSizeText);
        }
    }
    void createUI(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::createUI(gRenderer);

        mLayoutParent   = new LayoutAbsolute();

        mpMensajeTexto = new LabelComponent();

        mpMensajeTexto->setText(mMensajeMostrar);
        mpMensajeTexto->setFont("data/fuentes/OpenSans-Bold.ttf",mSizeText);
        mpMensajeTexto->setTextColor(mColorTexto);
        mpMensajeTexto->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL,LAYOUT_PARAM_TRUE);
        mpMensajeTexto->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_VERTICAL,LAYOUT_PARAM_TRUE);

        mLayoutParent->addComponent(mpMensajeTexto);

        mpGameRenderer = gRenderer;
    }

    void packLayout(){
        SDL_Rect rect = mGameManager->getRectScreen();
        mLayoutParent->pack(mpGameRenderer);
        mLayoutParent->setRectDibujo(rect);
    }
    void start() override {
        PopUpInterfaz::start();
        mControlTimer.start();
    }

    void update() override {
        PopUpInterfaz::update();

        if(mControlTimer.getTicks()/1000 >= mTiempoDeMuestraSegundos){
            InterfazEstandarBackResult * resultado = new InterfazEstandarBackResult();
            resultado->texto = mMensajeMostrar;
            mGameManager->closePopUp(resultado);
        }
    }

    void draw(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::draw(gRenderer);

        if(mLayoutParent->isDisabled()){
            packLayout();
        }
        mLayoutParent->draw(gRenderer);
    }

    ~PopUpMostrarMensajeTextoTimer(){
        delete mLayoutParent;
    }
protected:
    std::string     mMensajeMostrar;
    LayoutAbsolute *mLayoutParent;
    LTimer mControlTimer;
    Uint8 mTiempoDeMuestraSegundos = 0;
    LabelComponent *mpMensajeTexto = nullptr;

    SDL_Color mColorTexto {255,255,0,255};
    int mSizeText = 15;
    SDL_Renderer *mpGameRenderer;
};
#endif //BOMBERMAN_MOSTRARMENSAJETEXTO_HPP
