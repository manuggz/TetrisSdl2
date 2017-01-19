#include "CMetaData.hpp"

MetaData::MetaData(){
    mRuta = "";
    mDelimitador = "";

}

/**
 *
 * Inicializa la clase parseando los datos en un archivo
 * Los datos del archivo deben estar en el siguiente formato

 * cada linea debe ser de la forma:

 * [<key><delimitador><value>]
 * ....
 * [<key><delimitador><value>]
 *
 * Ejemplo:
 *
 * variable1:valor1
 * Variable2:valor2
 *
 * @param ruta
 * @param delim
 * @return
 * @warning Notar que no hay espacios en el formato esto es importante porque no se hace trim en la lectura y
 * se toma tod lo que este antes del delimitador como la clave y lo que este despues como el valor incluyendo los
 * espacios.
 * Las lineas donde no se encuentren los delimitadores o n haya clave son ignoradas.
 * Distingue entre mayusculas y minusculas.
 * las claves deben ser unicas.
 */
bool MetaData::cargarMetaData(std::string ruta,std::string delim){

    std::ifstream fpArchivo(ruta);

    if(!fpArchivo){
        std::cerr << "[ERROR] leyendo archivo(MetaData):" << ruta << std::endl;
        return false;
    }

    if(mData.size() > 0 ){
        mData.clear();
    }

    std::string linea;

    int pos = -1;

    while (fpArchivo >> linea) {

        pos = (int) linea.find(delim);

        if(pos > 0){ // Linea sin delimitador o sin valor key son ignoradas
            // Notar que se toman TODOS LOS caracteres despues y antes del limitador incluyendo los espacios
            // hay que hacer un strim
            // dejo para mas adelante
            mData[linea.substr(0, (unsigned long) pos)] = linea.substr(pos + delim.size());
        }
    }

    mRuta = ruta;
    mDelimitador = delim;

    fpArchivo.close();

    return true;
}

/**
 *
 * Inicializa la clase parseando los datos en un archivo
 * Los datos del archivo deben estar en el siguiente formato

 * cada linea debe ser de la forma:

 * [<key><delimitador><value>]
 * ....
 * [<key><delimitador><value>]
 *
 * Ejemplo:
 *
 * variable1:valor1
 * Variable2:valor2
 *
 * @param ruta
 * @param delim
 * @return
 * @warning Notar que no hay espacios en el formato esto es importante porque no se hace trim en la lectura y
 * se toma tod lo que este antes del delimitador como la clave y lo que este despues como el valor incluyendo los
 * espacios.
 * Las lineas donde no se encuentren los delimitadores o n haya clave son ignoradas.
 * Distingue entre mayusculas y minusculas.
 * las claves deben ser unicas.
 */
MetaData::MetaData(std::string ruta,std::string delim){
    cargarMetaData(ruta,delim);
}

/**
 * Guarda los datos de la clase en un archivo
 * Los datos del archivo se guardaran en el siguiente formato

 * cada linea sera de la forma:

 * [<key><delimitador><value>]
 * ....
 * [<key><delimitador><value>]
 *
 * Ejemplo:
 *
 * variable1:valor1
 * Variable2:valor2
 *
 * Donde <delimitador> es el delimitador pasado como argumento en el constructor
 *
 * @param rutaDestino ruta en donde se guardaran los datos, si no se especifica se guardara en la ruta especificada
 * en el constructor
 * @return
 */
bool MetaData::guardar(std::string rutaDestino,std::string delimitador){

    std::string delimitadorAUsar;
    if(mDelimitador.empty()){
        delimitadorAUsar = delimitador;

    }else{
        delimitadorAUsar = mDelimitador;
    }

    if(rutaDestino.empty()){
        rutaDestino = mRuta;
        if(rutaDestino.empty()){
            std::cerr << "[ERROR] Error sin ruta destino de metadatos" << std::endl;
            return false;
        }
    }

    std::ofstream fnivel(rutaDestino);

    if(!fnivel){
        std::cerr << "[ERROR] Error No se logro abrir el archivo destino : " << rutaDestino <<std::endl;
        return false;
    }

    std::unordered_map<std::string,std::string>::iterator pDato = mData.begin();

    while(pDato != mData.end()){
        fnivel << (*pDato).first << delimitadorAUsar <<(*pDato).second<< std::endl;
        pDato++;

    }
    fnivel.close();
    return true;
}

/**
 * Establece el valor de un metadato
 * @param clave
 * @param valor
 */
void MetaData::setMetaData(std::string clave,std::string valor){
    mData[clave] = valor;
}

/**
 * Obtiene el valor de un metadato
 * @param clave
 * @return
 */
std::string MetaData::getMetaData(std::string clave){
    return mData[clave];
}
