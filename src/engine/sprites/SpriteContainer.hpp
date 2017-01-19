//
// Created by manuggz on 10/01/17.
//

#ifndef BOMBERMAN_SPRITECONTAINER_HPP
#define BOMBERMAN_SPRITECONTAINER_HPP

class Sprite;
class SpriteContainer{
public:
    virtual void add(Sprite *)=0; //A�ade un sprite al grupo
    //bool contain(Sprite *);
    //Sprite * collide(Sprite *); //detecta si un sprite colisiona con uno del grupo en el caso, regresa una referencia de con quien colisiona
    virtual bool erase(Sprite *) = 0;

    virtual bool eraseSprite(Sprite *) = 0;
};
#endif //BOMBERMAN_CONTAINER_HPP
