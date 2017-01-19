#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "../util/util.hpp"
#include "SpriteContainer.hpp"
//#define DEBUG
/**
 * Clase Base para Todos los elementos que integran un juego
 * Inspirada de la clase Sprite de pygame.
 */
class Group;
class UpdateGroup;

class Sprite{

    friend class Group;
    friend class UpdateGroup;

  public:
     Sprite(){
    };

    /*Funciones para administrar los grupos*/
  /*  void add(Group *);*/
    void kill();

   /* void setRect(const Rect &  rect2){rect=rect2;};*/
    virtual  void move_ip(int aum_x,int aum_y){
       move(x + aum_x,y + aum_y);
   }
    virtual int getWidth(){
        return rect.w;
    }
    virtual int getHeight(){
        return rect.h;
    }
    virtual SDL_Rect getRect(){
        return rect;
    }
    virtual void update(const Uint8 *keys=nullptr)=0;
    virtual void draw(SDL_Renderer * )=0;
    //virtual int getTipo(){return type;};
    //virtual int getId(){return id;};
    //virtual void setId(int nuevo){id=nuevo;};
    virtual bool colision(SDL_Rect & rect_coli);
    virtual int getX(){return x;};
    virtual int getY(){return y;};
    virtual void setX(int nuevo){x=nuevo;};
    virtual void setY(int nuevo){y=nuevo;};
    virtual void move(int nuevaX,int nuevaY){
        x = nuevaX;
        y = nuevaY;
        rect.x = nuevaX;
        rect.y = nuevaY;
    };
//        virtual void moveIp(int,int){};

    virtual ~Sprite(){
        return;
    };

protected:
    std::vector <SpriteContainer *> v_grupos;

    bool mSelfKill = false; //sera matado?
    int x = 0,y = 0;
    SDL_Rect rect {0,0,0,0};
    //int id;
    //int type;


private:
    bool isKilled();
    void addGroup(SpriteContainer *pGroup);
    void removeFromGroup(SpriteContainer *pGroup);
};
#endif
