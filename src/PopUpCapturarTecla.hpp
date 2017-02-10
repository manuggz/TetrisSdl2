//
// Created by manuggz on 10/02/17.
//

#ifndef TETRIS_POPUPCAPTURARTECLA_HPP
#define TETRIS_POPUPCAPTURARTECLA_HPP
#include <iostream>
#include "engine/interfaces/PopUpInterfaz.hpp"
#include "engine/util/CMetaData.hpp"
#include "engine/util/BitmapFont.hpp"
#include "engine/GameManager/GameManager.hpp"


class PopUpCapturarTecla : public PopUpInterfaz{

public:

    /**
     * Inicia la clase
     * @param mensaje MEnsaje a mostrar
     * @param tiempoDeMuestraSegundos  segundos que durará el mensaje
     */
    PopUpCapturarTecla(GameManagerPopUpInterfaz *gameManager,
                       int ID,
                     ControlTeclasPlayer * pTeclasPlayer,
                       ControlTeclasPlayer::TeclaControlPlayer teclaControlPlayer) : PopUpInterfaz(gameManager,ID) {
        PopUpCapturarTecla::pTeclasPlayer = pTeclasPlayer;
        PopUpCapturarTecla::teclaControlEditando = teclaControlPlayer;

    }

    void prepare() override {

    }

    void createUI(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::createUI(gRenderer);

        SDL_Rect rect_screen = mGameManager->getRectScreen();

        pTextureFondo = new LTexture();
        pTextureFondo->cargarDesdeArchivo("resources/images/insertenueva_tecla_placeholder.png",gRenderer,false);

        x_dibujo = rect_screen.w/2 - pTextureFondo->getWidth()/2;
        y_dibujo = rect_screen.h/2 - pTextureFondo->getHeight()/2;

        pBFTextPresioneNuevaTecla = new BitmapFont(gRenderer,"resources/fuentes/tetris_fuente_1_32_amarilla.png");
        //pBFTextNickPlayer    = new BitmapFont(gRenderer,"resources/fuentes/tetris_fuente_1_32_roja.png");

        pBFRTextPresioneNuevaTecla = new BitmapFontRenderer(pBFTextPresioneNuevaTecla,0,y_dibujo + 30);
        pBFRTextPresioneNuevaTecla->setCenterX(rect_screen.w/2);
        pBFRTextPresioneNuevaTecla->setText("Press a new key!!");

    }

    void procesarEvento(SDL_Event *event) override {
        if(event->type == SDL_JOYBUTTONDOWN){
            if(event->jbutton.type == SDL_JOYBUTTONDOWN){

                pTeclasPlayer->setJoybuttonMapping(teclaControlEditando,event->jbutton.button);
                pTeclasPlayer->setIsBotonJoystick(teclaControlEditando,true);
                pTeclasPlayer->setIsDireccionJoystick((int)teclaControlEditando,false);

                SDL_Joystick * joystick = SDL_JoystickFromInstanceID(event->jbutton.which);
                //SDL_Log("Joystick Boton %d Presionado.",event->jbutton.button);
                SDL_JoystickGUID guidj = SDL_JoystickGetGUID(joystick);
                char temp[33];
                SDL_JoystickGetGUIDString(guidj,temp,33);
                pTeclasPlayer->setJoystickGUID(teclaControlEditando,temp);
                mGameManager->closePopUp(nullptr);
            }

        }else if(event->type == SDL_JOYAXISMOTION){
            bool modificado=false;
            if(event->jaxis.axis == 0) {
                if(event->jaxis.value > 0){
                    pTeclasPlayer->setKeyboardMapping(teclaControlEditando, SDL_SCANCODE_RIGHT);
                    modificado=true;
                }

                else if(event->jaxis.value < 0){
                    pTeclasPlayer->setKeyboardMapping(teclaControlEditando, SDL_SCANCODE_LEFT);
                    modificado=true;
                }
            } else {
                if(event->jaxis.value > 0){
                    pTeclasPlayer->setKeyboardMapping(teclaControlEditando, SDL_SCANCODE_DOWN);
                    modificado=true;
                }else if(event->jaxis.value < 0){
                    pTeclasPlayer->setKeyboardMapping(teclaControlEditando, SDL_SCANCODE_UP);
                    modificado=true;
                }
            }

            if(modificado){
                pTeclasPlayer->setIsBotonJoystick(teclaControlEditando,false);
                pTeclasPlayer->setIsDireccionJoystick(teclaControlEditando,true);

                SDL_Joystick * joystick = SDL_JoystickFromInstanceID(event->jbutton.which);
                SDL_JoystickGUID guidj = SDL_JoystickGetGUID(joystick);
                char temp[33];
                SDL_JoystickGetGUIDString(guidj,temp,33);
                pTeclasPlayer->setJoystickGUID(teclaControlEditando,temp);
                mGameManager->closePopUp(nullptr);
            }
        }else if(event->type==SDL_KEYDOWN){
            if(event->key.keysym.sym != SDLK_ESCAPE){
                pTeclasPlayer->setKeyboardMapping(teclaControlEditando, event->key.keysym.scancode);
                pTeclasPlayer->setIsBotonJoystick(teclaControlEditando,false);;
                pTeclasPlayer->setIsDireccionJoystick(teclaControlEditando,false);
            }
            mGameManager->closePopUp(nullptr);
        }
    }


    void draw(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::draw(gRenderer);
        pTextureFondo->draw(gRenderer,x_dibujo,y_dibujo);
        pBFRTextPresioneNuevaTecla->draw(gRenderer);
    }

    ~PopUpCapturarTecla() {
        delete pBFRTextPresioneNuevaTecla;
        delete pBFTextPresioneNuevaTecla;
        delete pTextureFondo;
    }

protected:
    LTexture * pTextureFondo;
    BitmapFontRenderer * pBFRTextPresioneNuevaTecla = nullptr;

    BitmapFont * pBFTextPresioneNuevaTecla = nullptr;

    int x_dibujo;
    int y_dibujo;

    ControlTeclasPlayer *pTeclasPlayer;
    ControlTeclasPlayer::TeclaControlPlayer teclaControlEditando;
};

#endif //TETRIS_POPUPCAPTURARTECLA_HPP
