#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../../EngineMath/Matrix.h"

namespace Constants{
    namespace Projection{

        constexpr float nearPlane = 0.1f;
        constexpr float farPlane = 1000.0f;
        constexpr float q = 1 / (farPlane - nearPlane);

        constexpr float r = nearPlane * farPlane;
        constexpr float k = -2 * farPlane * q;
    }

    namespace Matrices{
        inline const Matrix<float,4,4> translationToWorldCenter = {
            1, 0, 0, -0.5,
            0, 1, 0, -0.5,
            0, 0, 1, -0.5,
            0, 0, 0, 1
        };

        inline const Matrix<float,4,4> translationToWorldCenterInverse = {
            1, 0, 0, 0.5,
            0, 1, 0, 0.5,
            0, 0, 1, 0.5,
            0, 0, 0, 1
        };
    }
}



#endif