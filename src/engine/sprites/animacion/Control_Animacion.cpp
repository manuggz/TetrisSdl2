#include "Control_Animacion.hpp"




ControlAnimacion::ControlAnimacion(string frames) {
    if(!frames.empty()){
        setCuadros(frames);
    }

}
void ControlAnimacion::setCuadros(string frames){
    int i = 0;
    char frames_tmp[MAX_NUM_CUADROS];
    char *proximo;    
    
    strncpy(frames_tmp, frames.c_str(),MAX_NUM_CUADROS);
    // Trabajamos con una copia de los cuadros indicados

    for(proximo = strtok(frames_tmp, ","); proximo; i++){

        // Desmembramos la cadena separada por comas

        this->cuadros[i] = atoi(proximo);
        //cout << "cuadro["<<i<<"] =" << cuadros[i] << endl;
        proximo = strtok(NULL, ",\0");
    }

    // Inicializamos las variables
    
    this->cuadros[i] = -1;
    this->paso = 0;
    inicio=0;
	
}

int ControlAnimacion::cuadro(void) {
    return cuadros[paso];
}


bool ControlAnimacion::avanzar(void) {	

    if(cuadros[++paso] == -1) {
	   paso = inicio;
	   return true;
    }
    
    return false;
}


void ControlAnimacion::reiniciar(void) {

    // Volvemos al principio

    paso = inicio;
}


bool ControlAnimacion::es_primer_cuadro(void) {

    return paso == inicio;

}

ControlAnimacion::~ControlAnimacion() {
}
