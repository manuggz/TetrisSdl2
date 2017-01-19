//
// Created by manuggz on 18/12/16.
//

#ifndef TETRIS_CONTROLKEYBOARDPLAYER_HPP
#define TETRIS_CONTROLKEYBOARDPLAYER_HPP
#include <string>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <cstring>
#include "engine/util/LTimer.hpp"


class ControlTeclasPlayer{
public:

    static const int DELAY_REPEAT_NONE   = 0;
    static const int DELAY_REPEAT_SHORT  = 150;
    static const int DELAY_REPEAT_MEDIUM = 350;
    static const int DELAY_REPEAT_LONG   = 600;
    static const int NO_REPEAT   = -1;

    typedef enum {
        TECLA_HARD_DROP,
        TECLA_SOFT_DROP,
        TECLA_MOVER_LEFT,
        TECLA_MOVER_RIGHT,
        TECLA_GIRAR_LEFT,
        TECLA_GIRAR_RIGHT,
        TECLA_PAUSA,
        N_TECLAS
    }TeclaControlPlayer;

    ControlTeclasPlayer() {
        for(int i = 0; i < N_TECLAS;i++){
            maxTeclaRepeatDelay[i] = DELAY_REPEAT_SHORT;
        }
        establecerValorDefault();
    }

    void cargarDesdeArchivo(std::string rutaArchivoKeyBoard){

        std::ifstream fileArchivoKeyboard(rutaArchivoKeyBoard,std::ios::in|std::ios::binary);

        if(!fileArchivoKeyboard){
            std::cerr<<"Error leyendo control en:"<<rutaArchivoKeyBoard<<std::endl;
            establecerValorDefault();
        }else{
            fileArchivoKeyboard.read(reinterpret_cast<char *> (mTeclasKeyboard),sizeof(mTeclasKeyboard));
            fileArchivoKeyboard.read(reinterpret_cast<char *> (mNombreJoystickUsado),sizeof(mNombreJoystickUsado));
            fileArchivoKeyboard.read(reinterpret_cast<char *> (mEsBotonJoystick),sizeof(mEsBotonJoystick));
            fileArchivoKeyboard.read(reinterpret_cast<char *> (mEsDireccionJoystick),sizeof(mEsDireccionJoystick));
            fileArchivoKeyboard.close();
        }
    }

    void establecerValorDefault(){
        mTeclasKeyboard[TECLA_HARD_DROP] = SDL_SCANCODE_UP;
        mTeclasKeyboard[TECLA_SOFT_DROP] = SDL_SCANCODE_DOWN;
        mTeclasKeyboard[TECLA_MOVER_LEFT] = SDL_SCANCODE_LEFT;
        mTeclasKeyboard[TECLA_MOVER_RIGHT] = SDL_SCANCODE_RIGHT;
        mTeclasKeyboard[TECLA_GIRAR_LEFT] = SDL_SCANCODE_Z;
        mTeclasKeyboard[TECLA_GIRAR_RIGHT] = SDL_SCANCODE_X;
        mTeclasKeyboard[TECLA_PAUSA] = SDL_SCANCODE_P;
    }

    void update( const Uint8 * _teclas){
        for(int i = 0 ; i < N_TECLAS;i++){
            mEstaTeclaAceptada[i] = false;
            if(_teclas[mTeclasKeyboard[i]]){
                if(!mEsBotonJoystick[i] && !mEsDireccionJoystick[i]){
                    if(maxTeclaRepeatDelay[i] == NO_REPEAT){
                        if(!mEstabaTeclaPresionada[i]){
                            mEstaTeclaAceptada[i] = true;
                        }
                    }else if(maxTeclaRepeatDelay[i] == DELAY_REPEAT_NONE){
                        mEstaTeclaAceptada[i] = true;
                    }else{
                        if(mEstabaTeclaPresionada[i]){
                            if(mControlTimerTeclas[i].getTicks() >= maxTeclaRepeatDelay[i]) {
                                mEstaTeclaAceptada[i] = true;
                                mControlTimerTeclas[i].start();
                            }
                        }else{
                            mControlTimerTeclas[i].start();
                            mEstaTeclaAceptada[i] = true;
                        }
                    }
                }
            }else{
                if(!mEsBotonJoystick[i] && !mEsDireccionJoystick[i]){
                    if(mEstabaTeclaPresionada[i]){
                        mControlTimerTeclas[i].stop();
                    }
                }

            }
            mEstabaTeclaPresionada[i] = _teclas[mTeclasKeyboard[i]];
        }
    }

    bool estaTeclaAceptada(TeclaControlPlayer tecla){
        return mEstabaTeclaPresionada[tecla] && mEstaTeclaAceptada[tecla];
    }

    bool estaTeclaPresionada(TeclaControlPlayer tecla) {
        return mEstabaTeclaPresionada[tecla];
    }

    void setTeclaRepeatDelay(TeclaControlPlayer tecla,int delay){
        maxTeclaRepeatDelay[tecla] = delay;
        mEstaTeclaAceptada[tecla] = false;
        mEstabaTeclaPresionada[tecla] = false;
    }

private:
    SDL_Scancode mTeclasKeyboard[N_TECLAS] {SDL_SCANCODE_UNKNOWN};

    char mNombreJoystickUsado[N_TECLAS][100];
    bool mEsBotonJoystick[N_TECLAS] {false};
    bool mEsDireccionJoystick[N_TECLAS] {false};

    bool mEstabaTeclaPresionada[N_TECLAS]{false};
    bool mEstaTeclaAceptada[N_TECLAS] {false};
    int maxTeclaRepeatDelay[N_TECLAS] {DELAY_REPEAT_SHORT};

    LTimer mControlTimerTeclas[N_TECLAS];

    //Uint32 mMaxDelayRepeat = 150;

};
#endif //TETRIS_CONTROLKEYBOARDPLAYER_HPP
