//
// Created by manuggz on 10/01/17.
//
#include "CUpdateGroup.hpp"

UpdateGroup::UpdateGroup(UpdateGroupContainerInterfaz * parent){
    this->parent = parent;
};

bool UpdateGroup::erase(Sprite *sprite) {
    if(Group::erase(sprite)){
        if(isUpdating)
            mEliminadosEnUpdate.push_back(sprite);
        return true;
    }
    return false;
}

/**
 * Actualiza todos los sprites
 * Itera y llama a Sprite->update
 * @param keys
 */
void UpdateGroup::update(const Uint8 *keys){

    isUpdating = true;

    std::deque<Sprite * > copia            = v_personajes;
    std::deque<Sprite*>::iterator p_Sprite = copia.begin();

    while(p_Sprite != copia.end()){

        // Notar que el sprite pueda que ya no exista
        // Pueda que el Sprite haya sido eliminado en el parent o en otro grupo
        // Si ocurrio uno de los casos anteriores hay un problema de lógica del juego pero igual
        // hay que revisar

        // Si no está eliminado
        bool estaEliminado = false;
        auto iteSprEliminado = mEliminadosEnUpdate.begin();
        while(iteSprEliminado != mEliminadosEnUpdate.end()){
            if(*iteSprEliminado == *p_Sprite){
                estaEliminado = true;
                //mEliminadosEnUpdate.erase(iteSprEliminado);
                break;
            }
            iteSprEliminado++;
        }

        if(!estaEliminado){
            (*p_Sprite)->update(keys);

            // En este punto, el Sprite pudo llamar a una funcion que hace que se elimine él o otro del grupo
            // por lo que ahora p_Sprite puede apuntar a otro elemento

            //if((*p_Sprite)->isKilled()) {
            //    parent->eliminarSprite((*p_Sprite));
            //}
        }
        p_Sprite++;

    }

    auto iteSprEliminado = mEliminadosEnUpdate.begin();
    while(iteSprEliminado != mEliminadosEnUpdate.end()){
        if((*iteSprEliminado)->isKilled()){
            parent->eliminarSprite((*iteSprEliminado));
            iteSprEliminado = mEliminadosEnUpdate.erase(iteSprEliminado);
        }else{
            iteSprEliminado++;
        }
    }

    isUpdating = false;
    mEliminadosEnUpdate.clear();
}

bool UpdateGroup::eraseSprite(Sprite *pSprite) {
    if(Group::eraseSprite(pSprite)){
        if(isUpdating)
            mEliminadosEnUpdate.push_back(pSprite);
        return true;
    }
    return false;
}
