#include <iostream>
#include "Vector.h"

#ifndef VERTEX_H
#define VERTEX_H


template <typename ComponentType>
struct Vertex3 {
    using Vector3 = Vector<ComponentType, 3>;
    using Vector2 = Vector<ComponentType, 2>;
    Vector3 position, normal;
    Vector2 textureCoordinates;

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

    Vertex3() {
        position = Vector3();
        normal = Vector3();
        textureCoordinates = Vector2();
    }
};



#endif