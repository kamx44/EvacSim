#ifndef PLAYER_H
#define PLAYER_H
#include<string>
#include<vector>
#include"Ship.h"


class Player
{
public:
    Player(std::string name = "Player1");
    virtual ~Player();
    std::string name;
    std::vector<Ship*> playerShips;
    Ship* mainShip;

protected:
private:

};

#endif // PLAYER_H
