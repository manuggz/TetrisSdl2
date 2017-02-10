//
// Created by manuggz on 19/01/17.
//

#ifndef TETRIS_CONTEXTMENU_HPP
#define TETRIS_CONTEXTMENU_HPP

//#include <bits/unordered_map.h>
#include <map>
#include "FlechaDinamica.hpp"
#include "engine/util/BitmapFont.hpp"

class OpcionMenuContext{
public:

    OpcionMenuContext(std::string leftText,std::string rightText,int id){
        mLeftText = leftText;
        mRightText = rightText;
        mId = id;
    }
    bool isVisible() const {
        return visible;
    }

    void setVisible(bool visible) {
        OpcionMenuContext::visible = visible;
    }

    void setAlpha(Uint8 alpha){
        pLeftTextRenderer->setAlpha(alpha);
        pRightTextRenderer->setAlpha(alpha);
    }
    ~OpcionMenuContext(){
        delete pLeftTextRenderer;
        delete pRightTextRenderer;
    }

    void setRight(int right){
        mRight = right;
        if(pRightTextRenderer != nullptr) {
            pRightTextRenderer->setRight(mRight);
        }
    }

    void draw(SDL_Renderer *pRenderer, int x, int y) {
        if(visible&&pLeftTextRenderer != nullptr)pLeftTextRenderer->draw(pRenderer,x,y);
        if(visible&&pRightTextRenderer != nullptr){
            pRightTextRenderer->setTop(y);
            pRightTextRenderer->draw(pRenderer);
        }
    }

    int getId(){
        return mId;
    }
    int getHeight() {
        return (pLeftTextRenderer == nullptr)? 0:pLeftTextRenderer->getHeight();
    }
    void establecerRightText(std::string nuevoValor){
        mRightText = nuevoValor;
        if(pRightTextRenderer != nullptr) {
            pRightTextRenderer->setText(mRightText);
        }
    }

    void setBitmapFont(BitmapFont *pFont){
        if(pLeftTextRenderer == nullptr){

            pLeftTextRenderer = new BitmapFontRenderer(pFont);
            pLeftTextRenderer->setText(mLeftText);

            pRightTextRenderer = new BitmapFontRenderer(pFont);
            pRightTextRenderer->setText(mRightText);
            pRightTextRenderer->setRight(mRight);
        }else{
            pLeftTextRenderer->setBitmapFont(pFont);
            pRightTextRenderer->setBitmapFont(pFont);
        }
    }
    std::string getLeftText(){
        return mLeftText;
    }

    std::string getRightText(){
        return mRightText;
    }

    void setFlechasHaciaAfuera(bool nuevo){
        flechas_hacia_afuera = nuevo;
    }
    bool getFlechasHaciaAfuera(){
        return flechas_hacia_afuera;
    }
private:
    bool visible  = true;
    bool flechas_hacia_afuera = false;
    BitmapFontRenderer * pLeftTextRenderer = nullptr;
    BitmapFontRenderer * pRightTextRenderer = nullptr;
    std::string mLeftText;
    int mId;
    std::string mRightText;
    int mRight = 0;
};

class ContextMenuParent {
public:
    virtual void cambiadoResaltadoActualMenuContextual(int idNuevaOpcionResaltada){};

    virtual void eventoFlechaPresionadaMenuContextual(int idOpcionResaltadaActual){};

    virtual void eventoFlechaDerechaPresionadaMenuContextual(int idOpcionResaltadaActual){};
    virtual void eventoFlechaIzquierdaPresionadaMenuContextual(int idOpcionResaltadaActual) {};

    virtual void opcionSeleccionadaMenuContextual(int idOpcionSeleccionada) = 0;

};

class MenuContextual{

public:


    typedef enum{
        FUENTE_NORMAL,
        FUENTE_RESALTADA,
        _N_FUENTES
    }TipoFuente;

    MenuContextual(ContextMenuParent * p_parent ,int x,int y){
        pflechaIzquierda = nullptr;
        pflechaDerecha = nullptr;
        mRectFondo.x = x;
        mRectFondo.y = y;
        mRectFondo.w = 0;
        mRectFondo.h = 0;
        mRectDibujoOpciones = mRectFondo;
        MenuContextual::p_parent = p_parent;
    }

    int getMargenEntreOpciones() const {
        return margenEntreOpciones;
    }

    void establecerMargenEntreOpciones(int margenEntreOpciones) {
        MenuContextual::margenEntreOpciones = margenEntreOpciones;
    }
    void agregarOpcionMenu(std::string leftText,int id,std::string rightText = ""){
        opciones_v.push_back(new OpcionMenuContext(leftText,rightText,id));
    }

    void establecerRightTextoOpcionMenu(int idOpcion, std::string nuevoTexto){
        for(int i = 0; i < opciones_v.size();i++){
            if(opciones_v[i]->getId() == idOpcion){
                opciones_v[i]->establecerRightText(nuevoTexto);
                return;
            }
        }
    }

    void establecerFlechaHaciaAfuera(int idOpcion, bool nuevoValor){
        for(int i = 0; i < opciones_v.size();i++){
            if(opciones_v[i]->getId() == idOpcion){
                opciones_v[i]->setFlechasHaciaAfuera(nuevoValor);
                return;
            }
        }
    }

    void drawFondo(SDL_Renderer * gRenderer){
        if(mTextureFondo != nullptr){
            //mTextureFondo->draw(gRenderer,mRectFondo.x,mRectFondo.y);
            SDL_RenderCopy(gRenderer, mTextureFondo->getRawTexture(), nullptr, &mRectFondo); //Render the backBuffer onto the

        }
    }

    void drawOpciones(SDL_Renderer * gRenderer){

        int yDibujoOpcion = mRectDibujoOpciones.y;

        for(int i = 0; i < opciones_v.size();i++){
            opciones_v[i]->setRight(mRectDibujoOpciones.x + mRectDibujoOpciones.w);
            opciones_v[i]->draw(gRenderer,mRectDibujoOpciones.x,yDibujoOpcion);

            if(i == indice_resaltado_actual){
                pflechaIzquierda->setY(yDibujoOpcion);
                pflechaDerecha->setY(yDibujoOpcion);
            }

            if(opciones_v[i]->isVisible()){
                yDibujoOpcion += opciones_v[i]->getHeight() + margenEntreOpciones;
            }

        }
    }
    void drawFlechas(SDL_Renderer * gRenderer){
        pflechaIzquierda->draw(gRenderer);
        pflechaDerecha->draw(gRenderer);
    }

    virtual void draw(SDL_Renderer * gRenderer){
        drawFondo(gRenderer);
        drawOpciones(gRenderer);
        drawFlechas(gRenderer);
    }

    int obtener_siguiente_opcion_ip(int aum_j){
        int indice_actual = indice_resaltado_actual + aum_j;
        while(indice_actual >= 0 && indice_actual < opciones_v.size()){
            if(opciones_v[indice_actual]->isVisible()){
                return opciones_v[indice_actual]->getId();
            }else{
                indice_actual = indice_actual + aum_j;
            }
        }
        return -1;
    }


    void procesarEvento(SDL_Event *event){
        if(indice_resaltado_actual == -1) return;

        if (event->type == SDL_KEYDOWN) {
            switch (event->key.keysym.sym) {
                case SDLK_DOWN:
                case SDLK_UP:{
                    int nueva_opcion_id = obtener_siguiente_opcion_ip((event->key.keysym.sym == SDLK_DOWN)?1:-1);
                        if(nueva_opcion_id != -1){
                            p_parent->cambiadoResaltadoActualMenuContextual(nueva_opcion_id);
                            establecerResaltado(nueva_opcion_id);
                        }
                    }

                    break;
                case SDLK_RETURN:
                    if(indice_resaltado_actual != -1){
                        p_parent->opcionSeleccionadaMenuContextual(opciones_v[indice_resaltado_actual]->getId());
                    }
                    break;
                case SDLK_LEFT:
                    if(indice_resaltado_actual != -1){
                        p_parent->eventoFlechaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());
                        p_parent->eventoFlechaIzquierdaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());

                    }
                    break;
                case SDLK_RIGHT:
                    if(indice_resaltado_actual != -1){
                        p_parent->eventoFlechaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());
                        p_parent->eventoFlechaDerechaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());
                    }
                    break;
                default:
                    break;
            }

        }else if(event->type == SDL_JOYAXISMOTION&&event->jaxis.type == SDL_JOYAXISMOTION){
            if(event->jaxis.axis != 0){
                int nueva_opcion_id = -1;
                if(event->jaxis.value > 10){
                    nueva_opcion_id = obtener_siguiente_opcion_ip(1);
                }else if(event->jaxis.value < -10){
                    nueva_opcion_id = obtener_siguiente_opcion_ip(-1);
                }
                if(nueva_opcion_id != -1){
                    p_parent->cambiadoResaltadoActualMenuContextual(nueva_opcion_id);
                    establecerResaltado(nueva_opcion_id);
                }
            }else{
                if(event->jaxis.value > 10){
                    if(indice_resaltado_actual != -1){
                        p_parent->eventoFlechaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());
                        p_parent->eventoFlechaDerechaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());
                    }
                }else if(event->jaxis.value < -10){
                    if(indice_resaltado_actual != -1){
                        p_parent->eventoFlechaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());
                        p_parent->eventoFlechaIzquierdaPresionadaMenuContextual(
                                opciones_v[indice_resaltado_actual]->getId());

                    }
                }
            }
        }else if(event->type == SDL_JOYBUTTONDOWN){
            if(event->jbutton.type == SDL_JOYBUTTONDOWN)
                if(event->jbutton.button + 1==3) {
                    if(indice_resaltado_actual != -1){
                        p_parent->opcionSeleccionadaMenuContextual(opciones_v[indice_resaltado_actual]->getId());
                    }
                }
        }
    }

    void establecerTexturaFondo(LTexture * fondo) {

        if (mTextureFondo != nullptr)delete mTextureFondo;

        mTextureFondo = fondo;
    }

    void actualizarRectanguloDibujo(){
        //mRectFondo.w = fondo->getWidth();
        //mRectFondo.h = fondo->getHeight();
        if(mpBitmapFont[FUENTE_NORMAL] == nullptr)return;

        int mayor_width_opciones = 0;

        int width_opcion;
        for(int i = 0; i < opciones_v.size();i++){
            width_opcion = mpBitmapFont[FUENTE_NORMAL]->calculateWidth(opciones_v[i]->getLeftText());

            if(!opciones_v[i]->getRightText().empty())
                width_opcion += mpBitmapFont[FUENTE_NORMAL]->calculateWidth(" " + opciones_v[i]->getRightText());

            if(width_opcion > mayor_width_opciones){
                mayor_width_opciones = width_opcion;
            }
        }

        mRectFondo.w = mayor_width_opciones +
                pflechaIzquierda->getWidth() + pflechaIzquierda->getWidth() /2 +
                pflechaDerecha->getWidth() + pflechaDerecha->getWidth() /2 + 20;
        mRectFondo.h = opciones_v.size() * mpBitmapFont[FUENTE_NORMAL]->getHeight() +
                (opciones_v.size())*margenEntreOpciones +
                mpBitmapFont[FUENTE_NORMAL]->getHeight();

        mRectDibujoOpciones.x = mRectFondo.x + mRectFondo.w/2 - mayor_width_opciones/2;
        mRectDibujoOpciones.y = mRectFondo.y + mpBitmapFont[FUENTE_NORMAL]->getHeight()/2;
        mRectDibujoOpciones.w = mayor_width_opciones;
        mRectDibujoOpciones.h = mRectFondo.h;
    }

    void establecerFuente(BitmapFont *pFont, MenuContextual::TipoFuente tipoFuente) {
        if(pFont == nullptr || tipoFuente == _N_FUENTES) return;
        mpBitmapFont[tipoFuente]  = pFont;
    }
    
    void establecerResaltado(int id){

        int indice_nuevo_resaltado = -1;

        for(int i = 0; i < opciones_v.size();i++){
            if(opciones_v[i]->getId() == id){
                indice_nuevo_resaltado = i;
                break;
            }
        }

        if(indice_nuevo_resaltado != -1){

            if(indice_resaltado_actual != -1 && mpBitmapFont[FUENTE_NORMAL])
                opciones_v[indice_resaltado_actual]->setBitmapFont(mpBitmapFont[FUENTE_NORMAL]);

            if(mpBitmapFont[FUENTE_RESALTADA])
                opciones_v[indice_nuevo_resaltado]->setBitmapFont(mpBitmapFont[FUENTE_RESALTADA]);

            indice_resaltado_actual =  indice_nuevo_resaltado;

            if(opciones_v[indice_nuevo_resaltado]->getFlechasHaciaAfuera()) {
                pflechaIzquierda->setX(mRectDibujoOpciones.x - pflechaIzquierda->getWidth() - pflechaIzquierda->getWidth()/2);
                pflechaDerecha->setX(mRectFondo.x + mRectFondo.w - pflechaIzquierda->getWidth() - pflechaIzquierda->getWidth()/2);
            }else{
                pflechaDerecha->setX(mRectDibujoOpciones.x - pflechaDerecha->getWidth()- pflechaDerecha->getWidth()/2);
                pflechaIzquierda->setX(mRectFondo.x + mRectFondo.w - pflechaIzquierda->getWidth() - pflechaIzquierda->getWidth()/2);

            }
        }


    }

    void establecerTexturaFlechaDerecha(LTexture *textureFlechaDerecha) {
        if(pflechaDerecha != nullptr){
            delete pflechaDerecha;
        }
        pflechaDerecha = new FlechaDinamica(textureFlechaDerecha);
    }

    void establecerTexturaFlechaIzquierda(LTexture *textureFlechaIzquierda) {
        if(pflechaIzquierda != nullptr){
            delete pflechaIzquierda;
        }
        pflechaIzquierda = new FlechaDinamica(textureFlechaIzquierda);
    }

    virtual void createUI(SDL_Renderer *pRenderer) {

        if(mpBitmapFont[FUENTE_NORMAL] != nullptr) {
            for (int i = 0; i < opciones_v.size(); i++) {
                if(indice_resaltado_actual == i && mpBitmapFont[FUENTE_RESALTADA] != nullptr){
                    opciones_v[i]->setBitmapFont(mpBitmapFont[FUENTE_RESALTADA]);
                }else{
                    opciones_v[i]->setBitmapFont(mpBitmapFont[FUENTE_NORMAL]);
                }
            }
        }
        actualizarRectanguloDibujo();
    }
    virtual void update(){
        pflechaIzquierda->update(nullptr);
        pflechaDerecha->update(nullptr);
    }

    ~MenuContextual(){
        while( opciones_v.size()){
            delete opciones_v[opciones_v.size() - 1];
            opciones_v.pop_back();
        }
        delete pflechaDerecha;
        delete pflechaIzquierda;

        for(int i = 0; i < _N_FUENTES;i++) {
            delete mpBitmapFont[i];
        }

        delete mTextureFondo;
    }

    void setVisible(bool nuevo) {
        visibilidad = nuevo;
    }
    bool esVisible(){
        return visibilidad;
    }
    void establecerVisilidad(int id_opcion, bool activado){
        for(int i = 0; i < opciones_v.size();i++){
            if(id_opcion == opciones_v[i]->getId()){
                opciones_v[i]->setVisible(activado);
                return;
            }
        }
    }

    int obtenerIDResaltadoActual() {
        return opciones_v[indice_resaltado_actual]->getId();
    }

    bool esOpcionMenuVisible(const int ID) {
        for(int i = 0; i < opciones_v.size();i++){
            if(ID == opciones_v[i]->getId()){
                return opciones_v[i]->isVisible();
            }
        }
        return false;
    }

private:

    BitmapFont * mpBitmapFont[_N_FUENTES] {nullptr};

    int indice_resaltado_actual = -1;

    SDL_Rect mRectDibujoOpciones {0,0,0,0};

    int margenEntreOpciones = 5;

protected:
    SDL_Rect mRectFondo {0, 0, 0, 0};
    ContextMenuParent *p_parent;
    LTexture *mTextureFondo = nullptr;
    FlechaDinamica *pflechaDerecha   = nullptr;
    FlechaDinamica *pflechaIzquierda = nullptr;

    std::vector<OpcionMenuContext * > opciones_v;
    bool visibilidad = true;
};
#endif //TETRIS_CONTEXTMENU_HPP
