//
// Created by manuggz on 11/01/17.
//

#ifndef BOMBERMAN_MUSICAFONDO_HPP
#define BOMBERMAN_MUSICAFONDO_HPP

#include <iostream>
#include <SDL2/SDL_mixer.h>


class MusicaFondo{

public:
    static const int MAX_VOLUMEN = 128;
    static const int MIN_VOLUMEN = 0;

    MusicaFondo(std::string path) {
        mpRawMixMusic = Mix_LoadMUS(path.c_str());
        if (mpRawMixMusic != nullptr) {
            SDL_Log("Cargado Sonido: %s.",path.c_str());
        } else {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"No se logro cargar Sonido: %s . %s",
                        path.c_str(),Mix_GetError());
        }
    }

    void play(int volumen = MAX_VOLUMEN,int loops = -1){
        //Mix_VolumeMusic(volumen);
        //Mix_PlayMusic(mpRawMixMusic, loops);
    }

    /* Fade in music or a channel over "ms" milliseconds, same semantics as the "Play" functions */
    void playFadeIn(int ms= 2000,int volumen = MAX_VOLUMEN,int loops = -1){
        Mix_VolumeMusic(volumen);
        //Mix_FadeInMusic(mpRawMixMusic, -1,ms);
    }

    ~MusicaFondo(){
        Mix_FreeMusic(mpRawMixMusic);
    }
private:
    Mix_Music * mpRawMixMusic;

};
#endif //BOMBERMAN_MUSICAFONDO_HPP
