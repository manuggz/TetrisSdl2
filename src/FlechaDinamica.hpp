//
// Created by manuggz on 16/01/17.
//

#ifndef BOMBERMAN_SPRITEFLECHA_HPP
#define BOMBERMAN_SPRITEFLECHA_HPP

#include "engine/sprites/CSprite.hpp"

class FlechaDinamica: public Sprite{
    static  const int MAX_DELAY_MOV_HORIZONTAL = 2;
    static  const int MAX_DELAY_CAMBIO_DIR_HORIZONTAL = MAX_DELAY_MOV_HORIZONTAL*6;

    static const int MAX_DELAY_MOV_VERTICAL = 1;
public:
    FlechaDinamica(LTexture * pTexture) : Sprite() {
        pTextureFlecha = pTexture;
    }
    void setBlendMode( SDL_BlendMode blending )
    {
        pTextureFlecha->setBlendMode(blending);
    }

    void setAlpha( Uint8 alpha )
    {
        pTextureFlecha->setAlpha(alpha);
    }

    void setYDestino(int yDest){
        yFinal = yDest;
    }

    int getWidth() override {
        return pTextureFlecha->getWidth();
    }

    int getHeight() override {
        return pTextureFlecha->getHeight();
    }

    void update(const Uint8 *keys) override {

        if (++delayMovimientoHorizontal >= MAX_DELAY_MOV_HORIZONTAL) {
            delayMovimientoHorizontal = 0;
            if(direccion == IZQUIERDA){
                move_ip(-1,0);
            }else{
                move_ip(1,0);
            }

        }

        if (++delayCambioDireccionHorizontal >= MAX_DELAY_CAMBIO_DIR_HORIZONTAL) {
            delayCambioDireccionHorizontal = 0;
            direccion = (direccion == IZQUIERDA)?DERECHA:IZQUIERDA;
        }

        if (++delayMovimientoVertical >= MAX_DELAY_MOV_VERTICAL) {
            delayMovimientoVertical = 0;
            if(yFinal > y ){
                move_ip(0,1);
            }else if(yFinal < y){
                move_ip(0,-1);
            }
        }

    }

    void draw(SDL_Renderer *renderer) override {
        pTextureFlecha->draw(renderer,x,y);
    }

    ~FlechaDinamica() {
        delete pTextureFlecha;
    }

private:

    LTexture * pTextureFlecha;
    enum DireccionFlecha{
        IZQUIERDA,
        DERECHA
    };
    int delayCambioDireccionHorizontal = 0;
    int delayMovimientoHorizontal = 0;
    int delayMovimientoVertical = 0;
    int yFinal;
    DireccionFlecha direccion = IZQUIERDA;
};
#endif //BOMBERMAN_SPRITEFLECHA_HPP
