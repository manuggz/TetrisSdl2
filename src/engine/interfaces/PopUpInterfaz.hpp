//
// Created by manuggz on 25/11/16.
//

#ifndef BOMBERMAN_POPUPMESSAGE_HPP
#define BOMBERMAN_POPUPMESSAGE_HPP

#include <SDL2/SDL.h>
#include "GameManagerPopUpInterfaz.hpp"

//class  GameManagerPopUpInterfaz;
/**
 * Esta clase define un Layout que se va a mostrar en pantalla.
 * Esta clase no es un manejador principal de la pantalla
 * Funciona parecido a un Modal en una Web.
 */
class PopUpInterfaz{
public:

    PopUpInterfaz(GameManagerPopUpInterfaz * gameManager);

    virtual void start();
    virtual bool isPaused();
    virtual bool isStarted();
    virtual bool isStopped();
    virtual void stop(); // La detiene marcandola  para eliminacion
    virtual void prepare(){};
    virtual void update(){};
    virtual void createUI(SDL_Renderer * gRenderer){};
    virtual void procesarEvento(SDL_Event *event);

    /**
     * Oscurece el fondo del PopUp
     * @param gRenderer
     */
    virtual void draw(SDL_Renderer *gRenderer);
    virtual ~PopUpInterfaz(){

    }

protected:
    SDL_Color mpColorOscurecer {0, 0, 0, 150};
    GameManagerPopUpInterfaz * mGameManager;
    bool mIsPaused = false;
    bool mIsStarted = false;

    bool mIsStopped = false;
};
#endif //BOMBERMAN_POPUPMESSAGE_HPP
