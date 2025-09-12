#ifndef CAMERA_H
#define CAMERA_H

#include "../../Core/Math/Vector.h"
#include "../../Core/Math/Matrix.h"
#include "../../Core/Math/Quaternion.h"
#include "../EventController/EventController.h"
#include "../../Events/InputEvents.h"

class Camera {
private:

    // Movement flags
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    

    Vector<float, 3> position, direction, up, right, negativeDirection, negativePosition;
    
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    
    float movementSpeed;
    float rotationSpeed;
    
    // For mouse look
    bool mouseLookEnabled;
    int lastMouseX, lastMouseY;
    
    // Event subscriptions
    size_t keySubscription;
    size_t mouseMotionSubscription;
    size_t mouseButtonSubscription;
    
    void HandleKeyboardEvent(const KeyboardEvent& event);
    void HandleMouseMotion(const MouseMotionEvent& event);
    void HandleMouseButton(const MouseButtonEvent& event);
    
    void UpdateVectors();

public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
    ~Camera();
    
    void SubscribeToEvents(EventController& eventController);
    void UnsubscribeFromEvents(EventController& eventController);
    
    Matrix<float, 4, 4> GetViewMatrix() const;
    Matrix<float, 4, 4> GetProjectionMatrix() const;

    void SetPosition(const Vector<float, 3> &newPosition);
    void SetDirection(const Vector<float, 3> &newDirection);
    
    void Move(const Vector<float, 3> &offset);
    void Rotate(float yaw, float pitch);
    
    Vector<float, 3> GetPosition() const { return position; };
    Vector<float, 3> GetDirection() const { return direction; };
    
    void Update(float deltaTime);
};

#endif