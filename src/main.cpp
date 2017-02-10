#include "engine/GameManager/GameManager.hpp"
#include "MenuPrincipal.hpp"


int main(int argc, char *argv[]){

    GameManager juego ("Tetris","",1024,768,false);
    //SDL_Log("RUTA 2: %s",juego.obtenerPrefPath().c_str());
    juego.cambiarInterfaz(new MenuPrincipal(&juego));
    juego.run();

    return EXIT_SUCCESS;
}
