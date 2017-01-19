//
// Created by manuggz on 18/01/17.
//

#ifndef BOMBERMAN_TOAST_HPP
#define BOMBERMAN_TOAST_HPP
#include <SDL2/SDL.h>

#include "../interfaces/GameManagerInterfazUI.hpp"
#include "LTimer.hpp"

class Toast{
public:
    static const int TOAST_DURACION_CORTA = 1000;
    static const int TOAST_DURACION_MEDIA = 3000;
    static const int TOAST_DURACION_LARGA = 5000;

    Toast(GameManagerInterfazUI * pGameManagerInterfazUI,std::string mensaje,int duracion){
        mpGameManagerInterfaz = pGameManagerInterfazUI;
        Toast::mensaje  = mensaje;
        mDuracion = duracion;
    }

    virtual void createUI(SDL_Renderer * gRenderer) = 0;
    virtual void draw(SDL_Renderer * gRenderer) = 0;
    virtual ~Toast(){};

    void update(){
        if(timer.getTicks() >= mDuracion){
            stop();
        }
    }
    void prepare(){

    }
    /**
     * Este metodo se encarga de Iniciar la Interfaz, al igual que los anteriores métodos.
     * Este metodo debe usarse para dividir el inicio de variables y elementos del juego.
     */
    virtual void start(){mIsStarted=true;
        timer.start();
    };

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
    virtual void pause(){
        mIsPaused = true;
        timer.pause();
    };
    virtual void stop(){
        mIsStopped = true;
        timer.stop();
    }; // La detiene marcandola  para eliminacion
    virtual void resume() {
        mIsPaused = false;
        timer.resume();
    };

protected:
    GameManagerInterfazUI * mpGameManagerInterfaz;
    std::string mensaje;
    bool mIsPaused = false;
    bool mIsStarted = false;

    bool mIsStopped = false;

    int mDuracion = 0;
    LTimer timer;
};

#endif //BOMBERMAN_TOAST_HPP
