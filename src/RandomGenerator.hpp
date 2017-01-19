//
// Created by manuggz on 16/12/16.
//

#ifndef TETRIS_RANDOMGENERATOR_HPP
#define TETRIS_RANDOMGENERATOR_HPP

#include <vector>
#include <random>
#include <algorithm>
#include "Tetromino.hpp"

class RandomGenerator{

public:

    int getNextFormaTetromino(){
        if(mFormaRegresadaIndice >= Tetromino::N_FORMAS){
            randomBolsa();
            mFormaRegresadaIndice = -1;
        }

        return mVectorFormasBolsa[++mFormaRegresadaIndice];
    }

private:
    std::vector<int>  mVectorFormasBolsa {
            Tetromino::I,
            Tetromino::J,
            Tetromino::L,
            Tetromino::O,
            Tetromino::S,
            Tetromino::T,
            Tetromino::Z,
    };
    int mFormaRegresadaIndice = Tetromino::N_FORMAS;
    void randomBolsa(){

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(mVectorFormasBolsa.begin(), mVectorFormasBolsa.end(), g);

    }
};
#endif //TETRIS_RANDOMGENERATOR_HPP
