#ifndef GROUP_HPP
#define GROUP_HPP
#include <SDL2/SDL.h>
#include <deque>
#include "CSprite.hpp"
#include "SpriteContainer.hpp"

class Group: public SpriteContainer{
public :
    virtual void add(Sprite *); //A�ade un sprite al grupo
    //bool contain(Sprite *);
    //Sprite * collide(Sprite *); //detecta si un sprite colisiona con uno del grupo en el caso, regresa una referencia de con quien colisiona

    virtual bool erase(Sprite *);
    virtual void clear();

    std::deque<Sprite *> collide(SDL_Rect );
    std::deque<Sprite *> collide(Sprite *);

    unsigned long size(){
        return v_personajes.size();
    }
    bool contain(Sprite *pSprite){
        auto pSpriteBusqueda = v_personajes.begin();
        while(pSpriteBusqueda != v_personajes.end()){
            if((*pSpriteBusqueda)==pSprite){
                return true;
            }
            pSpriteBusqueda++;
        }

        return false;
    }

    std::deque<Sprite *>::iterator begin(){ return v_personajes.begin();}
    std::deque<Sprite *>::iterator end(){ return v_personajes.end();}

    //void update(const Uint8 *keys= nullptr); //llama a los metodos "update" de los Sprites
    //void draw(SDL_Renderer *); // llama a los metodos "draw" con la superficie pasada
    ~Group();

    void kill();

protected:
    std::deque <Sprite * > v_personajes;

    bool eraseSprite(Sprite *pSprite) override;
};

#endif
