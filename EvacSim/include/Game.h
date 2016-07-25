#ifndef GAME_H
#define GAME_H

#include"World.h"



class Game
{
public:
    Game();
    virtual ~Game();
    World *gameWorld;  //world placed on the heap
//        Menu gameMenu;      //menu placed on stack
    void gameOn();

protected:
private:


};

#endif // GAME_H
