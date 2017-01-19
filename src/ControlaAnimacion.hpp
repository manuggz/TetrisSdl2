//
// Created by manuggz on 13/12/16.
//

#ifndef TETRIS_CONTROLAANIMACION_HPP
#define TETRIS_CONTROLAANIMACION_HPP

#include <list>
#include <SDL.h>
#include <unordered_map>
#include "ControlaAnimacionInterfaz.hpp"
#include "SimpleAnimacion.hpp"

class ControlaAnimacion{
public:
    ControlaAnimacion(ControlaAnimacionInterfaz*  parent){
        mParent = parent;
    }

    void add(int groupId,SimpleAnimacion* nuevaAnimacion) {

        nuevaAnimacion->start();
        dequeGrupoAnimaciones[groupId].push_back(nuevaAnimacion);
    }

    void erase(int grupoId,SimpleAnimacion * pSprite){

		auto itGrupo = dequeGrupoAnimaciones.find(grupoId);

		// Si no existe el grupo
		if (itGrupo == dequeGrupoAnimaciones.end()) return;
		
		(*itGrupo).second.remove(pSprite);

		if ((*itGrupo).second.empty()) {
			dequeGrupoAnimaciones.erase(itGrupo);
		}
    }

    int numeroAnimacionesActivas(const int grupoId) {
        return (int) dequeGrupoAnimaciones[grupoId].size();
    }

    void update(){
        
		auto dequeCopia = dequeGrupoAnimaciones;
		
		for (auto & grupoAnimacion : dequeCopia) {
			for (auto & animacion : grupoAnimacion.second) {
				animacion->update();

				if (animacion->isStopped()) {				
					erase(grupoAnimacion.first, animacion);
					delete animacion;
					if (numeroAnimacionesActivas(grupoAnimacion.first) == 0) {
						mParent->stopped(grupoAnimacion.first);
					}
				}

			}
		}
    }

    void draw(SDL_Renderer * gRenderer){
		for (auto & grupoAnimacion : dequeGrupoAnimaciones) {
			for (auto & animacion : grupoAnimacion.second) {
				animacion->draw(gRenderer);
			}
		}
		
    }

	~ControlaAnimacion(){

		for (auto & grupoAnimacion : dequeGrupoAnimaciones) {
			for (auto & animacion : grupoAnimacion.second) {
				delete animacion;
			}
		}
    }

private:

    std::unordered_map<int,std::list<SimpleAnimacion *>> dequeGrupoAnimaciones;
    ControlaAnimacionInterfaz *mParent;
};
#endif //TETRIS_CONTROLAANIMACION_HPP
