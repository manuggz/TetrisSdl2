//#ifndef GALERIA_HPP
//#define GALERIA_HPP
////#define DEBUG
//#include <iostream>
//#include <fstream>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
//#include "../../util/constantes.hpp"
//#include "util.hpp"
//#include "LTexture.hpp"
//
//using namespace std;
//
//class Galeria{
//
//public:
//    typedef enum {
//        SFX_COGER_ITEM,
//        SFX_TONO_ACUATICO,
//        SFX_TONO_SECO,
//        SFX_CAMPANADA = 3,
//        SFX_EXPLOSION,
//        SFX_ESTRANYO,
//        SFX_PIERDE_VIDA,
//        _EFECTOS
//    }CodeMusicEfecto;
//
//    typedef enum {
//        SND_MENU=0,
//        SND_EDITOR=1,
//        SND_LEVEL_START=2,
//        SND_WARNING_TIME=3,
//        SND_IN_GAME_1=4,
//        SND_IN_GAME_2=5,
//        _SONIDOS
//    }CodeMusicSonido;
//
//    enum CodeImagen{
//        IMG_GLOBO,
//        IMG_PLAYER_1,
//        IMG_PLAYER_2,
//        IMG_PLAYER_3,
//        IMG_PLAYER_4,
//        IMG_PLAYER_5,
//        IMG_PLAYER_1_MURIENDO,
//        IMG_PLAYER_2_MURIENDO,
//        IMG_PLAYER_3_MURIENDO,
//        IMG_PLAYER_4_MURIENDO = 9,
//        IMG_PLAYER_5_MURIENDO,
//        IMG_FONDO_MENU,
//        IMG_FONDO_CREDITOS,
//        IMG_FONDO_EDITOR_SELECT_FILE,
//        IMG_FONDO_PARTI,
//        IMG_FONDO_EDIFICIOS = 15,
//        IMG_FONDO_METAL,
//        IMG_FONDO_NEGRO,
//        IMG_FONDO_BLANCO,
//        IMG_FUENTE_1,
//        IMG_FUENTE_2,
//        IMG_FUENTE_3 = 21,
//        IMG_FUENTE_4,
//        IMG_FUENTE_5,
//        IMG_FUENTE_6,
//        IMG_FUENTE_7,
//        IMG_FUENTE_8,
//        IMG_TXT_PUNTAJE,
//        IMG_TXT_PRESIONA_START = 28,
//        IMG_TXT_TILES,
//        IMG_TXT_ARRIBA,
//        IMG_TXT_ABAJO,
//        IMG_TXT_IZQUIERDA,
//        IMG_TXT_DERECHA,
//        IMG_TXT_ACCION = 34,
//        IMG_TXT_START,
//        IMG_TXT_VICTORIAS,
//        IMG_TXT_ACTIVADO,
//        IMG_TXT_PLAYERS_EN_BATALLA,
//        IMG_TXT_PRESIONA_ACCION,
//        IMG_TXT_PRESIONA = 40,
//        IMG_TXT_TIEMPO_POR_RONDA,
//        IMG_TXT_SCOREBOARD,
//        IMG_BOTON_ACTIVADO_DESACTIVADO,
//        IMG_BOTON_JUGAR,
//        IMG_BOTON_ESTRANIO,
//        IMG_BOTON_CAMBIAR,
//        IMG_BOTON_GUARDAR,
//        IMG_BOTON_BORRAR_MAPA = 48, //ad
//        IMG_BOTON_FLECHA_PEQUE_DERECHA,
//        IMG_BOTON_MAS,
//        IMG_BOTON_MENOS,
//        IMG_BOTON_FLECHA_GRANDE_IZQUIERDA,
//        IMG_BOTON_FLECHA_GRANDE_DERECHA = 53,
//        IMG_BOTON_PLAYER_1,
//        IMG_BOTON_PLAYER_2,
//        IMG_BOTON_PLAYER_3,
//        IMG_BOTON_PLAYER_4,
//        IMG_BOTON_PLAYER_5,
//        IMG_BOTON_CAMBIAR_MAPA,
//        IMG_BOTON_JUGAR_2 = 60,
//        IMG_TABLERO,
//        IMG_BLOQUES_FIRE,
//        IMG_ITEM_FIRE,
//        IMG_GUI_INPUT_TEXT,
//        IMG_TABLA_KILLS,
//        IMG_CARAS_BOMBERMAN,
//        IMG_CARAS_BOMBERMAN_GRANDES = 67,
//        IMG_BOMBA,
//        IMG_ITEM,
//        IMG_EXPLOSION,
//        IMG_CUADRO_PEQUENIO,
//        IMG_CUADRO_MEDIANO = 72,
//        IMG_CUADRO_GRANDE,
//        IMG_LLAMA,
//        IMG_BOMBA_PEQUE,
//        IMG_CORAZON = 76,
//        IMG_PREGUNTA,
//        IMG_TILES,
//        IMG_TROFEO,
//        IMG_CUADRO_SCOREBOARD = 80,
//        _IMAGENES
//    };
//
//    Galeria();
//    LTexture * getImagen(CodeImagen code){
//        return baulimgs[code];  /*SIN IMPLEMENTAR*/
//    };
//    Mix_Chunk * getMusicEfecto(CodeMusicEfecto code){
//        if(!sonidoCargado)return NULL;
//        return sfx_efectos[code];
//    };
//    Mix_Music * getMusicSonido(CodeMusicSonido code){
//        if(!sonidoCargado)return NULL;
//        return snd_musicas[code];
//    };
//    bool cargarTexturas(SDL_Renderer *gRenderer, string rutaTexturas);
//    bool cargarSonidos(string rutaSonidos);
//    ~Galeria();
//
//private:
//    bool sonidoCargado;
//    LTexture *baulimgs[_IMAGENES];
//    Mix_Chunk *sfx_efectos[_EFECTOS];
//    Mix_Music *snd_musicas[_SONIDOS];
//
//};
//
//#endif
