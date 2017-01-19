#ifndef UTIL_HPP
#define UTIL_HPP
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "LTexture.hpp"

//SDL_Texture *cargar_textura(SDL_Renderer *gRenderer, std::string ruta, bool tiene_color_clave);
Mix_Chunk * cargar_sonido(char ruta[]);
Mix_Music * cargar_musica(const char ruta[]);
void imprimir_desde_grilla(LTexture * src, int cuadro, SDL_Renderer *,int x_dest,int y_dest, int fil, int col,int alpha);
//SDL_Surface * iniciar_modo(Uint16 w,Uint16 h,Uint8 bpp,Uint32 flags);
//int fps_sincronizar (void);
bool rects_colisionan(SDL_Rect & rect_1,SDL_Rect & rect_2);
void mostrar_error_salir(std::string msg);

/**
 * Regresa True si rectB esta dentro rectA
 * @param rectA
 * @param rectB
 * @return
 */
bool rect_contain(SDL_Rect  rectA, SDL_Rect rectB);

inline bool punto_en_rect(Sint32 x, Sint32 y, SDL_Rect * rect_coli){return (x > rect_coli->x && x < rect_coli->x + rect_coli->w && y > rect_coli->y && y < rect_coli->y + rect_coli->h);}
/**
 * Abre un archivo de texto y busca el valor de "nombreVariable" en él
 *
 * El archivo de texto es de  la forma:
 *
 * [<nombre nombreVariable>:<valorVariable>]
 * @param ruta
 * @param nombreVariable
 * @return
 */
std::string buscar_dato(std::string ruta,std::string nombreVariable,std::string delim=":");
inline bool punto_en_rect_coordenadas(Sint16 x_1,Sint16 y_1,Sint16 x_2,Sint16 y_2,Sint16 w_2,Sint16 h_2){    return (x_1>x_2&&x_1<x_2+w_2&&y_1>y_2&&y_1<y_2+h_2);};
bool estado_direccion_joy(SDL_Keycode tecla, SDL_Joystick *joy);
Uint32 get_pixel (SDL_Surface * ima, int x, int y);
/**
 * Crea un rectangulo que CONTIENE a los dos rectangulos rect1 y rect2
 * @param rect1
 * @param rect2
 * @param rectRest
 */
void unir_rects(SDL_Rect & rect1,SDL_Rect & rect2,SDL_Rect & rectRest);
#endif