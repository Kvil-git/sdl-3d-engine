#ifndef VERTEX_H
#define VERTEX_H

#include <array>
#include "Vector.h"


template <typename ComponentType>
struct Vertex3 {
    using Vector3 = Vector<ComponentType, 3>;
    using Vector2 = Vector<ComponentType, 2>;
    Vector3 position, normal;
    Vector2 textureCoordinates;

    template<typename MatrixComponentType, int MatrixRows> // can only multiply by matrices with same width as dimensions of vectors
    Vertex3 operator*(const Matrix<MatrixComponentType, MatrixRows, 3> &matrix){
        Vertex3 copy = *this;
        copy.position *= matrix;
        copy.normal *= matrix;
        return copy;
    }

    template<typename MatrixComponentType, int MatrixRows> // can only multiply by matrices with same width as dimensions of vectors
    void operator*=(const Matrix<MatrixComponentType, MatrixRows, 3> &matrix){
        position *= matrix;
        normal *= matrix;
    }

    Vertex3(Vector3 _position, Vector3 _normal, Vector2 _textureCoordinates) {
        position = _position;
        normal = _normal;
        textureCoordinates = _textureCoordinates;
    }

    Vertex3(Vector3 _position) {
        position = _position;
        normal = Vector3();
        textureCoordinates = Vector2();
    }

    Vertex3(ComponentType x, ComponentType y, ComponentType z) {
        position = Vector3(x, y, z);
    }

    Vertex3() {
        position = Vector3();
        normal = Vector3();
        textureCoordinates = Vector2();
    }
};



#endif