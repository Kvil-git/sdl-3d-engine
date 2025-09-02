#include <math.h>
#include <iostream>

#ifndef QUATERNION_H
#define QUATERNION_H

template <typename ComponentType>
struct Quaternion {
    ComponentType components[4];

    bool IsZero() {
        for(int i=0; i < 4; i++) if(components[i] != 0) return false;
        return true;
    }

    Quaternion(const ComponentType &a, const ComponentType &b,
               const ComponentType &c, const ComponentType &d) {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        ComponentType array[4] = {a,b,c,d};
        for(int i=0; i < 4; i++) components[i] = array[i];
    }

    Quaternion(ComponentType array[4]) {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        for(int i=0; i < 4; i++) components[i] = array[i];
    }

    Quaternion() {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        for(int i=0; i < 4; i++) components[i] = 0;
    }

    Quaternion operator*(const Quaternion &other) {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        ComponentType a1 = components[0];
        ComponentType b1 = components[1];
        ComponentType c1 = components[2];
        ComponentType d1 = components[3];
        
        ComponentType a2 = other.components[0];
        ComponentType b2 = other.components[1];
        ComponentType c2 = other.components[2];
        ComponentType d2 = other.components[3];
        
        return Quaternion (
            a1*a2 - b1*b2 - c1*c2 - d1*d2,
            a1*b2 + b1*a2 + c1*d2 - d1*c2,
            a1*c2 - b1*d2 + c1*a2 + d1*b2,
            a1*d2 + b1*c2 - c1*b2 + d1*a2
        );
    }

    Quaternion operator+(const Quaternion &other) {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        Quaternion result = *this;
        for(int i=0; i < 4; i++) result.components[i] += other.components[i];
        return result;
    }

    // Unary minus operator aka negation
    void operator-() {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        for(int i=0; i < 4; i++) components[i] = -components[i];
    }

    Quaternion operator/(float scalar) {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        Quaternion result = *this;
        for(int i=0; i < 4; i++) result.components[i] /= scalar;
        return result;
    }

    void operator/=(float scalar) {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        for(int i=0; i < 4; i++) components[i] /= scalar;
    }

    ComponentType SquaredLength() {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        if (IsZero()) return 0;
        float sum = 0;
        for(int i=0; i < 4; i++) sum += components[i] * components[i];
        return sum;
    }

    float Length() {
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        return sqrt(SquaredLength());
    }

    void Normalize(){
        static_assert(sizeof(ComponentType) > 0, "ComponentType must be a valid type.");
        float squaredLength = SquaredLength();
        if(squaredLength == 0){
            std::cout<<"Error: Cannot normalize a zero quaternion."<<std::endl;
            return;
        }
        float length = sqrt(squaredLength);
        for(int i=0; i < 4; i++) components[i] /= length;
    }
};

#endif