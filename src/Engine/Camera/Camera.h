#ifndef CAMERA_H
#define CAMERA_H

#include "../../Core/Math/Vector.h"
#include "../../Core/Math/Matrix.h"

class Camera {
private:
    Vector<float, 3> position;
    Vector<float, 3> direction;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

public:
    Camera(float fov, float aspectRatio, float nearPlane, float farPlane) : fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane){};
    
    Matrix<float, 4, 4> GetViewMatrix() const;
    Matrix<float, 4, 4> GetProjectionMatrix() const;

    void SetPosition(const Vector<float, 3> &newPosition);
    void SetDirection(const Vector<float, 3> &direction);

    Vector<float, 3> GetPosition() const {
        return position;
    };
};

#endif