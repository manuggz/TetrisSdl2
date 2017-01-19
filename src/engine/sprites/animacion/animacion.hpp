#ifndef ANIMACION_HPP
#define ANIMACION_HPP
#include <SDL2/SDL.h>
#include "../CSprite.hpp"
#include "Control_Animacion.hpp"
#include "../../util/LTexture.hpp"
#include "../../util/SpriteSheet.hpp"

/**
 * Representa un Sprite "Animado"
 *
 * Es decir, que tiene una animacion.
 *
 *
 * La clase recive una Sprite Sheet
 */
class Animacion:public Sprite{
public:
    Animacion(SpriteSheet  * spriteSheet,string frames=NULL,int x=0,int y=0,int delayCambioFrame=2);
    void update(const Uint8 *keys=NULL);
    virtual void disable();
    virtual void draw(SDL_Renderer * );

    void move(int nuevaX, int nuevaY) override;
    /*int getTipo(){return type;};
    int getId(){return id;};*/
    //int getLoop(){return mLoop;};
    int getCuadro(){return mControlFrames.cuadro();};
    void setRepeticiones(int nuevo){mRepeticiones=nuevo;};
    void setCuadroDespues(int nuevo){indexInicioAniEnd=nuevo;};
    void setDelayCambioFrame(int nuevo){mDelayCambioFrame=nuevo;};

    void setCuadrosFrames(char * frames);
    ~Animacion(){
        delete mSprSCuadros;
    }
private:
    //LTexture * mTextuGrilla = nullptr;
    ControlAnimacion mControlFrames;
    //bool cambiarOrden = false;
    //int mFilas,mColumnas;
    int delay = 0;

    //cuando la animacion termine una ejecuci�n,
    // si se especif�ca �sta variable se dibujara solo los frames que comienzen desde su valor
    int indexInicioAniEnd = 0;

    int mDelayCambioFrame = 0;
protected:
    SpriteSheet * mSprSCuadros = nullptr;
    int mRepeticiones = 0;
};

#endif
