//
// Created by manuggz on 10/02/17.
//

#ifndef TETRIS_POPUPCAMBIARNICK_HPP
#define TETRIS_POPUPCAMBIARNICK_HPP

#include <iostream>
#include "engine/interfaces/PopUpInterfaz.hpp"
#include "engine/util/CMetaData.hpp"
#include "engine/util/BitmapFont.hpp"
#include "ConfiguracionConstantes.hpp"
#include "ConfiguracionSingleton.hpp"


class PopUpCambiarNick : public PopUpInterfaz{

public:

    /**
     * Inicia la clase
     * @param mensaje MEnsaje a mostrar
     * @param tiempoDeMuestraSegundos  segundos que durará el mensaje
     */
    PopUpCambiarNick(GameManagerPopUpInterfaz *gameManager,int ID) : PopUpInterfaz(gameManager,ID) {
    }

    void prepare() override {
        configuracionSingleton = ConfiguracionSingletonManager::obtenerConfiguracion();
    }

    void createUI(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::createUI(gRenderer);

        SDL_Rect rect_screen = mGameManager->getRectScreen();

        pTextureFondo = new LTexture();
        pTextureFondo->cargarDesdeArchivo("resources/images/insertarnuevonick_placeholder.png",gRenderer,false);

        x_dibujo = rect_screen.w/2 - pTextureFondo->getWidth()/2;
        y_dibujo = rect_screen.h/2 - pTextureFondo->getHeight()/2;

        pBFTextInsertNewNick = new BitmapFont(gRenderer,"resources/fuentes/tetris_fuente_1_32_amarilla.png");
        pBFTextNickPlayer    = new BitmapFont(gRenderer,"resources/fuentes/tetris_fuente_1_32_roja.png");

        pBFRTextInsertNewNick = new BitmapFontRenderer(pBFTextInsertNewNick,0,y_dibujo + 30);
        pBFRTextInsertNewNick->setCenterX(rect_screen.w/2);
        pBFRTextInsertNewNick->setText("Insert New Nick");

        pBFRTextNickPlayer  = new BitmapFontRenderer(pBFTextNickPlayer,0,y_dibujo + pBFRTextInsertNewNick->getHeight() + 50);
        pBFRTextNickPlayer->setCenterX(rect_screen.w/2);

        textoActualIntroducido = configuracionSingleton->obtenerString(ID_NICK_NAME,DEFAULT_TEXT_NICK);

        pBFRTextNickPlayer->setText(textoActualIntroducido);


    }
    int toggleMayus(char c){
        if(isupper(c)){
            return tolower(c);
        }
        return toupper(c);
    }

    void procesarEvento(SDL_Event *event) override {
        PopUpInterfaz::procesarEvento(event);
        if(event->type == SDL_KEYDOWN){
            char keyName[10];
            if(((event->key.keysym.scancode>= SDL_SCANCODE_A &&event->key.keysym.scancode<= SDL_SCANCODE_0)||
                event->key.keysym.scancode == SDL_SCANCODE_PERIOD ||
                event->key.keysym.scancode == SDL_SCANCODE_KP_PERIOD
               )&&textoActualIntroducido.size() < mMaxCaracteres){
                sprintf(keyName,"%s",SDL_GetKeyName(event->key.keysym.sym));

                if(event->key.keysym.mod & KMOD_CAPS){
                    keyName[0] = toupper(keyName[0]);
                }else{
                    keyName[0] = tolower(keyName[0]);
                }

                if(event->key.keysym.mod & KMOD_LSHIFT ||
                   event->key.keysym.mod & KMOD_RSHIFT){
                    keyName[0] = toggleMayus(keyName[0]);
                }

                textoActualIntroducido += keyName[0];
                pBFRTextNickPlayer->setText(textoActualIntroducido);
            }else{
                switch(event->key.keysym.sym){
                    case SDLK_BACKSPACE:
                        if(!textoActualIntroducido.empty()) {
                            textoActualIntroducido.pop_back();
                            pBFRTextNickPlayer->setText(textoActualIntroducido);
                        }
                        break;
                    case SDLK_RETURN:
                        if(textoActualIntroducido.size() >= mMinCaracteres) {
                            //InterfazEstandarBackResult *resultado = new InterfazEstandarBackResult();
                            //resultado->texto = textoActualIntroducido;
                            configuracionSingleton->guardarString(ID_NICK_NAME,textoActualIntroducido);
                            mGameManager->closePopUp(nullptr);
                        }
                        break;
                }
            }

            //SDL_Log("",event->key.keysym.scancode);
        }
    }

    void update() override {
        PopUpInterfaz::update();
    }

    void draw(SDL_Renderer *gRenderer) override {
        PopUpInterfaz::draw(gRenderer);
        pTextureFondo->draw(gRenderer,x_dibujo,y_dibujo);
        pBFRTextInsertNewNick->draw(gRenderer);
        pBFRTextNickPlayer->draw(gRenderer);
    }

    ~PopUpCambiarNick() {
        delete pBFRTextInsertNewNick;
        delete pBFRTextNickPlayer;
        delete pBFTextInsertNewNick;
        delete pBFTextNickPlayer;
        delete pTextureFondo;
    }

protected:
    LTexture * pTextureFondo;
    BitmapFontRenderer * pBFRTextInsertNewNick = nullptr;
    BitmapFontRenderer * pBFRTextNickPlayer = nullptr;

    BitmapFont * pBFTextInsertNewNick = nullptr;
    BitmapFont * pBFTextNickPlayer    = nullptr;

    //MetaData *mMetaData;
    ConfiguracionSingleton * configuracionSingleton;
    std::string textNick;
    int x_dibujo;
    int y_dibujo;

    std::string textoActualIntroducido;
    unsigned long mMaxCaracteres = 12;
    unsigned long mMinCaracteres = 5;
};
#endif //TETRIS_POPUPCAMBIARNICK_HPP
