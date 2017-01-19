//
// Created by manuggz on 13/12/16.
//

#ifndef TETRIS_SIMPLEANIMACION_HPP
#define TETRIS_SIMPLEANIMACION_HPP

#include <SDL.h>

class SimpleAnimacion{
public:

    SimpleAnimacion(){
    }
    virtual void start(){
        mIsStarted = true;
        mIsRunning = true;

    }

    virtual void update() = 0;
    virtual void draw(SDL_Renderer * gRenderer) = 0;
    virtual void pause(){

    }

    virtual void stop(){
        mIsStopped = true;
        mIsRunning = false;
    }

    virtual bool isStopped(){
        return mIsStopped;
    }

    int getCodigo() {
        return mCodigo;
    }
    void setCodigo(int nuevo) {
        mCodigo = nuevo;
    }

protected:
    bool mIsPaused  = false;
    bool mIsStopped = false;
    bool mIsStarted = false;
    bool mIsRunning = false;
    int mCodigo = -1;
};
#endif //TETRIS_SIMPLEANIMACION_HPP
