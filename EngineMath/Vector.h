#include <math.h>
#include <iostream>
#include <array>
#include <vector>

#ifndef MATRIX_H
template <typename ElementType, size_t Rows, size_t Columns> struct Matrix;
#endif

#ifndef QUATERNION_H
template <typename ComponentType> struct Quaternion;
#endif

#ifndef VECTOR_H
#define VECTOR_H

template <typename ComponentType, size_t Dimensions>
struct Vector {
    ComponentType components[Dimensions];
    using VectorN = Vector<ComponentType, Dimensions>;
    using Vector3 = Vector<ComponentType, 3>;

    // matrix-vector multiplication
    template<typename MatrixType> VectorN operator*(const Matrix<MatrixType, Dimensions, Dimensions> &matrix) {
        static_assert(Dimensions > 0, "Matrix-vector multiplication is not defined for zero-dimensional vectors.");
        VectorN temp;
        for(int i=0; i < Dimensions; i++) {
            for(int j=0; j < Dimensions; j++) {
                temp.components[i] += matrix.components[i * Dimensions + j] * components[j];
            }
        }
        return temp;
    }

    // matrix-vector multiplication
    template<typename MatrixType> void operator*=(const Matrix<MatrixType, Dimensions, Dimensions> &matrix) {
        static_assert(Dimensions > 0, "Matrix-vector multiplication is not defined for zero-dimensional vectors.");
        VectorN temp;
        for(int i=0; i < Dimensions; i++) {
            for(int j=0; j < Dimensions; j++) {
                temp.components[i] += matrix.components[i * Dimensions + j] * this->components[j];
            }
        }
        *this = temp;
    }

    // Apply a rotation stored in unit quaternion to a 3D vector
    template<typename QuaternionType>
    Vector3 RotateByQuaternion(const QuaternionType &quaternion) {
        static_assert(Dimensions == 3, "Rotation by quaternion is only defined for 3D vectors.");

        QuaternionType q = quaternion;
        QuaternionType q_conjugate = {q.components[0], -q.components[1], -q.components[2], -q.components[3]};

        QuaternionType p = {0, components[0], components[1], components[2]};

        QuaternionType result = q * p * q_conjugate;

        return Vector3({result.components[1], result.components[2], result.components[3]});
    }

    // vector addition just means adding corresponding coords 
    Vector operator+(const VectorN &other) {
        static_assert(Dimensions > 0, "Vector addition is not defined for zero-dimensional vectors.");
        Vector<ComponentType,Dimensions> temp;
        for(int i=0; i < Dimensions; i++) temp.components[i] = components[i] + other.components[i];
        return temp;
    }

    // addition with assignment
    void operator+=(const VectorN &other) {
        static_assert(Dimensions > 0, "Vector addition is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) components[i] += other.components[i];
    }

    // unary minus, basically vector negation
    Vector operator-() const {
        static_assert(Dimensions > 0, "Vector negation is not defined for zero-dimensional vectors.");
        VectorN temp;
        for(int i=0; i < Dimensions; i++) temp.components[i] = -components[i];
        return temp;
    }

    // actual vector subtraction is basically addition of a negated vector
    Vector operator-(const VectorN &other) {
        static_assert(Dimensions > 0, "Vector subtraction is not defined for zero-dimensional vectors.");
        return (*this) + (-other);
    }

    // vector scaling
    template<typename scalarType> VectorN operator*(const scalarType &scalar) {
        static_assert(Dimensions > 0, "Vector scaling is not defined for zero-dimensional vectors.");
        VectorN temp;
        for(int i=0; i < Dimensions; i++) temp.components[i] = components[i] * scalar;
        return temp;
    }

    // vector scaling with assignment
    template<typename scalarType> void operator*=(const scalarType &scalar) {
        static_assert(Dimensions > 0, "Vector scaling is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) components[i] *= scalar;
    }

    // vector scaling 2
    template<typename scalarType> Vector<ComponentType,Dimensions> operator/(const scalarType &scalar) {
        static_assert(Dimensions > 0, "Vector scaling is not defined for zero-dimensional vectors.");
        VectorN temp;
        for(int i=0; i < Dimensions; i++) temp.components[i] = components[i] / scalar;
        return temp;
    }

    // vector scaling 2 with assignment
    template<typename scalarType> void operator/=(const scalarType &scalar) {
        static_assert(Dimensions > 0, "Vector scaling is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) components[i] /= scalar;
    }

    // vector length and magnitude are the same thing idgaf
    double Length() {
        static_assert(Dimensions > 0, "Length is not defined for zero-dimensional vectors.");
        return sqrt(this->SquaredComponentSum());
    }

    double Mangitude() {
        return this->Length();
    }

    // sum of a vector's squared components is its length squared
    ComponentType SquaredComponentSum() {
        static_assert(Dimensions > 0, "Squared component sum is not defined for zero-dimensional vectors.");
        ComponentType squaredComponentSum = 0;
        for(int i=0; i < Dimensions; i++) squaredComponentSum += components[i] * components[i];
        return squaredComponentSum;
    }

    ComponentType LengthSquared() {
        return this->SquaredComponentSum();
    }

    ComponentType MangitudeSquared() {
        return this->SquaredComponentSum();
    }

    // dot product of two vectors
    ComponentType operator*(const VectorN &other) {
        static_assert(Dimensions > 0, "Dot product is not defined for zero-dimensional vectors.");
        size_t size = Dimensions;
        ComponentType dotProduct = 0;
        for(int i=0; i < size; i++) dotProduct+= components[i] * other.components[i];
        return dotProduct;
    }

    // cross product
    Vector3 operator%(const Vector<ComponentType,Dimensions> &other) {
        static_assert(Dimensions == 3, "Cross product is only defined for 3D vectors.");
        
        ComponentType v1 = components[0];
        ComponentType v2 = components[1];
        ComponentType v3 = components[2];

        ComponentType w1 = other.components[0];
        ComponentType w2 = other.components[1];
        ComponentType w3 = other.components[2];

        return Vector3({v2*w3 - v3*w2, v3*w1 - v1*w3, v1*w2 - v2*w1});
    }

    ComponentType ComponentSum() {
        static_assert(Dimensions > 0, "Component sum is not defined for zero-dimensional vectors.");
        ComponentType sum = 0;
        for(int i=0; i < Dimensions; i++) sum += components[i];
        return sum;
    }

    inline bool IsZero(){
        return this->ComponentSum() == 0;
    }

    // turn this vector into a unit vector
    void Normalize() {
        static_assert(Dimensions > 0, "Normalization is not defined for zero-dimensional vectors.");
        *this /= this->Length();
    }    

    // return a normalized copy of this vector
    VectorN Unit() {
        static_assert(Dimensions > 0, "Unit vector is not defined for zero-dimensional vectors.");
        return *this / this->Length();
    }


    // N dimensional zero vector constructor
    Vector() {
        static_assert(Dimensions > 0, "Zero vector is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) components[i] = 0;
    }

    // N dimensional nonzero vector constructor
    Vector(ComponentType _components[Dimensions]) {
        static_assert(Dimensions > 0, "Nonzero vector is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) components[i] = _components[i];
    }

    // N dimensional nonzero vector constructor
    Vector(std::array<ComponentType, Dimensions> _components) {
        static_assert(Dimensions > 0, "Nonzero vector is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) components[i] = _components[i];
    }

    // copy constructor
    Vector(const VectorN &other) {
        static_assert(Dimensions > 0, "Copy constructor is not defined for zero-dimensional vectors.");
        for(int i=0; i < Dimensions; i++) this->components[i] = other.components[i];
    }

};

#endif