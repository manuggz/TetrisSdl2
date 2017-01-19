//
// Created by manuggz on 27/11/16.
//

#ifndef BOMBERMAN_TILEBLOQUEROMPIBLEANIMADO_HPP
#define BOMBERMAN_TILEBLOQUEROMPIBLEANIMADO_HPP

#include "../sprites/animacion/animacion.hpp"

class TileBloqueRompibleAnimado: public Animacion{
public:
    TileBloqueRompibleAnimado(SpriteSheet *spriteSheet, string frames, int x, int y, int delayCambioFrame)
            : Animacion(spriteSheet, frames, x, y, delayCambioFrame) {
        mRepeticiones=-1;

    }
};
#endif //BOMBERMAN_TILEBLOQUEROMPIBLEANIMADO_HPP
