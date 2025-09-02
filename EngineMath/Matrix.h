#include <iostream>
#include <array>

#ifndef VECTOR_H
template <typename ElementType, size_t Dimensions> struct Vector;
#endif

#ifndef MATRIX_H
#define MATRIX_H

template <typename ElementType, size_t Rows, size_t Columns>
struct Matrix {
    using MatrixNxN = Matrix<ElementType, Rows, Columns>;
    ElementType elements[Rows * Columns];

    // matrix addition
    MatrixNxN operator+(const MatrixNxN &other) {
        static_assert(Rows > 0 && Columns > 0, "Matrix addition is not defined for zero-dimensional matrices.");
        MatrixNxN temp;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] = elements[i] + other.elements[i];
        return temp;
    }

    // addition with assignment
    void operator+=(const MatrixNxN &other) {
        static_assert(Rows > 0 && Columns > 0, "Matrix addition is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] += other.elements[i];
    }

    // matrix subtraction
    MatrixNxN operator-(const MatrixNxN &other) {
        static_assert(Rows > 0 && Columns > 0, "Matrix subtraction is not defined for zero-dimensional matrices.");
        MatrixNxN temp;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] = elements[i] - other.elements[i];
        return temp;
    }

    // subtraction with assignment
    void operator-=(const MatrixNxN &other) {
        static_assert(Rows > 0 && Columns > 0, "Matrix subtraction is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] -= other.elements[i];
    }

    // matrix negation
    MatrixNxN operator-() {
        static_assert(Rows > 0 && Columns > 0, "Matrix negation is not defined for zero-dimensional matrices.");
        MatrixNxN temp;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] = -elements[i];
        return temp;
    }

    // matrix-scalar addition
    template<typename scalarType> MatrixNxN operator+(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar addition is not defined for zero-dimensional matrices.");
        MatrixNxN temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] += scalar;
        return temp;
    }

    // matrix-scalar subtraction
    template<typename scalarType> MatrixNxN operator-(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar subtraction is not defined for zero-dimensional matrices.");
        MatrixNxN temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] -= scalar;
        return temp;
    }

    // matrix-scalar multiplication
    template<typename scalarType> MatrixNxN operator*(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar multiplication is not defined for zero-dimensional matrices.");
        MatrixNxN temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] *= scalar;
        return temp;
    }

    // matrix-scalar multiplication with assignment
    template<typename scalarType> void operator*=(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar multiplication is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] *= scalar;
    }

    // matrix-scalar division
    template<typename scalarType> MatrixNxN operator/(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar division is not defined for zero-dimensional matrices.");
        MatrixNxN temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] /= scalar;
        return temp;
    }

    // matrix-scalar division with assignment
    template<typename scalarType> void operator/=(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar division is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] /= scalar;
    }

    // matrix-matrix multiplication
    template<size_t OtherColumns>
    Matrix<ElementType, Rows, OtherColumns> operator*(const Matrix<ElementType, Columns, OtherColumns> &other) {
        static_assert(Rows > 0 && Columns > 0 && OtherColumns > 0, "Matrix-matrix multiplication is not defined for zero-dimensional matrices.");
        static_assert(OtherColumns == Columns, "Matrix-matrix multiplication is only defined for matrices with matching inner dimensions.");
        Matrix<ElementType, Rows, OtherColumns> temp;
        for(int i=0; i < Rows; i++) {
            for(int j=0; j < OtherColumns; j++) {
                temp.elements[i * OtherColumns + j] = 0;
                for(int k=0; k < Columns; k++) {
                    temp.elements[i * OtherColumns + j] += elements[i * Columns + k] * other.elements[k * OtherColumns + j];
                }
            }
        }
        return temp;
    }

    // default constructor
    Matrix() {
        static_assert(Rows > 0 && Columns > 0, "Zero matrix is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] = 0;
    }

    // constructor from 2D array
    Matrix(ElementType array[Rows][Columns]){
        static_assert(Rows > 0 && Columns > 0, "Nonzero matrix is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows; i++){
            for(int j=0; j < Columns; j++){
                elements[Rows * i + j] = array[i][j];
            }
        }
    }

    // constructor from 1D array
    Matrix(ElementType array[Rows * Columns]) {
        static_assert(Rows > 0 && Columns > 0, "Nonzero matrix is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] = array[i];
    }
};

#endif