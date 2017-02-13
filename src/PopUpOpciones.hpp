//
// Created by manuggz on 10/02/17.
//

#ifndef TETRIS_POPUPOPCIOCIONES_HPP
#define TETRIS_POPUPOPCIOCIONES_HPP

#include <iostream>
#include "engine/interfaces/PopUpInterfaz.hpp"
#include "MenuContextual.hpp"
#include "engine/util/CMetaData.hpp"
#include "PopUpCambiarNick.hpp"
#include "PopUpConfigurarTeclas.hpp"
#include "ConfiguracionSingleton.hpp"

static const int ID_OPTION_MENU_MUSIC = 1;

static const int ID_OPTION_MENU_SFX = 2;

static const int ID_OPTION_CHANGE_NICK = 3;

static const int ID_OPTION_CHANGE_KEYS = 4;


static const int ID_POPUP_CHANGE_NICK = 2;
static const int ID_POPUP_CONFIGURAR_TECLAS = 3;

class PopUpOpciones : public PopUpInterfaz, public ContextMenuParent {

public:

    /**
     * Inicia la clase
     * @param mensaje MEnsaje a mostrar
     * @param tiempoDeMuestraSegundos  segundos que durará el mensaje
     */
    PopUpOpciones(GameManagerPopUpInterfaz *gameManager,int ID) : PopUpInterfaz(gameManager,ID) {
    }

    void prepare() override {
        configuracionSingleton = ConfiguracionSingletonManager::obtenerConfiguracion();

    }

    void createUI(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::createUI(gRenderer);

        menu = new MenuContextual(this, 310, 270);
        mSfxChangeSelect = new EfectoSonido("resources/music/SFX_PieceLockdown.ogg",MIX_MAX_VOLUME);

        menu->agregarOpcionMenu("Music:", ID_OPTION_MENU_MUSIC, (configuracionSingleton->obtenerBooleano(ID_IS_MUSIC_ON,true))?"ON":"OFF");
        menu->agregarOpcionMenu("Sfx:", ID_OPTION_MENU_SFX, (configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))?"ON":"OFF");
        menu->agregarOpcionMenu("Change Nick", ID_OPTION_CHANGE_NICK);
        menu->agregarOpcionMenu("Change Keys", ID_OPTION_CHANGE_KEYS);

        menu->establecerFlechaHaciaAfuera(ID_OPTION_MENU_MUSIC,true);
        menu->establecerFlechaHaciaAfuera(ID_OPTION_MENU_SFX,true);

        menu->establecerFuente(new BitmapFont(gRenderer,"resources/fuentes/tetris_fuente_1_32_blanca.png"),
                               MenuContextual::FUENTE_NORMAL);
        menu->establecerFuente(new BitmapFont(gRenderer,"resources/fuentes/tetris_fuente_1_32_amarilla.png"),
                               MenuContextual::FUENTE_RESALTADA);


        LTexture *textureFlechaDerecha = new LTexture();
        textureFlechaDerecha->cargarDesdeArchivo("resources/images/tetris_flecha_opcion_seleccionada_dir_derecha.png",
                                                 gRenderer, false);
        menu->establecerTexturaFlechaDerecha(textureFlechaDerecha);

        LTexture *textureFlechaIzquierda = new LTexture();
        textureFlechaIzquierda->cargarDesdeArchivo("resources/images/tetris_flecha_opcion_seleccionada_dir_izquierda.png",
                                                   gRenderer, false);
        menu->establecerTexturaFlechaIzquierda(textureFlechaIzquierda);

        menu->createUI(gRenderer);
        LTexture * textureFondoMenu = new LTexture();
        textureFondoMenu->cargarDesdeArchivo("resources/images/menuopciones_placeholder.png",gRenderer,false);
        menu->establecerTexturaFondo(textureFondoMenu);
        menu->establecerResaltado(ID_OPTION_MENU_MUSIC);
    }


    void procesarEvento(SDL_Event *event) override {
        PopUpInterfaz::procesarEvento(event);
        menu->procesarEvento(event);
    }

    void eventoFlechaPresionadaMenuContextual(int idOpcionResaltadaActual) override {
        switch(idOpcionResaltadaActual){
            case ID_OPTION_MENU_MUSIC:
                configuracionSingleton->guardarBooleano(ID_IS_MUSIC_ON,!configuracionSingleton->obtenerBooleano(ID_IS_MUSIC_ON,true));
                menu->establecerRightTextoOpcionMenu(ID_OPTION_MENU_MUSIC,configuracionSingleton->obtenerBooleano(ID_IS_MUSIC_ON,true)?"ON":"OFF");
                break;
            case ID_OPTION_MENU_SFX:
                configuracionSingleton->guardarBooleano(ID_IS_SFX_ON,!configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true));
                menu->establecerRightTextoOpcionMenu(ID_OPTION_MENU_SFX,configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true)?"ON":"OFF");
                break;
        }
        if(!configuracionSingleton->obtenerBooleano(ID_IS_MUSIC_ON,true)){
            Mix_PauseMusic();
        }else{
            Mix_ResumeMusic();
        }
    }


    void opcionSeleccionadaMenuContextual(int idOpcionSeleccionada) override {
        switch(idOpcionSeleccionada){
            case ID_OPTION_CHANGE_NICK:{
                PopUpCambiarNick * popUpCambiarNick = new PopUpCambiarNick(mGameManager, ID_POPUP_CHANGE_NICK);
                mGameManager->showPopUp(popUpCambiarNick);
                }
                break;
            case ID_OPTION_CHANGE_KEYS:{
                PopUpConfigurarTeclas * popUpConfigurarTeclas = new PopUpConfigurarTeclas(mGameManager, ID_POPUP_CONFIGURAR_TECLAS);
                mGameManager->showPopUp(popUpConfigurarTeclas);

            }
                break;
        }
    }
    void cambiadoResaltadoActualMenuContextual(int id) override {
        if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxChangeSelect->play();
    }

    void update() override {
        PopUpInterfaz::update();

        menu->update();
    }

    void draw(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::draw(gRenderer);

        menu->draw(gRenderer);
    }

    ~PopUpOpciones() {
        delete menu;
        delete mSfxChangeSelect;
    }

protected:
    MenuContextual  * menu;
    //MetaData *mMetaData;
    ConfiguracionSingleton * configuracionSingleton;
    EfectoSonido * mSfxChangeSelect = nullptr;
};
#endif //TETRIS_POPUPOPCIOCIONES_HPP
