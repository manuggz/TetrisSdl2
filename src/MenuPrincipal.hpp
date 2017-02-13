//
// Created by manuggz on 19/01/17.
//

#ifndef TETRIS_MENUPRINCIPAL_HPP
#define TETRIS_MENUPRINCIPAL_HPP

static const int ID_POP_UP_OPCIONES = 1;

#include "engine/interfaces/InterfazGrafica.hpp"
#include "engine/util/LTexture.hpp"
#include "engine/util/BitmapFont.hpp"
#include "engine/util/MusicaFondo.hpp"
#include "engine/util/EfectoSonido.hpp"
#include "TetrisInterfaz.hpp"
#include "FlechaDinamica.hpp"
#include "MenuContextualAnimado.hpp"
#include "PopUpOpciones.hpp"
#include "ConfiguracionSingleton.hpp"

class MenuPrincipal: public  InterfazGrafica, public ContextMenuParent {

public:

    MenuPrincipal(GameManagerInterfazUI *gameManagerInterfaz) : InterfazGrafica(gameManagerInterfaz) {
    }

    void prepare() override {
        InterfazGrafica::prepare();
        ConfiguracionSingletonManager::create(mGameManagerInterfaz->obtenerPrefPath() + NOMBRE_FILE_CONFIG);
        configuracionSingleton = ConfiguracionSingletonManager::obtenerConfiguracion();
    }

    void createUI(SDL_Renderer *gRenderer) override {
        InterfazGrafica::createUI(gRenderer);

        mTextureFondo = new LTexture();
        mTextureFondo->cargarDesdeArchivo("resources/images/background_menu_principal.png",gRenderer,false);

        mMusicaFondo = new MusicaFondo("resources/music/8_bit_electro_house_remix.wav");
        mSfxChangeSelect = new EfectoSonido("resources/music/SFX_PieceLockdown.ogg",MIX_MAX_VOLUME);


        // Recursos para el menu
        menu = new MenuContextualAnimado(this,310,270,220,600);

        menu->agregarOpcionMenu("CONTINUE",CONTINUE);
        menu->agregarOpcionMenu("NEW GAME",NEW_GAME);
        menu->agregarOpcionMenu("SCORES",SCORES);
        menu->agregarOpcionMenu("OPTIONS",OPTIONS);
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
        menu->establecerResaltado(NEW_GAME);
        menu->establecerVisilidad(CONTINUE,false);
        menu->establecerVisilidad(SCORES,false);
    }

    void start() override {
        InterfazGrafica::start();
        menu->open(MenuContextualAnimado::DESVANECER_FULL_IN);
    }

    void resume() override {
        InterfazGrafica::resume();
        if(configuracionSingleton->obtenerBooleano(ID_IS_MUSIC_ON,true)){
            mMusicaFondo->play();
        }
        SDL_ShowCursor(SDL_DISABLE);
    }

    void procesarEvento(SDL_Event *event) override {
        InterfazGrafica::procesarEvento(event);
        menu->procesarEvento(event);
    }
    void opcionSeleccionadaMenuContextual(int id) override {
        switch (id) {
            case CONTINUE:
                break;
            case NEW_GAME:
                mGameManagerInterfaz->cambiarInterfaz(new TetrisInterfaz(mGameManagerInterfaz));
                break;
            case SCORES:
                break;
            case OPTIONS:{
                PopUpOpciones * popUpOpciones = new PopUpOpciones(mGameManagerInterfaz,ID_POP_UP_OPCIONES);
                mGameManagerInterfaz->showPopUp(popUpOpciones);
            }
                break;
        }
    }

    void resultPopUp(InterfazEstandarBackResult *result, int idPopUp) override {
        switch(idPopUp){
            case ID_POP_UP_OPCIONES:
                break;
        }
    }

    void cambiadoResaltadoActualMenuContextual(int id) override {
        if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxChangeSelect->play();
    }

    void update() override {
        InterfazGrafica::update();
        menu->update();
    }

    void draw(SDL_Renderer *gRenderer) override {

        mTextureFondo->draw(gRenderer,0,0);
        menu->draw(gRenderer);
    }

    ~MenuPrincipal(){
        delete mTextureFondo;
        delete mMusicaFondo;
        delete mSfxChangeSelect;
        delete menu;
        configuracionSingleton->guardar();
        ConfiguracionSingletonManager::destroy();
    }
private:

    //int xIni = 0,yIni = 0;

    LTexture * mTextureFondo = nullptr;

    typedef enum {
        CONTINUE,
        NEW_GAME,
        SCORES,
        OPTIONS,
    }OpcionesMenu;

    //BitmapFont * mpBitmapFont[2]  {nullptr};

    //bool mEsOpcionVisible[N_OPCIONES] {false};
    //BitmapFontRenderer * mpBitFntRendOpsMenuPausa[N_OPCIONES] {nullptr};

    //LTexture * mpTextureFlechaOpcionSeleccionada = nullptr;

    //OpcionesMenu mOpcionSeleccionadaMenuPausa;

    MusicaFondo * mMusicaFondo = nullptr;

    EfectoSonido * mSfxChangeSelect = nullptr;
    //LTexture * mpTextureCuadroAzulFondo = nullptr;
    MenuContextualAnimado  * menu;
    ConfiguracionSingleton *configuracionSingleton;
};

#endif //TETRIS_MENUPRINCIPAL_HPP
