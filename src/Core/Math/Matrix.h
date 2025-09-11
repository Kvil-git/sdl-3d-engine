#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <array>
#include <initializer_list>

template <typename ElementType, size_t Rows, size_t Columns>
struct Matrix {
    using MatrixNxM = Matrix<ElementType, Rows, Columns>;
    std::array<ElementType, Rows*Columns> elements;

    // Matrix addition
    MatrixNxM operator+(const MatrixNxM &other) const {
        static_assert(Rows > 0 && Columns > 0, "Matrix addition is not defined for zero-dimensional matrices.");
        MatrixNxM temp;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] = elements[i] + other.elements[i];
        return temp;
    }

    // Addition with assignment
    void operator+=(const MatrixNxM &other) {
        static_assert(Rows > 0 && Columns > 0, "Matrix addition is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] += other.elements[i];
    }

    // Matrix subtraction
    MatrixNxM operator-(const MatrixNxM &other) const {
        static_assert(Rows > 0 && Columns > 0, "Matrix subtraction is not defined for zero-dimensional matrices.");
        MatrixNxM temp;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] = elements[i] - other.elements[i];
        return temp;
    }

    // Subtraction with assignment
    void operator-=(const MatrixNxM &other) {
        static_assert(Rows > 0 && Columns > 0, "Matrix subtraction is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] -= other.elements[i];
    }

    // Matrix negation
    void operator-() {
        static_assert(Rows > 0 && Columns > 0, "Matrix negation is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] = -elements[i];
    }

    // Matrix-scalar addition
    template<typename scalarType> MatrixNxM operator+(const scalarType &scalar) const {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar addition is not defined for zero-dimensional matrices.");
        MatrixNxM temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] += scalar;
        return temp;
    }

    // Matrix-scalar subtraction
    template<typename scalarType> MatrixNxM operator-(const scalarType &scalar) const {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar subtraction is not defined for zero-dimensional matrices.");
        MatrixNxM temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] -= scalar;
        return temp;
    }

    // Matrix-scalar multiplication
    template<typename scalarType> MatrixNxM operator*(const scalarType &scalar) const {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar multiplication is not defined for zero-dimensional matrices.");
        MatrixNxM temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] *= scalar;
        return temp;
    }

    // Matrix-scalar multiplication with assignment
    template<typename scalarType> void operator*=(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar multiplication is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] *= scalar;
    }

    // Matrix-scalar division
    template<typename scalarType> MatrixNxM operator/(const scalarType &scalar) const {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar division is not defined for zero-dimensional matrices.");
        MatrixNxM temp = *this;
        for(int i=0; i < Rows * Columns; i++) temp.elements[i] /= scalar;
        return temp;
    }

    // Matrix-scalar division with assignment
    template<typename scalarType> void operator/=(const scalarType &scalar) {
        static_assert(Rows > 0 && Columns > 0, "Matrix-scalar division is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] /= scalar;
    }

    inline ElementType operator()(int i, int j) const {
        if (i >= Rows)
            throw std::out_of_range("Row index out of range");
        if (j >= Columns)
            throw std::out_of_range("Column index out of range");
        return elements[i * Columns + j];
    }

    // Non-const version for write access
    inline ElementType& operator()(int i, int j) {
        if (i >= Rows)
            throw std::out_of_range("Row index out of range");
        if (j >= Columns)
            throw std::out_of_range("Column index out of range");
        return elements[i * Columns + j];
    }

    // Matrix-matrix multiplication
    template<size_t OtherColumns>
    Matrix<ElementType, Rows, OtherColumns> operator*(const Matrix<ElementType, Columns, OtherColumns> &other) const {
        static_assert(Rows > 0 && Columns > 0 && OtherColumns > 0, "Matrix-matrix multiplication is not defined for zero-dimensional matrices.");
        Matrix<ElementType, Rows, OtherColumns> temp;
        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < OtherColumns; j++) {
                ElementType sum = 0;
                for (size_t k = 0; k < Columns; k++) {
                    sum += (*this)(i, k) * other(k, j);
                }
                temp(i, j) = sum;
            }
        }
        return temp;
    }

    // Default constructor
    Matrix() {
        static_assert(Rows > 0 && Columns > 0, "Zero matrix is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] = 0;
    }

    // Constructor from 2D array
    Matrix(ElementType array[Rows][Columns]){
        static_assert(Rows > 0 && Columns > 0, "Nonzero matrix is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows; i++){
            for(int j=0; j < Columns; j++){
                (*this)(i, j) = array[i][j];
            }
        }
    }

    // Constructor from 1D array
    Matrix(ElementType array[Rows * Columns]) {
        static_assert(Rows > 0 && Columns > 0, "Nonzero matrix is not defined for zero-dimensional matrices.");
        for(int i=0; i < Rows * Columns; i++) elements[i] = array[i];
    }

    Matrix(std::initializer_list<ElementType> init){
        if(init.size() != Rows * Columns){
            throw std::runtime_error("invalid number of elements in initializer list");
        }
        std::copy(init.begin(), init.end(), elements.begin());
    }
};

#endif