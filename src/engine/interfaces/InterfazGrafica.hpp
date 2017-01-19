#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include "GameManagerInterfazUI.hpp"

/**
 * Clase base para todas las interfaces mostradas al usuario.
 * Maneja estados como Pausado/Comenzado.
 *
 * Los metodos son llamados por el GameManager, algunos siguen un Orden.
 *
 * El Orden de llamada desde el GameManager es:
 *
 * prepare()
 * createUI()
 * start()
 *
 * Cuando un PopUp se va a mostrar o se agrega una interfaz al stack se llama a pause().
 *
 * Cuando se establece un root en el GM o se hace back en el GM se llama a stop.
 */

class InterfazGrafica {
public:
    /**
     * Establece una referencia a un GameManager.
     * @param gameManagerInterfaz Instancia de un GameManager que implemente la interfaz requerida
     */
    InterfazGrafica(GameManagerInterfazUI * gameManagerInterfaz){
        mGameManagerInterfaz = gameManagerInterfaz;
    }
    /**
     * Se encarga de establecer valores de variables iniciales de la Interfaz
     * Este metodo se ejecuta antes de createUI, por lo que puede usarse para obtener datos necesarios para la
     * creacion de la UI
     */
    virtual void prepare(){};
    /**
     * Este metodo se encarga de crear la UI necesaria/inicial de la interfaz.
     * En algunos juegos no es necesario establecer UI en este metodo, sino dibujarlo tod
     * cuando se llama a InterfazUI::draw
     *
     * Sin embargo, crear los elementos de la UI por adelantado ayuda en la utilización de dirty rects y parecidos.
     * @param gRenderer
     */
    virtual void createUI(SDL_Renderer * gRenderer){};

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

    virtual void procesarEvento(SDL_Event * pEvento) {
        if(pEvento->type==SDL_KEYDOWN) {
            switch (pEvento->key.keysym.sym) {
                case SDLK_ESCAPE:
                    mGameManagerInterfaz->goBack();
                    break;
                default:break;
            }
        }

    };
    virtual void update(){};
    virtual void updateWhenPopUp(){};
    virtual void resultPopUp(InterfazEstandarBackResult * result, int idPopUp) {};
    virtual void draw(SDL_Renderer * gRenderer) = 0;

    virtual ~InterfazGrafica(){};

    // Esta funcion dibuja todos los elementos sin importar si fueron o no modificados anteriormente
    virtual void forceDraw() {}

    virtual void resultInterfazAnterior(int id, InterfazEstandarBackResult *pResult) {

    }

protected:

    GameManagerInterfazUI * mGameManagerInterfaz;

    bool mIsPaused = false;
    bool mIsStarted = false;

    bool mIsStopped = false;

};

#endif
