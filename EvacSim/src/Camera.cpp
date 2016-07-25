#include "Camera.h"

Camera::~Camera()
{
    //dtor
}

Camera& Camera::getInstance()
{
    static Camera instance;
    return instance;
}

void Camera::handleMouseMove(int mouseX, int mouseY)
{

    std::cout << "Mouse cursor is at position (" << mouseX << ", " << mouseY <<std:: endl;

    glm::vec2 temp(mouseX,mouseY);
    camRotation += (temp / mouseSensivity);

    if (camRotation[0] < -90.0f)    camRotation[0] = -90.0f;
    if (camRotation[0] > 90.0f)     camRotation[0] = 90.0f;

    if (camRotation[1] < -180.0f)   camRotation[1] += 360.0f;
    if (camRotation[1] > 180.0f)    camRotation[1] -= 360.0f;
}

void Camera::move()
{

    glRotatef(camRotation[0], 1.0f, 0.0f, 0.0f);        // Rotate our camera on the x-axis (looking up and down)
    glRotatef(camRotation[1], 0.0f, 1.0f, 0.0f);        // Rotate our camera on the  y-axis (looking left and right)

    glTranslatef(-camPosition[0],-camPosition[1],-camPosition[2]);
    glTranslatef(0,0,-33);


}
void Camera::calculateCameraMovement()
{

    // Break up our movement into components along the X, Y and Z axis
    float camMovementXComponent = 0.0f;
    float camMovementYComponent = 0.0f;
    float camMovementZComponent = 0.0f;

    if (holdingForward == true)
    {
        // Control X-Axis movement
        float pitchFactor = cos(DEGTORAD(camRotation[0]));
        camMovementXComponent += ( movementSpeedFactor * float(sin(DEGTORAD(camRotation[1]))) ) * pitchFactor;

        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin(DEGTORAD(camRotation[0]))) * -1.0f;

        // Control Z-Axis movement
        float yawFactor = float(cos(DEGTORAD(camRotation[0])));
        camMovementZComponent += ( movementSpeedFactor * float(cos(DEGTORAD(camRotation[1]))) * -1.0f ) * yawFactor;
    }

    if (holdingBackward == true)
    {
        // Control X-Axis movement
        float pitchFactor = cos(DEGTORAD(camRotation[0]));
        camMovementXComponent += ( movementSpeedFactor * float(sin(DEGTORAD(camRotation[1]))) * -1.0f) * pitchFactor;

        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin(DEGTORAD(camRotation[0])));

        // Control Z-Axis movement
        float yawFactor = float(cos(DEGTORAD(camRotation[0])));
        camMovementZComponent += ( movementSpeedFactor * float(cos(DEGTORAD(camRotation[1]))) ) * yawFactor;
    }

    if (holdingLeftStrafe == true)
    {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = DEGTORAD(camRotation[1]);

        camMovementXComponent += -movementSpeedFactor * float(cos(yRotRad));
        camMovementZComponent += -movementSpeedFactor * float(sin(yRotRad));
    }

    if (holdingRightStrafe == true)
    {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = DEGTORAD(camRotation[1]);

        camMovementXComponent += movementSpeedFactor * float(cos(yRotRad));
        camMovementZComponent += movementSpeedFactor * float(sin(yRotRad));
    }

    // After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis

    float camXSpeed = camMovementXComponent;
    float camYSpeed = camMovementYComponent;
    float camZSpeed = camMovementZComponent;

    // Cap the speeds to our movementSpeedFactor (otherwise going forward and strafing at an angle is twice as fast as just going forward!)
    // X Speed cap
    if (camXSpeed > movementSpeedFactor)
    {
        //cout << "high capping X speed to: " << movementSpeedFactor << endl;
        camXSpeed = movementSpeedFactor;
    }
    if (camXSpeed < -movementSpeedFactor)
    {
        //cout << "low capping X speed to: " << movementSpeedFactor << endl;
        camXSpeed = -movementSpeedFactor;
    }

    // Y Speed cap
    if (camYSpeed > movementSpeedFactor)
    {
        //cout << "low capping Y speed to: " << movementSpeedFactor << endl;
        camYSpeed = movementSpeedFactor;
    }
    if (camYSpeed < -movementSpeedFactor)
    {
        //cout << "high capping Y speed to: " << movementSpeedFactor << endl;
        camYSpeed = -movementSpeedFactor;
    }

    // Z Speed cap
    if (camZSpeed > movementSpeedFactor)
    {
        //cout << "high capping Z speed to: " << movementSpeedFactor << endl;
        camZSpeed = movementSpeedFactor;
    }
    if (camZSpeed < -movementSpeedFactor)
    {
        //cout << "low capping Z speed to: " << movementSpeedFactor << endl;
        camZSpeed = -movementSpeedFactor;
    }

    camSpeed[0] = camXSpeed;
    camSpeed[1] = camYSpeed;
    camSpeed[2] = camZSpeed;

}
