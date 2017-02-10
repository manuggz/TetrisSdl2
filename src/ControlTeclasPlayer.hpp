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
#include "engine/util/util.hpp"


class ControlTeclasPlayerInterfazParent{

public:
    virtual int getJoysActivos() = 0;
    virtual SDL_Joystick *getJoy(int device_index) = 0;
};

class ControlTeclasPlayer{
public:

    static const int DELAY_REPEAT_NONE   = 0;
    static const int DELAY_REPEAT_SHORT  = 150;
    static const int DELAY_REPEAT_MEDIUM = 350;
    static const int DELAY_REPEAT_LONG   = 600;
    static const int NO_REPEAT   = -1;

    typedef enum {
        TECLA_MOVER_LEFT,
        TECLA_MOVER_RIGHT,
        TECLA_GIRAR_LEFT,
        TECLA_GIRAR_RIGHT,
        TECLA_PAUSA,
        TECLA_SOFT_DROP,
        TECLA_HARD_DROP,
        N_TECLAS
    }TeclaControlPlayer;

    ControlTeclasPlayer() {
        for(int i = 0; i < N_TECLAS;i++){
            maxTeclaRepeatDelay[i] = DELAY_REPEAT_SHORT;
            es_boton_joystick[i]     = false;
            es_direccion_joystick[i] = false;
            joybuttonMapping[i] = 0;
            strcpy(guidJoystick[i],"sin asig.");
            mTeclasKeyboard[i]= SDL_SCANCODE_UNKNOWN;
        }
        establecerValorDefault();
    }

    void cargarDesdeArchivo(std::string rutaArchivoKeyBoard){

        std::ifstream fileArchivoKeyboard(rutaArchivoKeyBoard,std::ios::in|std::ios::binary);

        if(!fileArchivoKeyboard){
            std::cerr<<"Error leyendo control en:"<<rutaArchivoKeyBoard<<std::endl;
            perror("");
            establecerValorDefault();
        }else{
            fileArchivoKeyboard.read(reinterpret_cast<char *> (this),sizeof(ControlTeclasPlayer));
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
    void guardar(std::string ruta){
        std::ofstream fs2(ruta,std::ios::out|std::ios::binary);
        fs2.write(reinterpret_cast<char *> (this),sizeof(ControlTeclasPlayer));
        fs2.close();
    }

    void setParent(ControlTeclasPlayerInterfazParent *parent){
        this->parent = parent;
    }

    void update(){

        bool estaTeclaPresionada = false;
        const Uint8 *teclas= SDL_GetKeyboardState(NULL);//se obtiene el estado_actual actual del teclado

        for(int i = 0 ; i < N_TECLAS;i++){
            mEstaTeclaAceptada[i] = false;
            if(!es_boton_joystick[i] && !es_direccion_joystick[i]){
                estaTeclaPresionada = teclas[mTeclasKeyboard[i]];
            }else{
                if(parent == nullptr){
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                                 "Parent del Control del Teclado es null, debe asignarse un parent antes de hacer update!!");
                }
                estaTeclaPresionada =  false;
                static char temp[33];
                SDL_Joystick * pjoy;
                for(int j=0;j<parent->getJoysActivos();j++){

                    pjoy = parent->getJoy(j);
                    if(SDL_JoystickGetAttached(pjoy)){
                        SDL_JoystickGUID guidj = SDL_JoystickGetGUID(pjoy);
                        SDL_JoystickGetGUIDString(guidj,temp,33);

                        if(!strcmp(temp,guidJoystick[i])){//si coincide con el joistick con el que se configuro
                            if(es_direccion_joystick[i])
                                estaTeclaPresionada  = estado_direccion_joy(SDL_GetKeyFromScancode(mTeclasKeyboard[i]),pjoy);
                            else{
                                estaTeclaPresionada =  SDL_JoystickGetButton(pjoy, joybuttonMapping[i]);
                            }
                        }
                    }
                }
            }

            if(estaTeclaPresionada){
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
            }else{
                if(mEstabaTeclaPresionada[i]){
                    mControlTimerTeclas[i].stop();
                }
            }
            mEstabaTeclaPresionada[i] = estaTeclaPresionada;

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

    char * getJoystickGUID(TeclaControlPlayer tecla){
        return guidJoystick[tecla];
    }
    bool isBotonJoystick(TeclaControlPlayer tecla){
        return es_boton_joystick[tecla];
    }
    bool isDireccionJoystick(int tecla){
        return es_direccion_joystick[tecla];
    }
    void setJoystickGUID(TeclaControlPlayer tecla, const char *name){
        strcpy(guidJoystick[tecla],name);
    }
    void setIsBotonJoystick(TeclaControlPlayer tecla,bool nuevo){
        es_boton_joystick[tecla]=nuevo;
    }
    void setIsDireccionJoystick(int tecla,bool nuevo){
        es_direccion_joystick[tecla]=nuevo;
    }
    Uint8 getJoybuttonMapping(TeclaControlPlayer tecla) {
        return joybuttonMapping[tecla];
    }

    void setJoybuttonMapping(TeclaControlPlayer tecla, Uint8 nuevaTecla) {
        joybuttonMapping[tecla] = nuevaTecla;
    }

    SDL_Scancode getKeyboardMapping(TeclaControlPlayer teclaControlPlayer){
        return mTeclasKeyboard[teclaControlPlayer];
    }

    void setKeyboardMapping(TeclaControlPlayer teclaControlPlayer, SDL_Scancode valor){
        mTeclasKeyboard[teclaControlPlayer] = valor;
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

    Uint8 joybuttonMapping[N_TECLAS];
    char guidJoystick[N_TECLAS][100];
    bool es_boton_joystick[N_TECLAS];
    bool es_direccion_joystick[N_TECLAS];
    //Uint32 mMaxDelayRepeat = 150;

    ControlTeclasPlayerInterfazParent * parent = nullptr;
};
#endif //TETRIS_CONTROLKEYBOARDPLAYER_HPP
