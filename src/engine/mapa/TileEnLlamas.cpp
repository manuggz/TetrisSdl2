#include "TileEnLlamas.hpp"


TileEnLlamas::TileEnLlamas(SpriteSheet *spriteSheet, std::string frames, int x, int y, int delayCambioFrame) : Animacion(
        spriteSheet, frames, x, y, delayCambioFrame) {
    //cout << "frames"<<frames<<endl;
}
