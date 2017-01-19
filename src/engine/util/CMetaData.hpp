#ifndef DAT_NIVEL_HPP
#define DAT_NIVEL_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class MetaData{
public:
    MetaData();

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
    MetaData(std::string ruta, std::string delim);
/**
 * Establece el valor de un metadato
 * @param clave
 * @param valor
 */
    void setMetaData(std::string clave, std::string);
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
 * @param rutaDestino ruta en donde se guardaran los datos, si no se especifica se guardara en la ruta donde se cargo
 * los datos(en caso que exista)
 * @return
 */
    bool guardar(std::string rutaDestino = "",std::string delimitador = ":");
/**
 * Obtiene el valor de un metadato
 * @param clave
 * @return
 */
    std::string getMetaData(std::string clave);
    bool cargarMetaData(std::string ruta, std::string delim);
private:
    std::unordered_map<std::string,std::string> mData; //Datos
    std::string mRuta; // Ruta en caso que se haya cargado los metadatos de un archivo
    std::string mDelimitador; // Delimitador de los datos

};
#endif
