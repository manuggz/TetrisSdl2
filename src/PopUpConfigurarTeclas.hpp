//
// Created by manuggz on 10/02/17.
//

#ifndef TETRIS_POPUPCONFIGURARTECLAS_HPP
#define TETRIS_POPUPCONFIGURARTECLAS_HPP

static const int ID_TECLA_LEFT = 0;

static const int ID_TECLA_RIGHT = 1;

static const int ID_TECLA_ROTA_LEFT = 2;

static const int ID_TECLA_ROTA_RIGHT = 3;

static const int ID_TECLA_PAUSE = 4;

static const int ID_TECLA_SOFT_DROP = 5;

static const int ID_TECLA_HARD_DROP = 6;

#include <iostream>
#include "engine/interfaces/PopUpInterfaz.hpp"
#include "MenuContextual.hpp"
#include "engine/util/CMetaData.hpp"
#include "PopUpCambiarNick.hpp"
#include "ControlTeclasPlayer.hpp"
#include "PopUpCapturarTecla.hpp"


class PopUpConfigurarTeclas : public PopUpInterfaz, public ContextMenuParent {

public:

    /**
     * Inicia la clase
     * @param mensaje MEnsaje a mostrar
     * @param tiempoDeMuestraSegundos  segundos que durará el mensaje
     */
    PopUpConfigurarTeclas(GameManagerPopUpInterfaz *gameManager, int ID) : PopUpInterfaz(gameManager, ID) {
    }

    void prepare() override {
        controlTeclasPlayer = new ControlTeclasPlayer();
        controlTeclasPlayer->cargarDesdeArchivo(mGameManager->obtenerPrefPath() + NOMBRE_FILE_TECLAS_PLAYER1);
        configuracionSingleton = ConfiguracionSingletonManager::obtenerConfiguracion();
    }

    void actualizarNombresTeclas(){
        for(int i = 0; i < ControlTeclasPlayer::N_TECLAS;i++){
            actualizarNombreTeclaEnGUI(i);

        }
    }
    void cambiadoResaltadoActualMenuContextual(int id) override {
        if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxChangeSelect->play();
    }

    void createUI(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::createUI(gRenderer);

        LTexture *textureFondoMenu = new LTexture();
        textureFondoMenu->cargarDesdeArchivo("resources/images/menu_cambiar_teclas_placeholder.png", gRenderer, false);
        mSfxChangeSelect = new EfectoSonido("resources/music/SFX_PieceLockdown.ogg",MIX_MAX_VOLUME);

        SDL_Rect rect_screen = mGameManager->getRectScreen();
        menu = new MenuContextual(
                this,
                rect_screen.w/2 - textureFondoMenu->getWidth()/2,
                rect_screen.h/2 - textureFondoMenu->getHeight()/2
        );

        // Notar que deben estar en el orden declarado en el enum ControlTeclasPlayer::TeclaControlPlayer
        menu->agregarOpcionMenu("Left:", ID_TECLA_LEFT);
        menu->agregarOpcionMenu("Right:", ID_TECLA_RIGHT);
        menu->agregarOpcionMenu("R.Left:", ID_TECLA_ROTA_LEFT);
        menu->agregarOpcionMenu("R.Right:", ID_TECLA_ROTA_RIGHT);
        menu->agregarOpcionMenu("Pause:", ID_TECLA_PAUSE);
        menu->agregarOpcionMenu("Soft Drop:", ID_TECLA_SOFT_DROP);
        menu->agregarOpcionMenu("Hard Drop:", ID_TECLA_HARD_DROP);
        actualizarNombresTeclas();


        menu->establecerFuente(new BitmapFont(gRenderer, "resources/fuentes/tetris_fuente_1_32_blanca.png"),
                               MenuContextual::FUENTE_NORMAL);
        menu->establecerFuente(new BitmapFont(gRenderer, "resources/fuentes/tetris_fuente_1_32_amarilla.png"),
                               MenuContextual::FUENTE_RESALTADA);


        LTexture *textureFlechaDerecha = new LTexture();
        textureFlechaDerecha->cargarDesdeArchivo("resources/images/tetris_flecha_opcion_seleccionada_dir_derecha.png",
                                                 gRenderer, false);
        menu->establecerTexturaFlechaDerecha(textureFlechaDerecha);

        LTexture *textureFlechaIzquierda = new LTexture();
        textureFlechaIzquierda->cargarDesdeArchivo(
                "resources/images/tetris_flecha_opcion_seleccionada_dir_izquierda.png",
                gRenderer, false);
        menu->establecerTexturaFlechaIzquierda(textureFlechaIzquierda);

        menu->createUI(gRenderer);
        menu->establecerTexturaFondo(textureFondoMenu);
        menu->establecerResaltado(ID_TECLA_LEFT);
    }


    void procesarEvento(SDL_Event *event) override {
        PopUpInterfaz::procesarEvento(event);
        menu->procesarEvento(event);

        if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
            controlTeclasPlayer->guardar(mGameManager->obtenerPrefPath() + NOMBRE_FILE_TECLAS_PLAYER1);
    }

    void actualizarNombreTeclaEnGUI(int i){
        static char nombre_tecla[20];

        if(controlTeclasPlayer->isBotonJoystick((ControlTeclasPlayer::TeclaControlPlayer)i))
            sprintf(nombre_tecla,"joy %d",controlTeclasPlayer->getJoybuttonMapping((ControlTeclasPlayer::TeclaControlPlayer)i) + 1);
        else if(controlTeclasPlayer->isDireccionJoystick(i))
            sprintf(nombre_tecla,"joy %s",SDL_GetScancodeName(controlTeclasPlayer->getKeyboardMapping((ControlTeclasPlayer::TeclaControlPlayer)i)));
        else
            sprintf(nombre_tecla,"Tecla %s",SDL_GetScancodeName(controlTeclasPlayer->getKeyboardMapping((ControlTeclasPlayer::TeclaControlPlayer)i)));

        menu->establecerRightTextoOpcionMenu(i,nombre_tecla);

    }

    void resultPopUp(InterfazEstandarBackResult *result, int idPopUp) override {
        actualizarNombreTeclaEnGUI(idPopUp);
    }

    void opcionSeleccionadaMenuContextual(int idOpcionSeleccionada) override {

        PopUpCapturarTecla * popUpCapturarTecla = new PopUpCapturarTecla(
                mGameManager,
                idOpcionSeleccionada,
                controlTeclasPlayer,
                (ControlTeclasPlayer::TeclaControlPlayer) idOpcionSeleccionada
        );
        mGameManager->showPopUp(popUpCapturarTecla);
    }

    void update() override {
        PopUpInterfaz::update();

        menu->update();
    }

    void draw(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::draw(gRenderer);

        menu->draw(gRenderer);
    }

    ~PopUpConfigurarTeclas() {
        delete menu;
        delete controlTeclasPlayer;
        delete mSfxChangeSelect;
    }

protected:
    MenuContextual *menu;
    //MetaData *configuracionSingleton;
    ControlTeclasPlayer *controlTeclasPlayer;
    EfectoSonido * mSfxChangeSelect = nullptr;
    ConfiguracionSingleton * configuracionSingleton;
};
#endif //TETRIS_POPUPCONFIGURARTECLAS_HPP
