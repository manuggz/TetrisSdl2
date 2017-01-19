// Listado: Control_Animacion.h
//
// Esta clase controla la secuencia de animación de los personajes de la 
// aplicación

#ifndef _CONTROL_ANIMACION_HPP_
#define _CONTROL_ANIMACION_HPP_

#include <iostream>
#include <cstring>
using namespace std;

const int MAX_NUM_CUADROS = 150;

class ControlAnimacion {
 public:
    
    // Constructor
    ControlAnimacion(string frames=NULL);

    // Consultoras
    int cuadro(void);
    bool es_primer_cuadro(void);
    
    // Modificadoras
    bool avanzar(void);
    void reiniciar(void);
    void setCuadros(string frames);
    void setInicioAnimacion(int nuevo){inicio=nuevo;};

    // Destructor
    ~ControlAnimacion();
    
 private:
    int cuadros[MAX_NUM_CUADROS];
    int paso,inicio;
};
#endif
