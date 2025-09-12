#include "Camera.h"
#include <iostream>
#include "../../Events/InputEvents.h"
#include "../../Core/Utilities/OutputFunctions.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) 
    : fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane),
      movementSpeed(5.0f), rotationSpeed(0.02f), mouseLookEnabled(false) {
    
    position = Vector<float, 3>(0.0f, 0.0f, 0.0f);
    direction = Vector<float, 3>(0.0f, 0.0f, 1.0f);
    up = Vector<float, 3>(0.0f, 1.0f, 0.0f);
    right = Vector<float, 3>(1.0f, 0.0f, 0.0f);
    
    UpdateVectors();
}

Camera::~Camera() {
    
}

void Camera::SubscribeToEvents(EventController& eventController) {
    keySubscription = eventController.Subscribe<KeyboardEvent>(
        [this](const KeyboardEvent& event) { HandleKeyboardEvent(event); });
    
    mouseMotionSubscription = eventController.Subscribe<MouseMotionEvent>(
        [this](const MouseMotionEvent& event) { HandleMouseMotion(event); });
    
    mouseButtonSubscription = eventController.Subscribe<MouseButtonEvent>(
        [this](const MouseButtonEvent& event) { HandleMouseButton(event); });
}

void Camera::UnsubscribeFromEvents(EventController& eventController) {
    eventController.Unsubscribe<KeyboardEvent>(keySubscription);
    eventController.Unsubscribe<MouseMotionEvent>(mouseMotionSubscription);
    eventController.Unsubscribe<MouseButtonEvent>(mouseButtonSubscription);
}

void Camera::HandleKeyboardEvent(const KeyboardEvent& event) {
    switch(event.key) {
        case SDLK_w: moveForward = event.pressed; break;
        case SDLK_s: moveBackward = event.pressed;  break;
        case SDLK_a: moveLeft = event.pressed;  break;
        case SDLK_d: moveRight = event.pressed; break;
        case SDLK_q: moveDown = event.pressed;  break;
        case SDLK_e: moveUp = event.pressed;  break;
        case SDLK_ESCAPE: 
            mouseLookEnabled = false;
            SDL_SetRelativeMouseMode(SDL_FALSE);
            break;
    }
}

void Camera::HandleMouseMotion(const MouseMotionEvent& event) {
    if(mouseLookEnabled) {
        float deltaX = event.relX;
        float deltaY = event.relY;
        
        Rotate(deltaX * rotationSpeed, deltaY * rotationSpeed);
    }
}

void Camera::HandleMouseButton(const MouseButtonEvent& event) {
    if(event.button == SDL_BUTTON_RIGHT) {
        mouseLookEnabled = event.pressed;
        SDL_SetRelativeMouseMode(event.pressed ? SDL_TRUE : SDL_FALSE);
        
        if(event.pressed) {
            lastMouseX = event.mouseX;
            lastMouseY = event.mouseY;
        }
    }
}

void Camera::UpdateVectors() {
    direction.Normalize();
    negativeDirection = -direction;

    right = (direction % up).Unit();
    up = (right % direction).Unit();
}

Matrix<float, 4, 4> Camera::GetViewMatrix() const {
    Vector<float, 3> target = position + direction;

    
    return {
        right[0],              right[1],                right[2],                negativePosition * right,
        up[0],                 up[1],                   up[2],                   negativePosition * up,
        negativeDirection[0],  negativeDirection[1],    negativeDirection[2],    position * direction,
        0.0f,                  0.0f,                    0.0f,                    1.0f
    };
}

Matrix<float, 4, 4> Camera::GetProjectionMatrix() const {
    float fovRad = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    float aspect = aspectRatio;

    float q = 1.0f / (farPlane - nearPlane);
    
    return {
        aspect * fovRad,   0.0f,          0.0f,              0.0f,
        0.0f,              fovRad,        0.0f,              0.0f,
        0.0f,              0.0f,          farPlane * q,     -q * farPlane * nearPlane,
        0.0f,              0.0f,          1.0f,              0.0f
    };
}

void Camera::SetPosition(const Vector<float, 3> &newPosition) {
    position = newPosition;
    negativePosition = -position;
}

void Camera::SetDirection(const Vector<float, 3> &newDirection) {
    direction = newDirection;
    UpdateVectors(); //this call normalizes the direction
}

void Camera::Move(const Vector<float, 3> &offset) {
    position += offset;
    negativePosition = -position;
}

void Camera::Rotate(float yaw, float pitch) {
    Quaternion<float> yawQuat(cos(yaw/2), 0, sin(yaw/2), 0);
    Quaternion<float> pitchQuat(cos(pitch/2), sin(pitch/2), 0, 0);
    
    Quaternion<float> rotation = yawQuat * pitchQuat;
    
    direction.RotateByQuaternion(rotation);
    
    UpdateVectors();
}

void Camera::Update(float deltaTime) {
    Vector<float, 3> moveDirection(0.0f, 0.0f, 0.0f);
    if (moveForward) moveDirection += direction;
    if (moveBackward) moveDirection -= direction;
    if (moveRight) moveDirection -= right;
    if (moveLeft) moveDirection += right;
    if (moveUp) moveDirection += up;
    if (moveDown) moveDirection -= up;

    std::cout<<moveDirection<<std::endl;

    if (moveDirection.SquaredComponentSum() > 0) {
        moveDirection.Normalize();
        Move(moveDirection * movementSpeed * deltaTime);
    }
}