#include "CSprite.hpp"
/*
Sprite::Sprite(Sint16 x,Sint16 y):
        rect(x,y){
}*/
/*
void Sprite::add(Group * nu){
    v_grupos.push_back(nu);
    nu->add(this);
}*/

bool Sprite::isKilled(){
     return mSelfKill;
}

void Sprite::kill(){

    mSelfKill=true;

    for(auto const& grupo: v_grupos) {
        grupo->eraseSprite(this);
    }

    v_grupos.clear();
}
bool Sprite::colision(SDL_Rect & rect_coli){       
    return (((rect.x+rect.w)>rect_coli.x) && \
    ((rect.y+rect.h)>rect_coli.y) &&\
    ((rect_coli.x+rect_coli.w)>rect.x) && \
    ((rect_coli.y+rect_coli.h)>rect.y));
}

void Sprite::addGroup(SpriteContainer *pGroup) {
    v_grupos.push_back(pGroup);
}

void Sprite::removeFromGroup(SpriteContainer *pGroup) {
    auto pIteGroup = v_grupos.begin();
    while(pIteGroup != v_grupos.end()){
        if(*pIteGroup == pGroup){
            v_grupos.erase(pIteGroup);
            return;
        }
        pIteGroup++;
    }
}
