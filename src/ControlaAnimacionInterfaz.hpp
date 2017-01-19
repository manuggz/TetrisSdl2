//
// Created by manuggz on 13/12/16.
//

#ifndef TETRIS_CONTROLAANIMACIONINTERFAZ_HPP
#define TETRIS_CONTROLAANIMACIONINTERFAZ_HPP

class SimpleAnimacion;

class ControlaAnimacionInterfaz{
public:
    virtual void stopped(const int GrupoId){};
    virtual void paused(SimpleAnimacion *){};
};
#endif //TETRIS_CONTROLAANIMACIONINTERFAZ_HPP
