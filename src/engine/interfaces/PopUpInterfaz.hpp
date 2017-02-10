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

    PopUpInterfaz(GameManagerPopUpInterfaz * gameManager,int ID);
    /**
     * Este metodo se encarga de Iniciar la Interfaz, al igual que los anteriores métodos.
     * Este metodo debe usarse para dividir el inicio de variables y elementos del juego.
     */
    virtual void start(){mIsStarted=true;};

    /**
     * Dice si la interfaz está pausada.
     * Lo cual puede suceder debido a que se está mostrando un PopUp al usuario, o que hay otra interfaz en el top de la pila.
     * @return
     */
    virtual bool isPaused(){return mIsPaused&&mIsStarted&&!mIsStopped;};

    /**
     * Dice si se ha iniciado la interfaz.
     * Una interfaz se considera iniciada si ejecuta prepare()->createUI()->start()
     * @return
     */
    virtual bool isStarted(){return mIsStarted;};
    /**
     * Dice si la interfaz está detenida.
     * Una vez que la interfaz está detenida no se vuelve a ejecutar.
     * Cuando el GM detecta una interfaz detenida la elimina.
     * @return
     */
    virtual bool isStopped(){return mIsStopped;};
    virtual void pause(){mIsPaused = true;};
    virtual void stop(){mIsStopped = true;}; // La detiene marcandola  para eliminacion
    virtual void resume() {mIsPaused = false;};

    virtual void resultPopUp(InterfazEstandarBackResult * result, int idPopUp) {};
    virtual void prepare(){};
    virtual void update(){};
    virtual void createUI(SDL_Renderer * gRenderer){};
    virtual void procesarEvento(SDL_Event *event);
    int getID(){
        return ID;
    }

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
    int ID;
};
#endif //BOMBERMAN_POPUPMESSAGE_HPP
