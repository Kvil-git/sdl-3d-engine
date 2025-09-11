#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H


#include <math.h>
#include "../../Core/Math/Matrix.h"


namespace MathFunctions{
    namespace Polygons {
            
        template<typename ComponentType> bool IsPointInTriangle2D
        (   const Vector<ComponentType, 2>& p,
            const Vector<ComponentType, 2>& a,
            const Vector<ComponentType, 2>& b,
            const Vector<ComponentType, 2>& c   ) {
                
            // Compute vectors
            Vector<ComponentType, 2> v0 = c - a;
            Vector<ComponentType, 2> v1 = b - a;
            Vector<ComponentType, 2> v2 = p - a;

            // Compute dot products
            ComponentType dot00 = v0 * v0;
            ComponentType dot01 = v0 * v1;
            ComponentType dot02 = v0 * v2;
            ComponentType dot11 = v1 * v1;
            ComponentType dot12 = v1 * v2;

            // Compute barycentric coordinates
            ComponentType invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            ComponentType u = (dot11 * dot02 - dot01 * dot12) * invDenom;
            ComponentType v = (dot00 * dot12 - dot01 * dot02) * invDenom;

            // Add a small epsilon for floating point precision
            const ComponentType epsilon = static_cast<ComponentType>(1e-10);
            
            // Check if point is in triangle
            return (u >= -epsilon) && (v >= -epsilon) && (u + v <= 1 + epsilon);
        }

        template<typename ComponentType, typename VertexContainer>
        std::vector<std::array<typename VertexContainer::value_type, 3>> Triangulate
        (   const VertexContainer& vertices,
            const Vector<ComponentType, 3>& normal  ) {
                
            using VertexType = typename VertexContainer::value_type;
            std::vector<std::array<VertexType, 3>> triangles;
            const size_t vertexCount = vertices.size();
            
            if (vertexCount < 3) return triangles;

            // Handle triangle case
            if (vertexCount == 3) {
                triangles.push_back({vertices[0], vertices[1], vertices[2]});
                return triangles;
            }

            // Project vertices to 2D
            int axis1 = 0, axis2 = 1;
            ComponentType maxNormal = std::abs(normal[0]);
            if (std::abs(normal[1]) > maxNormal) {
                maxNormal = std::abs(normal[1]);
                axis1 = 0;
                axis2 = 2;
            }
            if (std::abs(normal[2]) > maxNormal) {
                axis1 = 1;
                axis2 = 2;
            }

            std::vector<Vector<ComponentType, 2>> points2D;
            std::vector<size_t> indices;
            for (size_t i = 0; i < vertexCount; ++i) {
                points2D.push_back({
                    vertices[i].position[axis1],
                    vertices[i].position[axis2]
                });
                indices.push_back(i);
            }

            // Ear clipping algorithm
            while (indices.size() > 3) {
                bool earFound = false;
                
                for (size_t i = 0; i < indices.size(); ++i) {
                    size_t prev = (i == 0) ? indices.size() - 1 : i - 1;
                    size_t next = (i + 1) % indices.size();

                    auto& a = points2D[indices[prev]];
                    auto& b = points2D[indices[i]];
                    auto& c = points2D[indices[next]];

                    // Check convexity
                    Vector<ComponentType, 2> ab = b - a;
                    Vector<ComponentType, 2> bc = c - b;
                    ComponentType cross = ab[0] * bc[1] - ab[1] * bc[0];
                    
                    if (cross <= 0) continue;

                    // Check if ear
                    bool isEar = true;
                    for (size_t j = 0; j < indices.size(); ++j) {
                        if (j == prev || j == i || j == next) continue;
                        
                        if (IsPointInTriangle2D(points2D[indices[j]], a, b, c)) {
                            isEar = false;
                            break;
                        }
                    }

                    if (isEar) {
                        triangles.push_back({
                            vertices[indices[prev]],
                            vertices[indices[i]],
                            vertices[indices[next]]
                        });
                        
                        indices.erase(indices.begin() + i);
                        earFound = true;
                        break;
                    }
                }

                if (!earFound) {
                    // Fallback to fan triangulation
                    for (size_t i = 1; i < indices.size() - 1; ++i) {
                        triangles.push_back({
                            vertices[indices[0]],
                            vertices[indices[i]],
                            vertices[indices[i + 1]]
                        });
                    }
                    break;
                }
            }

            // Add final triangle
            if (indices.size() == 3) {
                triangles.push_back({
                    vertices[indices[0]],
                    vertices[indices[1]],
                    vertices[indices[2]]
                });
            }

            return triangles;
        }

    };

    namespace Matrices {
        using Matrix4x4F = Matrix<float,4,4>;

        // Only for Rotation or Translation Matrices
        Matrix4x4F QuickMatrixInverse(const Matrix4x4F &input){ 
            Matrix4x4F output;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    output(i, j) = input(j, i);
                }
            }
            for(int i=0; i<3; i++){
                float sum = 0.0f;
                for(int j=0; j<3; j++){
                    sum += output(3, j) * input(j, i);
                }
                output(3, i) = -sum;
            }
            return output;
        }

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