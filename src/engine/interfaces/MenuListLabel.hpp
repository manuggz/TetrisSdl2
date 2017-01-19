//
// Created by manuggz on 20/11/16.
//

#ifndef BOMBERMAN_MENULISTLABEL_HPP
#define BOMBERMAN_MENULISTLABEL_HPP

#include <SDL2/SDL.h>
#include "InterfazGrafica.hpp"
#include "../layout/LayoutManager/LayoutVertical.hpp"
#include "../layout/Componentes/LabelComponent.hpp"



/**
 * Representa un menu de opciones simple en el que el usuario se puede desplazar usando las teclas UP y DOWN
 *
 * Heredar esta clase y en el constructor agregar las opciones del menu en orden.
 * menuTextOptions.push_back("opcion i")
 *
 * Luego se ejecutara ejecutarAccionOpcionResaltada() cuando el usuario presione [ENTER].
 *
 * chequear la variable mOpcionMenuResaltadaActual para saber cual es la opcion resaltada actual.
 *
 * @example
 *
 * class MenuNuevoJuego : public MenuListLabel{
 * public:
 *    MenuNuevoJuego(GameManagerInterfazUI *game) : MenuListLabel(game) {
 *        mMenuOpcionesText.push_back("Historia");
 *        mMenuOpcionesText.push_back("Multiplayer");
 *        mMenuOpcionesText.push_back("Conexion");
 *    }
 *    void ejecutarAccionOpcionResaltada() {
 *        switch(mOpcionMenuResaltadaActual){
 *            case MENU_OPCION_HISTORIA:
 *                break;
 *            case MENU_OPCION_MULTIPLAYER:
 *                break;
 *            case MENU_OPCION_CONEXION:
 *                break;
 *            default:break;
 *        }
 *    }
 * private:
 *    typedef enum{
 *        MENU_OPCION_HISTORIA,
 *        MENU_OPCION_MULTIPLAYER,
 *        MENU_OPCION_CONEXION,
 *    }MenuOption;
 *
 *};
 *
 *
 */
class MenuListLabel : public InterfazGrafica{

public:

    MenuListLabel(GameManagerInterfazUI *gameManagerInterfaz);
    virtual void prepare() override;
    virtual void createUI(SDL_Renderer *renderer)  override;
    virtual void resume() override;
    void procesarEvento(SDL_Event *evento) override;
    void draw(SDL_Renderer *renderer) override;
    virtual ~MenuListLabel() override;

    void forceDraw() override;

protected:

    deque<LabelComponent  *> menuTextOptions;
    LayoutVertical * mLayout    = nullptr;
    LayoutVertical *mLayoutBackGround = nullptr;

    deque<string> mMenuOpcionesText;

    SDL_Color mColorLabelNormal    {255,0,0,255};
    SDL_Color mColorLabelResaltado {255,255,0,255};
    int mOpcionMenuResaltadaActual = -1;


    /**
    * Establece la nueva opcion del menu resaltada al usuario
    * @param nuevaOpcion Nueva seleccion
    * @return True en caso que se haya resaltado con exito, false en caso contrario
    */
    virtual bool setOpcionResaltada(int nuevaOpcion);

    virtual void ejecutarAccionOpcionResaltada() = 0;

};

#endif //BOMBERMAN_MENULISTLABEL_HPP
