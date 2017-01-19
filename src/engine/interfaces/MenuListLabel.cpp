//
// Created by manuggz on 20/11/16.
//

#include "MenuListLabel.hpp"

MenuListLabel::MenuListLabel(GameManagerInterfazUI *gameManagerInterfaz)
        : InterfazGrafica(gameManagerInterfaz) {

}

void MenuListLabel::prepare() {
    InterfazGrafica::prepare();
    mLayoutBackGround          = new LayoutVertical();
    mLayout                    = new LayoutVertical();
}

void MenuListLabel::createUI(SDL_Renderer *renderer) {

    // Establecemos el fondo con los parametros adecados para que ocupe toda la pantalla
    mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_HEIGHT,LAYOUT_PARAM_TRUE);
    mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_WIDTH,LAYOUT_PARAM_TRUE);
    mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_WRAP_WIDTH,LAYOUT_PARAM_FALSE);
    mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_WRAP_HEIGHT,LAYOUT_PARAM_FALSE);
    mLayoutBackGround->setBackgroundColor(SDL_Color {0,0,0,255});

    //mLayoutBackGround->setBackgroundTexture(renderer,"data/imagenes/fondos/fondo_menu.bmp",false);

    LabelComponent * nuevoTextLabel;
    for(int i = 0 ; i < mMenuOpcionesText.size() ; i++){
        nuevoTextLabel = new LabelComponent();
        // TODO: INDEPENDIZAR ESTA PARTE E.E
        nuevoTextLabel->setFont("data/fuentes/OpenSans-BoldItalic.ttf",20);
        nuevoTextLabel->setTextColor(mColorLabelNormal);
        nuevoTextLabel->setText(mMenuOpcionesText[i]);
        menuTextOptions.push_back(nuevoTextLabel);
        mLayout->addComponent(nuevoTextLabel);
        nuevoTextLabel->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL,LAYOUT_PARAM_TRUE);
    }

    menuTextOptions[0]->setTextColor(mColorLabelResaltado);

    mLayoutBackGround->addComponent(mLayout);
    mLayout->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_HORIZONTAL,LAYOUT_PARAM_TRUE);
    mLayout->setLayoutParam(LAYOUT_PARAM_CENTER_PARENT_VERTICAL,LAYOUT_PARAM_TRUE);

    //packLayout(renderer);

    if(menuTextOptions.size()){
        mOpcionMenuResaltadaActual = 0;
    }
    SDL_ShowCursor(SDL_DISABLE);//ocultamos el cursor
    //mLayout->pack(renderer);
}


void MenuListLabel::resume() {
    InterfazGrafica::resume();
    mLayoutBackGround->setDisabled(true);
    SDL_ShowCursor(SDL_DISABLE);//ocultamos el cursor
}

/**
* Establece la nueva opcion del menu resaltada al usuario
* @param nuevaOpcion Nueva seleccion
* @return True en caso que se haya resaltado con exito, false en caso contrario
*/
bool MenuListLabel::setOpcionResaltada(int nuevaOpcion){
    if(nuevaOpcion >= 0 && nuevaOpcion < menuTextOptions.size()){
        menuTextOptions[mOpcionMenuResaltadaActual]->setTextColor(mColorLabelNormal); // Cambiamos el color de la opcion
        menuTextOptions[nuevaOpcion]->setTextColor(mColorLabelResaltado); // Cambiamos el color de la opcion
        mLayoutBackGround->setDisabled(true); // Para que se dibuje de nuevo
        mOpcionMenuResaltadaActual =  nuevaOpcion;
        return true;
    }

    return false;
}

/**
     * Procesa el evento del usuario
     * Se encarga de mover la opcion resaltada al usuario o en caso de que sea ENTER llamar a la funcion enlazada a
     * la opcion.
     * @param evento  Evento producido por SDL
     */
void MenuListLabel::procesarEvento(SDL_Event *evento)  {
    if(evento->type==SDL_KEYDOWN){
        switch(evento->key.keysym.sym){
            case SDLK_ESCAPE:
                mGameManagerInterfaz->goBack();
                break;
            case SDLK_UP:
                setOpcionResaltada(mOpcionMenuResaltadaActual - 1);
                break;
            case SDLK_DOWN:
                setOpcionResaltada(mOpcionMenuResaltadaActual + 1);
                break;
            case SDLK_RETURN:
                ejecutarAccionOpcionResaltada();
                break;
            default:
                break;
        }
    }else if(evento->type == SDL_JOYAXISMOTION&&evento->jaxis.type == SDL_JOYAXISMOTION){
        if(evento->jaxis.axis != 0){
            if(evento->jaxis.value > 10){
                setOpcionResaltada(mOpcionMenuResaltadaActual + 1);
            }else if(evento->jaxis.value < -10){
                setOpcionResaltada(mOpcionMenuResaltadaActual - 1);
            }
        }
    }else if(evento->type == SDL_JOYBUTTONDOWN){
        if(evento->jbutton.type == SDL_JOYBUTTONDOWN)
            if(evento->jbutton.button + 1==3)
                ejecutarAccionOpcionResaltada();

    }
}
void MenuListLabel::draw(SDL_Renderer *renderer) {
    if(mLayoutBackGround->isDisabled()){
        SDL_Rect rect = mGameManagerInterfaz->getRectScreen();
        mLayoutBackGround->pack(renderer);
        mLayoutBackGround->setRectDibujo(rect);
        mLayoutBackGround->draw(renderer);
    }
}

void MenuListLabel::forceDraw() {
    mLayoutBackGround->setDisabled(true);
}

MenuListLabel::~MenuListLabel() {
    delete mLayoutBackGround; // Al liberar el layout parent se liberan todos sus mComponentes

    for(int i = 0 ; i < menuTextOptions.size();i++){
        delete menuTextOptions[i];
    }
}