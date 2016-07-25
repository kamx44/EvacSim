#include "Game.h"
#include "Object.h"
#include "Renderer.h"
#include "Camera.h"
#include "Events.h"
#include "PolygonGenerator.h"
#include <Box2D/Box2D.h>

Game::Game()
{

}

Game::~Game()
{
    //dtor
}

void Game::gameOn()
{
    int state = 0;

//    Player *pl;
    gameWorld = new World();
    gameWorld->startGame();
    state = 2;


}
