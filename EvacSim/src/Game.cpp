#include "Game.h"
#include "Object.h"
#include "Renderer.h"
#include "Events.h"
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
    gameWorld = new World();
    gameWorld->startGame();
}
