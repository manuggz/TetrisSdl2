//
// Created by manuggz on 10/01/17.
//

#include "CDrawGroup.hpp"


void DrawGroup::draw(SDL_Renderer * gr){
    for (size_t i=0; i<v_personajes.size(); i++)
        v_personajes[i]->draw(gr);
}
