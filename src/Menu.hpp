//
// Created by manuggz on 06/12/16.
//

#ifndef TETRIS_MENU_HPP
#define TETRIS_MENU_HPP

#include "TetrisInterfaz.hpp"

class Menu:public  InterfazGrafica{

public:

    Menu(GameManagerInterfazUI *gameManagerInterfaz) : InterfazGrafica(gameManagerInterfaz) {

    }

    void prepare() override {
        InterfazGrafica::prepare();


    }

    void createUI(SDL_Renderer *gRenderer) override {
        InterfazGrafica::createUI(gRenderer);

        mTextureFondo = new LTexture();
        mTextureFondo->cargarDesdeArchivo("resources/background_menu_principal.png",gRenderer,false);


        for(int i = 1;i <= 2;i++){
            mpBitmapFont[i - 1] = new BitmapFont(gRenderer,"resources/fuentes/fuente_" + std::to_string(i) + ".png");
        }

        xIni = 400;
        yIni = 364;
        mpBitFntRendOpsMenuPausa[OpcionesMenu::CONTINUE] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::NORMAL],0,0);
        mpBitFntRendOpsMenuPausa[OpcionesMenu::CONTINUE]->setText("CONTINUE");

        mpBitFntRendOpsMenuPausa[OpcionesMenu::NEW_GAME] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::RESALTADO],0,0);
        mpBitFntRendOpsMenuPausa[OpcionesMenu::NEW_GAME]->setText("NEW GAME");
        mEsOpcionVisible[OpcionesMenu::NEW_GAME] = true;

        mpBitFntRendOpsMenuPausa[OpcionesMenu::SCORES] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::NORMAL],0,0);
        mpBitFntRendOpsMenuPausa[OpcionesMenu::SCORES]->setText("SCORES");

        mpBitFntRendOpsMenuPausa[OpcionesMenu::EXIT] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::NORMAL],0,0);
        mpBitFntRendOpsMenuPausa[OpcionesMenu::EXIT]->setText("EXIT");
        mEsOpcionVisible[OpcionesMenu::EXIT] = true;

        mOpcionSeleccionadaMenuPausa = OpcionesMenu ::NEW_GAME;

        mpTextureFlechaOpcionSeleccionada= new LTexture();
        mpTextureFlechaOpcionSeleccionada->cargarDesdeArchivo("resources/flecha_opcion_seleccionada.png",gRenderer,false);

        mpTextureCuadroAzulFondo = new LTexture();
        mpTextureCuadroAzulFondo->cargarDesdeArchivo("resources/cuadro_opcion_seleccionada.png",gRenderer,false);

        mMusicaFondo = cargar_musica("resources/music/8_bit_electro_house_remix.wav");

        mSfxChangeSelect = cargar_sonido((char *) "resources/music/SFX_PieceLockdown.ogg");
    }

    void start() override {
        InterfazGrafica::start();
        mGameManagerInterfaz->playSound(mMusicaFondo,MIX_MAX_VOLUME);
    }

    void resume() override {
        InterfazGrafica::resume();
        mGameManagerInterfaz->playSound(mMusicaFondo,MIX_MAX_VOLUME);
    }

    void procesarEvento(SDL_Event *event) override {
        InterfazGrafica::procesarEvento(event);
        if(event->type==SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
                case SDLK_ESCAPE:
                    mGameManagerInterfaz->goBack();
                    //mGameManager->showPopUp()
                    break;
                case SDLK_DOWN:
                {
                    int avance = 1;
                    while(mOpcionSeleccionadaMenuPausa + avance < OpcionesMenu::N_OPCIONES){
                        if(mEsOpcionVisible[mOpcionSeleccionadaMenuPausa + avance]){
                            mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[NORMAL]);
                            mOpcionSeleccionadaMenuPausa = (Menu::OpcionesMenu)(mOpcionSeleccionadaMenuPausa + avance);
                            mGameManagerInterfaz->play(mSfxChangeSelect);
                            mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[RESALTADO]);
                            break;
                        }else{
                            avance++;
                        }
                    }

                }

                    break;
                case SDLK_UP:
                {
                    int avance = 1;
                    while(mOpcionSeleccionadaMenuPausa - avance >= 0){
                        if(mEsOpcionVisible[mOpcionSeleccionadaMenuPausa - avance]){
                            mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[NORMAL]);
                            mOpcionSeleccionadaMenuPausa = (Menu::OpcionesMenu)(mOpcionSeleccionadaMenuPausa - avance);
                            mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[RESALTADO]);
                            mGameManagerInterfaz->play(mSfxChangeSelect);
                            break;
                        }else{
                            avance++;
                        }
                    }
                    //mGameManager->goBack();
                    //mGameManager->showPopUp(new PopUpMostrarMensajeText())

                }

                    break;
                case SDLK_RETURN:
                    switch(mOpcionSeleccionadaMenuPausa){
                        case CONTINUE:break;
                        case NEW_GAME:
                            mGameManagerInterfaz->cambiarInterfaz(new TetrisInterfaz(mGameManagerInterfaz));
                            break;
                        case SCORES:break;
                        case EXIT:
                            mGameManagerInterfaz->goBack();
                            break;
                        case N_OPCIONES:break;
                    }
                    //mGameManager->goBack();
                    //mGameManager->showPopUp(new PopUpMostrarMensajeText())
                    break;
                default:
                    break;
            }
        }
    }

    void update() override {
        InterfazGrafica::update();
    }

    void draw(SDL_Renderer *gRenderer) override {

        mTextureFondo->draw(gRenderer,0,0);

        int yDibujoOpcion = yIni;
        for(int i = 0 ; i < OpcionesMenu::N_OPCIONES;i++){
            if(mEsOpcionVisible[i]){

                if(mOpcionSeleccionadaMenuPausa == i){
                    mpTextureFlechaOpcionSeleccionada->draw(gRenderer,xIni - mpTextureFlechaOpcionSeleccionada->getWidth() - 5,yDibujoOpcion);
                    mpTextureCuadroAzulFondo->draw(gRenderer,xIni - 3,yDibujoOpcion);
                }

                mpBitFntRendOpsMenuPausa[i]->draw(gRenderer,xIni,yDibujoOpcion);

                yDibujoOpcion +=  mpBitFntRendOpsMenuPausa[i]->getHeight() + 10;

            }
        }
    }

    ~Menu(){
        delete mTextureFondo;

		for (int i = 0; i < N_OPCIONES; i++) {
			delete mpBitFntRendOpsMenuPausa[i];
		}

		for (int i = 0; i < 2; i++) {
			delete mpBitmapFont[i];
		}

		delete mpTextureFlechaOpcionSeleccionada;
        delete mpTextureCuadroAzulFondo;

        Mix_FreeMusic(mMusicaFondo);
        Mix_FreeChunk(mSfxChangeSelect);
    }
private:

    int xIni = 0,yIni = 0;

    LTexture * mTextureFondo = nullptr;

    enum EstadoOpcionMenu{
        NORMAL,
        RESALTADO
    };

    enum OpcionesMenu{
        CONTINUE,
        NEW_GAME,
        SCORES,
        EXIT,
        N_OPCIONES
    };

    BitmapFont * mpBitmapFont[2]  {nullptr};

    bool mEsOpcionVisible[N_OPCIONES] {false};
    BitmapFontRenderer * mpBitFntRendOpsMenuPausa[N_OPCIONES] {nullptr};

    LTexture * mpTextureFlechaOpcionSeleccionada = nullptr;

    OpcionesMenu mOpcionSeleccionadaMenuPausa;
    Mix_Music * mMusicaFondo = nullptr;

    Mix_Chunk * mSfxChangeSelect = nullptr;
    LTexture * mpTextureCuadroAzulFondo = nullptr;
};

#endif //TETRIS_MENU_HPP
