//
// Created by manuggz on 21/11/16.
//

#ifndef BOMBERMAN_CDRAWGROUP_HPP
#define BOMBERMAN_CDRAWGROUP_HPP

#include "CUpdateGroup.hpp"

class DrawGroup : public UpdateGroup{
public :

    //DrawGroup(InterfazSpriteGroup *parent) : UpdateGroup(parent) {}

    DrawGroup(UpdateGroupContainerInterfaz *parent) : UpdateGroup(parent) {}

    void draw(SDL_Renderer * gr);

};


#endif //BOMBERMAN_CDRAWGROUP_HPP
