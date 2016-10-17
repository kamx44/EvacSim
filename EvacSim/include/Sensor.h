#ifndef SENSOR_H
#define SENSOR_H
#include "Object.h"

enum sensorType{
    COMMUNICATE,
    SIGHT
};

class Sensor : public Object
{
    public:
        sensorType type;
        Sensor(OBJECT_TYPE sensor_type,glm::vec2 position,float radius,unsigned int idParent,b2Vec2* parentForce,std::vector<unsigned int>& visibleObjects,sensorType type);
        virtual ~Sensor();
        void update(float dt);
        void draw();
        virtual void delObject()
        {
            //world_actor->delFromObjectsContainer(this);
        }
        b2Body* getBody();
        void addVisibleObject(unsigned int visibleObjectId);
        void removeInvisibleObject(unsigned int visibleObjectId);
        unsigned int getParentId();
        void printData(); //for testing
        void setParentVelocity(b2Vec2 velocity);
        void setDrawable(bool draw);


      /* void setVelocity(b2Vec2 vel){
            b
        } */
        bool rotateBody;
    protected:
    private:
        int32 size;
        b2CircleShape circle;
        float radius;
        unsigned int idParent;
        std::vector<unsigned int>* visibleObjects;
        float radiusMovable;
        b2Vec2* pushForce;

};

#endif // SENSOR_H
