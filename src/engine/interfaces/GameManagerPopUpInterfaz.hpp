//
// Created by manuggz on 10/01/17.
//

#ifndef BOMBERMAN_GAMEMANAGERPOPUPINTERFAZ_HPP
#define BOMBERMAN_GAMEMANAGERPOPUPINTERFAZ_HPP

#include <SDL2/SDL.h>
class InterfazEstandarBackResult;
class GameManagerPopUpInterfaz{
public:
    virtual void closePopUp(InterfazEstandarBackResult * result = nullptr)= 0;
    virtual SDL_Rect getRectScreen()=0;
};
#endif //BOMBERMAN_GAMEMANAGERDEFINICION_HPP
