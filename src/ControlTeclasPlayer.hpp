//
// Created by manuggz on 18/12/16.
//

#ifndef TETRIS_CONTROLKEYBOARDPLAYER_HPP
#define TETRIS_CONTROLKEYBOARDPLAYER_HPP
#include <string>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include "engine/util/LTimer.hpp"


class ControlTeclasPlayer{
public:

    enum TeclaControlPlayer{
        TECLA_HARD_DROP,
        TECLA_SOFT_DROP,
        TECLA_MOVER_LEFT,
        TECLA_MOVER_RIGHT,
        TECLA_GIRAR_LEFT,
        TECLA_GIRAR_RIGHT,
        TECLA_PAUSA,
        N_TECLAS
    };

    ControlTeclasPlayer() {
        establecerValorDefault();
    }

    ControlTeclasPlayer(std::string rutaArchivoKeyBoard){

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
        mTeclasKeyboard[TECLA_PAUSA] = SDL_SCANCODE_RETURN;
    }

    void update( const Uint8 * _teclas){
        for(int i = 0 ; i < N_TECLAS;i++){
            mEstaTeclaAceptada[i] = false;
            if(_teclas[mTeclasKeyboard[i]]){
                if(!mEsBotonJoystick[i] && !mEsDireccionJoystick[i]){
                    if(mEstaTeclaPresionada[i]){
                        if(mControlTimerTeclas[i].getTicks() >= mMaxDelayRepeat){
                            mEstaTeclaAceptada[i] = true;
                            mControlTimerTeclas[i].start();
                        }
                    }else{
                        mControlTimerTeclas[i].start();
                        mEstaTeclaAceptada[i] = true;
                    }
                }
            }else{
                if(!mEsBotonJoystick[i] && !mEsDireccionJoystick[i]){
                    if(mEstaTeclaPresionada[i]){
                        mControlTimerTeclas[i].stop();
                    }
                }

            }
            mEstaTeclaPresionada[i] = _teclas[mTeclasKeyboard[i]];
        }
    }

    bool estaTeclaAceptada(TeclaControlPlayer tecla){
        return mEstaTeclaPresionada[tecla] && mEstaTeclaAceptada[tecla];
    }

    bool estaTeclaPresionada(TeclaControlPlayer tecla) {
        return mEstaTeclaPresionada[tecla];
    }

private:
    SDL_Keycode mTeclasKeyboard[N_TECLAS] {0};

    char mNombreJoystickUsado[N_TECLAS][100];
    bool mEsBotonJoystick[N_TECLAS] {false};
    bool mEsDireccionJoystick[N_TECLAS] {false};

    bool mEstaTeclaPresionada[N_TECLAS]{false};
    bool mEstaTeclaAceptada[N_TECLAS] {false};

    LTimer mControlTimerTeclas[N_TECLAS];

    Uint32 mMaxDelayRepeat = 150;
};
#endif //TETRIS_CONTROLKEYBOARDPLAYER_HPP
