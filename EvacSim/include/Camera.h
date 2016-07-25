#ifndef CAMERA_H
#define CAMERA_H

#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <iostream>

#include <GL/gl.h>

//#include "common_libs.h"

class Camera
{
public:
    static Camera& getInstance();

    void moveCamera()
    {
        camPosition += camSpeed;
    };
    void handleMouseMove(int mouseX, int mouseY);
    void calculateCameraMovement();
    void move();

    virtual ~Camera();
    bool holdingForward;
    bool holdingBackward;
    bool holdingLeftStrafe;
    bool holdingRightStrafe;
    glm::vec3 camPosition;
    glm::vec2 camRotation;
protected:
private:
    //glm::vec2 rotation;


    glm::vec3 camSpeed;
    glm::vec2 mouseSensivity;
    float cameraSpeedFactor;
    float movementSpeedFactor;


    Camera()
    {

        holdingForward     = false;
        holdingBackward    = false;
        holdingLeftStrafe  = false;
        holdingRightStrafe = false;

        mouseSensivity = {10.0f, 10.0f};
        movementSpeedFactor = 0.1;
        glm::vec2 v(0,0);
        v++;
        //std::cout << "--->" << rotation[1] << std::endl;

    };
};


#endif // CAMERA_H
