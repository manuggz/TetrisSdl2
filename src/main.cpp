#include "Menu.hpp"
#include "engine/GameManager/GameManager.hpp"


int main(int argc, char *argv[]){

    GameManager juego ("Tetris","",1024,768,false);
    //SDL_Log("RUTA 2: %s",juego.obtenerPrefPath().c_str());
    juego.cambiarInterfaz(new Menu(&juego));
    juego.run();

    return EXIT_SUCCESS;
}
