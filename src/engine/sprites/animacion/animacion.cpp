#include "animacion.hpp"

Animacion::Animacion(SpriteSheet * spriteSheet,string frames,int x,int y,int delayCambioFrame):
    mControlFrames(frames){

    this->mSprSCuadros = spriteSheet;
    this->x = x;
    rect.x = x;
    this->y = y;
    rect.y = y;

    if(spriteSheet != nullptr){
        rect.w = spriteSheet->getWidthCuadro();
        rect.h = spriteSheet->getHeightCuadro();
    }

    mSelfKill = false;
    delay = 0;
    mRepeticiones = 0;
    indexInicioAniEnd = 0;
    mDelayCambioFrame = delayCambioFrame;
    mSprSCuadros->setCurrentCuadro(mControlFrames.cuadro());
}

void Animacion::update(const Uint8 * teclas){

	if (++delay > mDelayCambioFrame){

        if(mControlFrames.avanzar()){
            if(mRepeticiones!=-1&&--mRepeticiones<0)
                disable();
            else if(indexInicioAniEnd!=0){
                mControlFrames.setInicioAnimacion(indexInicioAniEnd);
                indexInicioAniEnd=0;
                mControlFrames.reiniciar();
            }
        }

        delay=0;
        mSprSCuadros->setCurrentCuadro(mControlFrames.cuadro());
	}
}
void Animacion::disable(){
    kill();
}

void Animacion::setCuadrosFrames(char * frames){
    mControlFrames.setCuadros(frames);
}

void Animacion::draw(SDL_Renderer * gRenderer){
    mSprSCuadros->draw(gRenderer,x,y);
}

void Animacion::move(int nuevaX, int nuevaY) {
    Sprite::move(nuevaX, nuevaY);
    rect.w = mSprSCuadros->getWidthCuadro();
    rect.h = mSprSCuadros->getHeightCuadro();
}
