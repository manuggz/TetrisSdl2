#include "util.hpp"

template <typename T> bool dato_en_array(T dato,T array[],int N){
    int i;
    
     for(i=0;i<N;i++)
        if(array[i]==dato)
           return true;
           
     return false;
     
}

void sort_array(int array_sort[5],int destino_sort[5]){
     int i,index,mayores[5];

     for(i=0;i<5;i++){
        destino_sort[i]=-1000;
        mayores[i]=-1000;
     }
        
     
     for(index=0;index<5;index++){
       for(i=0;i<5;i++){
             if(array_sort[i]>mayores[index]&&!dato_en_array(i,destino_sort,5)){
                destino_sort[index]=i;
                mayores[index]=array_sort[i];
             }
       }
    }
}



void mostrar_error_salir(std::string msg){
    std::cerr << "Error:"<<msg<<"Error SDL:"<<SDL_GetError()<<std::endl;
    exit(1);
}

Mix_Chunk * cargar_sonido(char ruta[]){
    Mix_Chunk * cargado;
    cargado=Mix_LoadWAV(ruta);
    if(cargado==NULL){
        std::cerr<<"Error cargando sonido:"<<ruta<<Mix_GetError()<<std::endl;
        exit(1);
    }
    std::cout << "+ cargando:"<<ruta<<std::endl;
	return cargado;
}
Mix_Music * cargar_musica(const char ruta[]){
    Mix_Music * cargado;
    cargado=Mix_LoadMUS(ruta);
    if(cargado==NULL){
        std::cerr<<"Error cargando musica:"<<ruta<<Mix_GetError()<<std::endl;
        exit(1);
    }
    std::cout << "+ cargando:"<<ruta<<std::endl;
	return cargado;
}

bool rects_colisionan(SDL_Rect & rect_1,SDL_Rect & rect_2)
{
    
    return (((rect_1.x+rect_1.w)>rect_2.x) && \
    ((rect_1.y+rect_1.h)>rect_2.y) &&\
    ((rect_2.x+rect_2.w)>rect_1.x) && \
    ((rect_2.y+rect_2.h)>rect_1.y)) ;
}


void imprimir_desde_grilla(LTexture * src, int cuadro, SDL_Renderer *gRenderer,
                           int x_dest,int y_dest, int fil, int col,int alpha) {
	SDL_Rect srcrect,dest_rect={x_dest,y_dest,0,0};

	srcrect.w = src->getWidth() / col;
	srcrect.h = src->getHeight() / fil;
	srcrect.x = (cuadro % col) * srcrect.w;
	srcrect.y = (cuadro / col) * srcrect.h;

	if(alpha){
        src->setAlpha(150);
    }else
        src->setAlpha(255);

    src->draw(gRenderer, x_dest, y_dest, &srcrect);
}



/*
 * imprime un caracter en el renderer
 */
int imprimir_letra (SDL_Renderer * gRenderer, LTexture * textureLetras,int x, int y, char letra,std::string orden_letras) {
	SDL_Rect srcrect;

    std::cout << "imprimir letra: x " << x << " y: " << y << "letra: " << letra << "orden_letras: "<<orden_letras<< std::endl;


	srcrect.y=0;
	srcrect.h=textureLetras->getHeight();
    srcrect.w= (int) (textureLetras->getWidth() / orden_letras.size());
    srcrect.x= (int) (srcrect.w * orden_letras.find(letra));

	if(srcrect.x>=0){
        std::cout << "srcrect.x " << srcrect.x
            << "srcrect.y " << srcrect.y
            << "srcrect.w " << srcrect.w
            << "srcrect.h " << srcrect.h
             << std::endl;
        textureLetras->draw(gRenderer, x, y, &srcrect);
    }

	return srcrect.w;
}


/*
 * imprime una cadena de textos completa sobre la superficie referenciada
 * por el primer par�metro
 */
void imprimir_palabra (SDL_Renderer * gRenderer, LTexture * textureLetras, int x, int y,std::string cadena,std::string orden_letras) {
    std::cout << "imprimir palabra x: " << x << " y: " << y << " cadena: " << cadena << std::endl;
	int i;
	int dx = x;

	for (i = 0; i < cadena.size(); i ++)
		dx += imprimir_letra (gRenderer, textureLetras, dx, y, cadena[i],orden_letras);
}

void mostrar_msg (SDL_Renderer * gRenderer, LTexture * txtLetras, int x,int y,const char * orden_letras, char * formato, ...)
{
    va_list lista;
    char buffer [1024];
    va_start (lista, formato);
        vsprintf (buffer, formato, lista);
        imprimir_palabra (gRenderer, txtLetras, x, y,orden_letras, buffer);
    va_end (lista);
}

Uint32 get_pixel (SDL_Surface * ima, int x, int y)
{
	int bpp = ima->format->BytesPerPixel;
	Uint8 *p = (Uint8 *) ima->pixels + y * ima->pitch + x * bpp;

	switch (bpp)
	{
		case 1:
			return *p;
		
		case 2:
			return *(Uint16 *)p;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;
	}
}

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
std::string buscar_dato(std::string ruta,std::string nombreVariable,std::string delim){

    std::ifstream fpArchivo(ruta);

    if(!fpArchivo){
        std::cerr << "Error leyendo archivo(Buscar Dato):" << ruta << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string linea;
    //std::string valorVariableFile;

    int pos;
    while (fpArchivo >> linea){
        pos = (int) linea.find(nombreVariable);

        if(pos >= 0){
            pos = (int) linea.find(delim);
            return linea.substr(pos + delim.size());
        }

    }
    fpArchivo.close();
    return nullptr;
}

bool estado_direccion_joy(SDL_Keycode tecla, SDL_Joystick *joy){
	switch(tecla){
		case SDLK_LEFT:
				return SDL_JoystickGetAxis(joy, 0) < -10;
			break;
		case SDLK_RIGHT:
				return SDL_JoystickGetAxis(joy, 0) > 10;
			break;
		case SDLK_UP:
				return SDL_JoystickGetAxis(joy, 1) <-10;
			break;
		case SDLK_DOWN:
				return SDL_JoystickGetAxis(joy, 1) > 10;
			break;
		default: return false;

	}
}

/**
 * Crea un rectangulo que CONTIENE a los dos rectangulos rect1 y rect2
 * @param rect1
 * @param rect2
 * @param rectRest
 */
void unir_rects(SDL_Rect & rect1,SDL_Rect & rect2,SDL_Rect & rectRest){
    rectRest.x = std::min(rect1.x,rect2.x);
    rectRest.y = std::min(rect1.y,rect2.y);
    rectRest.w = std::max(rect1.x + rect1.w - rectRest.x,rect2.x + rect2.w - rectRest.x);
    rectRest.h = std::max(rect1.y + rect1.h - rectRest.y,rect2.h + rect2.h - rectRest.y);
}


bool rect_contain(SDL_Rect rectA, SDL_Rect rectB) {
    return  rectB.x >= rectA.x &&
            rectB.y >= rectA.y &&
            rectB.x + rectB.w <= rectA.x + rectA.w &&
            rectB.y + rectB.h <= rectA.y + rectA.h;
}


