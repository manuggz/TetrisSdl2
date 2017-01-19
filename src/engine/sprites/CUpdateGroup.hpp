//
// Created by manuggz on 21/11/16.
//

#ifndef BOMBERMAN_CUPDATEGROW_HPP
#define BOMBERMAN_CUPDATEGROW_HPP

#include "CGroup.hpp"

class UpdateGroupContainerInterfaz{
public:
    virtual void eliminarSprite(Sprite * pSprite) = 0;
};

class UpdateGroup : public Group{
public :
    UpdateGroup(UpdateGroupContainerInterfaz * parent);

    bool erase(Sprite *sprite) override ;
    /**
     * Actualiza todos los sprites
     * Itera y llama a Sprite->update
     * @param keys
     */
    void update(const Uint8 *keys);

protected:
    bool eraseSprite(Sprite *pSprite) override;

    UpdateGroupContainerInterfaz * parent;
    bool isUpdating = false;
    std::deque <Sprite * > mEliminadosEnUpdate;

};

#endif //BOMBERMAN_CUPDATEGROW_HPP
