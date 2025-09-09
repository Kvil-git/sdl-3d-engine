#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H


#include <math.h>
#include "../../Core/Math/Matrix.h"


namespace MathFunctions{

    namespace Matrices {
        using Matrix4x4F = Matrix<float,4,4>;

        Matrix4x4F CreateRotationMatrix(float xAngle, float yAngle, float zAngle){
            const Matrix4x4F rotationMatrix_X = {
                1.0f,    0.0f,           0.0f,        0.0f,
                0.0f,    cos(xAngle),   -sin(xAngle), 0.0f,
                0.0f,    sin(xAngle),    cos(xAngle), 0.0f,
                0.0f,    0.0f,           0.0f,        1.0f
            };

            const Matrix4x4F rotationMatrix_Y = {
                cos(yAngle),     0.0f,           sin(yAngle),        0.0f,
                0.0f,            1.0f,           0.0f,               0.0f,
                -sin(yAngle),    0.0f,           cos(yAngle),        0.0f,
                0.0f,            0.0f,           0.0f,               1.0f
            };
            
            const Matrix4x4F rotationMatrix_Z = {
                cos(zAngle),  -sin(zAngle),   0.0f,      0.0f,
                sin(zAngle),   cos(zAngle),   0.0f,      0.0f,
                0.0f,          0.0f,          1.0f,      0.0f,
                0.0f,          0.0f,          0.0f,      1.0f
            };

            return rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

        }

        Matrix4x4F CreateTranslationMatrix(float translateX, float translateY, float translateZ) {
            return {
                1.0f, 0.0f, 0.0f, translateX,
                0.0f, 1.0f, 0.0f, translateY,
                0.0f, 0.0f, 1.0f, translateZ,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
    }
}

        




#endif