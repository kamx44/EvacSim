#ifndef POLYGONGENERATOR_H
#define POLYGONGENERATOR_H

#include <vector>
#include <string>
#include <Box2D/Box2D.h>

class PolygonGenerator
{
public:
    PolygonGenerator();
    virtual ~PolygonGenerator();
    float findOneNumber(std::string napis);
    int findComma(std::string& napis);
    std::vector<b2Vec2*> findVertices(std::string napis);
    std::vector<std::vector<b2Vec2*>> getPolygonContainer(std::string fileName);

protected:
private:
    float density;
    float friction;
    float restitution;

    std::vector<std::vector<b2Vec2*>> polygon_container;
};

#endif // POLYGONGENERATOR_H
