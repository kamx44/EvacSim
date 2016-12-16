#include "Exit.h"

Exit::Exit(glm::vec2 startPosition,glm::vec2 endPosition) : Wall(startPosition,endPosition)
{
    //ctor
    Wall::object_type = OBJECT_TYPE::WALL_EXIT;
    body->DestroyFixture(fixture);
    b2EdgeShape shape;
    vertices = new b2Vec2[vertexCount];
    vertices[0].Set(startPosition.x, startPosition.y);
    vertices[1].Set(endPosition.x, endPosition.y);
    shape.Set(vertices[0],vertices[1]);
    defineFixture(shape);
    body->CreateFixture(&fixtureDef);
    drawable=false;
    mainExit = false;
}

Exit::~Exit()
{
    //dtor
}

void Exit::defineFixture(b2EdgeShape& shape){
    fixtureDef.shape = &shape;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = false;
    fixtureDef.filter.categoryBits = 0x0040;
    fixtureDef.filter.maskBits = 0x0008 | 0x00010;
}

b2Vec2 Exit::getMiddlePoint(){
    return b2Vec2((vertices[0].x+vertices[1].x)/2, (vertices[0].y+vertices[1].y)/2);
}
