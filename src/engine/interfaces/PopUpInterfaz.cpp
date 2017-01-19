//
// Created by manuggz on 09/01/17.
//
#include "PopUpInterfaz.hpp"

PopUpInterfaz::PopUpInterfaz(GameManagerPopUpInterfaz * gameManager){
    mGameManager = gameManager;
    mIsPaused = false;
    mIsStarted = false;
}

void PopUpInterfaz::start(){
    mIsStarted=true;
}
bool PopUpInterfaz::isPaused(){
    return mIsPaused&&mIsStarted&&!mIsStopped;
}
bool PopUpInterfaz::isStarted(){
    return mIsStarted;
}
bool PopUpInterfaz::isStopped(){
    return mIsStopped;
}
void PopUpInterfaz::stop(){
    mIsStopped = true;
} // La detiene marcandola  para eliminacion
void PopUpInterfaz::procesarEvento(SDL_Event *event) {
    switch(event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_ESCAPE:
                    mGameManager->closePopUp(nullptr);
                    break;
            }
    }
}

/**
 * Oscurece el fondo del PopUp
 * @param gRenderer
 */
void PopUpInterfaz::draw(SDL_Renderer *gRenderer) {
    SDL_SetRenderDrawBlendMode(gRenderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer,mpColorOscurecer.r,mpColorOscurecer.g,mpColorOscurecer.b,mpColorOscurecer.a);
    SDL_RenderFillRect(gRenderer, nullptr);
}
