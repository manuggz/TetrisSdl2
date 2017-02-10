//
// Created by manuggz on 10/02/17.
//

#ifndef TETRIS_CONFIGURACIONSINGLETON_HPP
#define TETRIS_CONFIGURACIONSINGLETON_HPP

#include <SDL_log.h>
#include "ConfiguracionConstantes.hpp"
#include "engine/util/CMetaData.hpp"

class ConfiguracionSingleton{
public:

    ConfiguracionSingleton(std::string ruta){
        mMetaData = new MetaData();
        mMetaData->cargarMetaData(ruta,":");
    }

    bool obtenerBooleano(const char *const clave,bool valorDefault){
        if(mMetaData->tieneClave(clave))
            return (bool) std::stoi(mMetaData->getMetaData(clave));
        return valorDefault;
    }
    int obtenerEntero(const char *const clave,int valorDefault){
        if(mMetaData->tieneClave(clave))
            return  std::stoi(mMetaData->getMetaData(clave));
        return valorDefault;
    }
    std::string obtenerString(const char *const clave,std::string valorDefault){
        if(mMetaData->tieneClave(clave))
            return mMetaData->getMetaData(clave);
        return valorDefault;
    }

    void guardarEntero(const char *const clave,int entero){
        mMetaData->setMetaData(clave,std::to_string(entero));
    }
    void guardarString(const char *const clave,std::string cadena){
        mMetaData->setMetaData(clave,cadena);
    }
    void guardarBooleano(const char *const clave,bool booleano){
        mMetaData->setMetaData(clave,std::to_string(booleano));
    }

    void guardar(){
        mMetaData->guardar();
    }
    ~ConfiguracionSingleton(){
        delete mMetaData;
    }

private:
    MetaData *mMetaData;
};

class ConfiguracionSingletonManager{
public:
    static void create(std::string ruta){
        if(!configuracionSingleton){
            configuracionSingleton = new ConfiguracionSingleton(ruta);
        }
    }

    static ConfiguracionSingleton * obtenerConfiguracion(){
        return configuracionSingleton;
    }

    static void destroy(){
        delete configuracionSingleton;
        configuracionSingleton = nullptr;
    }
private:
    static ConfiguracionSingleton * configuracionSingleton;
};

ConfiguracionSingleton * ConfiguracionSingletonManager::configuracionSingleton = nullptr;
#endif //TETRIS_CONFIGURACIONSINGLETON_HPP
