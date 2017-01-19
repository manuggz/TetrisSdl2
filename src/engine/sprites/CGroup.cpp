#include "CGroup.hpp"


void Group::add(Sprite * pSprite){
    v_personajes.push_back(pSprite);
    pSprite->addGroup(this);
}

bool Group::erase(Sprite * pSpriteBorrar){
     //lo buscamos usando iteradores

    // Si se esta actualizando el grupo de sprites no se hacen eliminaciones
    // Este sprite se debería eliminar en la actualizacion si se establece selfkill
//    if(isUpdating)
  //      return;

    auto pSpriteBusqueda = v_personajes.begin();
    while(pSpriteBusqueda != v_personajes.end()){
        if((*pSpriteBusqueda) == pSpriteBorrar){
            v_personajes.erase(pSpriteBusqueda);
            // Remover Este grupo de los grupos que guarda (*pSpriteBusqueda)
            (*pSpriteBusqueda)->removeFromGroup(this);
            return true;
        }
        pSpriteBusqueda++;
    }

    return false;
}

/**
 * Función llamada por Sprite, para que solo se elimine él del grupo.
 * @param pSprite
 */
bool Group::eraseSprite(Sprite * pSprite){
    auto pIteSprite = v_personajes.begin();
    while(pIteSprite != v_personajes.end()){
        if((*pIteSprite) == pSprite){
            v_personajes.erase(pIteSprite);
            return true;
        }
        pIteSprite++;
    }
    return false;
}
Group::~Group(){
    ///std::deque<Sprite *> copia = v_personajes;
    v_personajes.clear();

    //auto pSprite = copia.begin();
    //while(pSprite != copia.end()){
        // Remover Este grupo de los grupos que guarda (*pSprite)
        // Antes de llamar a kill, porque en kill el sprite hace un bucle por todos los grupos que lo incluyen
        // incluyendonos
    //    (*pSprite)->removeFromGroup(this);
    //    pSprite++;
   // }
}

std::deque<Sprite *> Group::collide(SDL_Rect  rect) {
    std::deque<Sprite *> setColision;
    auto pSpriteBusqueda = v_personajes.begin();
    while(pSpriteBusqueda != v_personajes.end()){
        if((*pSpriteBusqueda)->colision(rect)){
            setColision.push_back((*pSpriteBusqueda));
        }
        pSpriteBusqueda++;
    }

    return setColision;
}

std::deque<Sprite *> Group::collide(Sprite * sprite) {
    return collide(sprite->rect);
}

void Group::clear() {
    std::deque<Sprite *> copia = v_personajes;
    v_personajes.clear();

    auto pSprite = copia.begin();
    while(pSprite != copia.end()){
        // Remover Este grupo de los grupos que guarda (*pSprite)
        (*pSprite)->removeFromGroup(this);
        pSprite++;
    }
}

void Group::kill() {
    std::deque<Sprite *> copia = v_personajes;
    v_personajes.clear();

    auto pSprite = copia.begin();
    while(pSprite != copia.end()){
        // Remover Este grupo de los grupos que guarda (*pSprite)
        // Antes de llamar a kill, porque en kill el sprite hace un bucle por todos los grupos que lo incluyen
        // incluyendonos
        (*pSprite)->removeFromGroup(this);
        (*pSprite)->kill();
        pSprite++;
    }
}
