//
// Created by manuggz on 22/11/16.
//

#ifndef BOMBERMAN_CMAPA_HPP
#define BOMBERMAN_CMAPA_HPP
#include <SDL2/SDL.h>
#include "include/TMXParser.h"
#include "../util/SpriteSheet.hpp"
#include "../sprites/CDrawGroup.hpp"
#include "TileBloqueRompibleAnimado.hpp"

class Mapa: public UpdateGroupContainerInterfaz{

public:

    Mapa(int x,int y):mGrpTilesAnimados(this){
        mTmxParser = new TMX::Parser();
        mRectDest.x = x;
        mRectDest.y = y;
    }

    void eliminarSprite(Sprite *sprite) override {
        delete sprite;
    }

    /**
     * Carga los datos de un mapa de un archivo TMX
     * El archivo TMX esta creado en Tiled
     * @param gRenderer
     * @param ruta
     * @return
     */
    virtual  bool cargar(SDL_Renderer * gRenderer,std::string ruta){
        // Cargamos/ Parseamos el Mapa
        if(mIsMapaCargado) {
            delete mSprtSTiles;
            delete []mLayerMapa;
            delete mTmxParser;
            mTmxParser = new TMX::Parser();
            mGrpTilesAnimados.clear();
            mIsMapaCargado = false;

        }

        if(!mTmxParser->load(ruta.c_str())){
            return false;
        }
        mIsMapaCargado = true;
        mSprtSTiles = new SpriteSheet();
        mSprtSTiles->cargarDesdeArchivo(gRenderer,mTmxParser->tilesetList[0].imgSource.source,
                                        mTmxParser->tilesetList[0].tileCount/mTmxParser->tilesetList[0].columns,
                                        mTmxParser->tilesetList[0].columns
        );
        /**
         * Creamos el mapa con los indices
         */
        auto layer = mTmxParser->tileLayer.begin(); // Obtenemos la primera layer del mapa (solo trabajamos con ella)
        // Obtenemos el tamaño del mapa
        int size_mapa = mTmxParser->tileLayer[layer->first].width*mTmxParser->tileLayer[layer->first].height;

        mRectDest.w = mTmxParser->mapInfo.width * mTmxParser->mapInfo.tileWidth;
        mRectDest.h = mTmxParser->mapInfo.height * mTmxParser->mapInfo.tileHeight;

        mTileHeight   = mTmxParser->mapInfo.tileHeight;
        mTileWidth    = mTmxParser->mapInfo.tileWidth;
        mMapWidth     = mTmxParser->mapInfo.width;
        mMapHeight    = mTmxParser->mapInfo.height;

        // Creamos el array a contener el mapa para unr apido acceso
        mLayerMapa = new int[size_mapa] {0};

        // Parseamos el contenido del mapa del archivo
        // Debe estar encodeado con csv sin compresion
        // Al mismo tiempo que recorremos el mapa
        int  i = 0;
        // Notar el -1 al final, es porque raramente se guardan con los id's + 1, supongo el 0 representa que no
        // hay item ahí, como el mapa lo hacemos nosotros y llenamos todos los tiles podemos restar 1 tranquilos
        // Así no nos confundimos mas adelantes teniendo que sumar/restar 1 a cada rato
        mLayerMapa[i] = std::stoi(strtok((char *) mTmxParser->tileLayer[layer->first].data.contents.c_str(), ",")) - 1;
        while(++i != size_mapa) {
            mLayerMapa[i] = std::stoi(strtok(nullptr, ",")) - 1;
        }


        for(int  i = 0 ; i < size_mapa; i++){
            // Si el tile que se va a mostrar es una animación agregamos una animación ahí
            if(mTmxParser->tilesetList[0].tilesMetaData[std::to_string(mLayerMapa[i])].animation.size() > 0){

                // Buscamos la animacion del mapa para los bloques en llamas
                vector<string> *pVectTileFrames = &mTmxParser->tilesetList[0].tilesMetaData[std::to_string(mLayerMapa[i])].animation;

                // Creamos los frames de la animacion
                // notar que se ignora la duracion de los frames
                // Eso es porque aun no esta implementado
                std::string frames = "";
                auto itAnimacion = pVectTileFrames->begin();
                while(itAnimacion != pVectTileFrames->end()){
                    frames += (*itAnimacion);
                    itAnimacion++;
                    if(itAnimacion != pVectTileFrames->end()){
                        frames+=",";
                    }
                }

                TileBloqueRompibleAnimado * tileBloqueRompibleAnimado = new TileBloqueRompibleAnimado(
                        new SpriteSheet(
                                gRenderer,
                                mTmxParser->tilesetList[0].imgSource.source,
                                mTmxParser->tilesetList[0].tileCount/mTmxParser->tilesetList[0].columns,
                                mTmxParser->tilesetList[0].columns
                        ),
                        frames,
                        (i % mMapWidth)  * mTileWidth  + mRectDest.x,
                        (i / mMapWidth) * mTileHeight + mRectDest.y,
                        1
                );
                mGrpTilesAnimados.add(tileBloqueRompibleAnimado);
            }
        }

        return true;

    }
    void update(){
        mGrpTilesAnimados.update(nullptr);
    }

    virtual void draw(SDL_Renderer * gRenderer){
        int indice;
        int dest_x,dest_y;

        for(int i=0;i<mTmxParser->mapInfo.height;i++)
            for(int j=0;j<mTmxParser->mapInfo.width;j++){
                // calculo de la posici�n del tile
                dest_x = j * mTmxParser->mapInfo.tileWidth + mRectDest.x;
                dest_y = i * mTmxParser->mapInfo.tileHeight + mRectDest.y;

                indice=mLayerMapa[i*mTmxParser->mapInfo.width+j];
                /*if(indice==BLOQUE_ITEM)
                    indice=BLOQUE_MADERA;
                else if(indice==BLOQUE_ENEMIGO)
                    indice=BLOQUE_PISO;*/
                mSprtSTiles->setCurrentCuadro(indice);
                mSprtSTiles->draw(gRenderer,dest_x,dest_y);
            }

        mGrpTilesAnimados.draw(gRenderer);
    }

    virtual ~Mapa(){
        delete mTmxParser;
        delete mSprtSTiles;
    }

    const std::string &getMapProperty(
            std::basic_string<char, std::char_traits<char>, std::allocator<char>> propertyName){
        return mTmxParser->mapInfo.property[propertyName];
    }

protected:
    TMX::Parser * mTmxParser = nullptr;

    SpriteSheet *mSprtSTiles = nullptr;
    int *mLayerMapa;

    SDL_Rect mRectDest {0,0,0,0};

    unsigned int mTileHeight = 0;
    unsigned int mMapWidth   = 0;
    unsigned int mMapHeight  = 0;
    unsigned int mTileWidth  = 0;
    //std::map<std::string, TMX::Parser::Tile> *  mpTilesMetaData;

    DrawGroup mGrpTilesAnimados;

    bool mIsMapaCargado = false;
};
#endif //BOMBERMAN_CMAPA_HPP
