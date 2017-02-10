//
// Created by manuggz on 10/01/17.
//

#ifndef BOMBERMAN_GAMEMANAGERPOPUPINTERFAZ_HPP
#define BOMBERMAN_GAMEMANAGERPOPUPINTERFAZ_HPP

#include <string>
#include <SDL2/SDL.h>
class InterfazEstandarBackResult;
class PopUpInterfaz;
class GameManagerPopUpInterfaz{
public:
    /**
     * Hace que se muestre un PopUp en la pantalla
     * @param popUpInterfaz Puntero al PopUp
     * @param codePopUp Codigo que usa el PopUp
     *
     * @note Cuando se cierra el PopUp, se pasa el codePopUp a la interfaz que esté corriendo
     */
    virtual void showPopUp(PopUpInterfaz * popUpInterfaz) = 0;
    virtual void closePopUp(InterfazEstandarBackResult * result = nullptr)= 0;
    virtual SDL_Rect getRectScreen()=0;
    virtual std::string obtenerPrefPath()  = 0;

};
#endif //BOMBERMAN_GAMEMANAGERDEFINICION_HPP
