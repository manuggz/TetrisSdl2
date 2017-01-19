//
// Created by manuggz on 15/12/16.
//

#ifndef TETRIS_BITMAPFONT_HPP
#define TETRIS_BITMAPFONT_HPP

#include <iostream>
#include "LTexture.hpp"

class BitmapFontRenderer;
class BitmapFont{
public:

    friend class BitmapFontRenderer;
    BitmapFont(SDL_Renderer * gRenderer, std::string rutaImagen){
        mTexture = new LTexture();
        mTexture->cargarDesdeArchivo(rutaImagen,gRenderer,true);
        identificarRects();
    }

    int getHeight(){
        return mTexture->getHeight();
    }

    int calculateWidth(std::string texto){
        int dx = 0;

        char letra;
        for (int i = 0; i < texto.size(); i ++){
            letra = texto[i];
            int indice = obtener_indice(letra);
            if (letra == ' ')
                dx += fuentes [0].w;
            else{
                dx += fuentes [indice].w;
            }
        }

        return dx;
    }

private:
    LTexture *mTexture;
    SDL_Rect fuentes[94];

/*
 * Informa si la columna (parámetro 2) es una linea completa de pixeles
 * transparentes en la superficie ima (parámetro 1)
 *
 * Se asume que el color transparente es aquel que coincide con el pixel
 * situado en la posicion (0,0) de la superficie.
 */
    bool esColumnaVacia(int columna){
        Uint32 pixel_transparente = mTexture->getpixel (0, 0);

        /* busca un pixel opaco */
        for (int fila = 0; fila < mTexture->getHeight(); fila ++){
            if (pixel_transparente != mTexture->getpixel (columna, fila)) {
                return false;
            }
        }

        return true;
    }

/*
 * Relaciona un caracter con un número entero
 */
    int obtener_indice (char caracter){

        if (caracter == ' ')
            return 1;
        int indice = caracter - '!';
        //std::cout << "obtener_indice (" << caracter << ") = " <<indice << std::endl;
        if(indice>= 0 && indice < 94){
            return indice;
        }

        std::cout <<"No se puede encontrar el índice de:"<<caracter<<std::endl;
        return -1;
    }

/*
 * imprime un caracter sobre la superficie dst (generalmente screen)
 */
    int imprimir_letra (SDL_Renderer * gRenderer,int x, int y, char letra){
        SDL_Rect srcrect;
        SDL_Rect dstrect = {x, y, 0, 0};
        int indice = obtener_indice (letra);

        if (letra == ' ')
            return fuentes [0].w;


        if (indice != -1){
            srcrect = fuentes [indice];
            mTexture->draw(gRenderer,x,y,&srcrect);
            //SDL_BlitSurface(ima, &srcrect, dst, &dstrect);
        }
        return srcrect.w;
    }
/*
 * Analiza la superficie ima buscando letras para registrarlas en el
 * vector de rectángulos fuentes (segundo parámetro)
 *
 * Para analizar, el programa recorre la imágen de izquierda a derecha
 * 'leyendo' barras verticales de píxeles. Así determina donde comienza
 * y termina cada letra (debe existir una separación de, al menos, un
 * pixel entre cada caracter)
 */
    void identificarRects (){
        int indice_letra = 0;
        bool esta_recorriendo_letra = false;
        int w = 0; /* ancho de la letra actual */

        for (int i = 0; i < mTexture->getWidth(); i ++)
        {
            if (esColumnaVacia ( i))
            {

                /* si estaba leyendo una letra entonces encontró
                 * el ancho de la misma */
                if (esta_recorriendo_letra)
                {
                    fuentes[indice_letra].x=i - w;
                    fuentes[indice_letra].y=0;
                    fuentes[indice_letra].w=w;
                    fuentes[indice_letra].h=mTexture->getHeight();
                    esta_recorriendo_letra = false;
                    indice_letra ++;
                }
            }
            else
            {
                /* si está recorriendo una letra aumenta el contador
                 * de ancho, en caso contrario encontró donde comienza
                 * la siguiente letra
                 */
                if (esta_recorriendo_letra)
                    w ++;
                else
                {
                    w = 1;
                    esta_recorriendo_letra = true;
                }
            }
        }

        std::cout<<"Econtré " <<indice_letra <<" letras en el archivo de fuentes\n";
    }
};

class BitmapFontRenderer{
public:

    BitmapFontRenderer(BitmapFont * bitmapFont,int left,int top){
        mTextoMostrar = "";
        mRect.x = left;
        mRect.y = top;
        mBitmapFont = bitmapFont;

    }

    void setBitmapFont(BitmapFont * nueva){
        mBitmapFont = nueva;
        setText(mTextoMostrar);
    }
    void setText(std::string nueva){

        /*if(mTextureRendered != nullptr){
            SDL_DestroyTexture(mTextureRendered);
            mTextureRendered = nullptr;
        }*/
        mTextoMostrar = nueva;

        mRect.w = mBitmapFont->calculateWidth(mTextoMostrar);
        mRect.h = mBitmapFont->getHeight();
        if(mRight) {
            mRect.x = mRight - mRect.w;
        }
        if(mBottom) {
            mRect.y = mBottom- mRect.h;
        }
        //mTextureRendered = SDL_CreateTexture(mGRenderer,)
    }

    void setRight(int right){
        mRect.x = right - mRect.w;
        mRight = right;
    }
    void setBottom(int bottom){
        mRect.y = bottom- mRect.h;
        mBottom = bottom;
    }

    void setTop(int top){
        mRect.y = top;
        mBottom = 0;
    }

    void setLeft(int left){
        mRect.x = left;
        mRight = 0;
    }
/*
 * imprime una cadena de textos completa sobre la superficie referenciada
 * por el primer parámetro
 */
    void draw (SDL_Renderer * gRenderer){
        int i;
        int dx = mRect.x;

        for (i = 0; i < mTextoMostrar.size(); i ++){
            dx += mBitmapFont->imprimir_letra(gRenderer, dx, mRect.y, mTextoMostrar [i]);
        }
    }

    int getLeft() {
        return mRect.x;
    }
    int getTop() {
        return mRect.y;
    }
    int getHeight() {
        return mRect.h;
    }

    void draw(SDL_Renderer *pRenderer, int x, int y) {
        int i;
        int dx = x;

        for (i = 0; i < mTextoMostrar.size(); i ++){
            dx += mBitmapFont->imprimir_letra(pRenderer, dx, y, mTextoMostrar [i]);
        }

    }

    std::string getText() {
        return mTextoMostrar;
    }

private:
    BitmapFont * mBitmapFont = nullptr;
    std::string mTextoMostrar;
    SDL_Rect mRect {0,0,0,0};

    int mBottom = 0,mRight = 0;
};


#endif //TETRIS_BITMAPFONT_HPP
