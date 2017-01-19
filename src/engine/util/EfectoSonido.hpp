//
// Created by manuggz on 10/01/17.
//

#ifndef ENGINE_SOUNDEFFECT_HPP
#define ENGINE_SOUNDEFFECT_HPP

#include <string>
//#include <iostream>
//#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class EfectoSonido{
public:
    EfectoSonido(std::string path,int volumen){
        mPath      = path;
        mpRawChunk = Mix_LoadWAV(path.c_str());
        if(mpRawChunk != nullptr){
            SDL_Log("Efecto de Sonido Cargado: %s.",path.c_str());
            Mix_VolumeChunk(mpRawChunk, volumen);
        }else{
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"No se logro cargar Efecto de Sonido: %s . %s",
                        path.c_str(),Mix_GetError());
        }
    }
    ~EfectoSonido(){
        Mix_FreeChunk(mpRawChunk);
    }

    void play(int channel = -1,int loops = 0) {
        //Mix_PlayChannel(channel,mpRawChunk,loops);
    }

private:
    Mix_Chunk * mpRawChunk;
    std::string mPath;
};
#endif //BOMBERMAN_SOUNDEFFECT_HPP
