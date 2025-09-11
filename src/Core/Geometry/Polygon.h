#ifndef POLYGON3D_H
#define POLYGON3D_H


#include <iostream>
#include <array>
#include <vector>
#include <stdexcept>
#include "Vertex.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"


template<typename ComponentType, int SideCount>
struct Polygon2D {
    using Vector2 = Vector<ComponentType, 2>;
    
    std::array<Vector2, SideCount> vertices;


    void operator= (const Polygon2D &other){
        vertices = other.vertices;
    }
    void operator+= (const Vector2 &translation){
        for(Vector2& vertex : vertices){
            vertex += translation;;
        }
    }
    template<typename scalarType>
    void operator*= (const scalarType &scalar){
        for(Vector2& vertex : vertices){
            vertex *= scalar;
        }
    }
    
    template<typename scalarType>
    void scaleX(const scalarType &scalar){
        for(Vector2& vertex : vertices){
            vertex[0] *= scalar;
        }
    }

    template<typename scalarType>
    void scaleY(const scalarType &scalar){
        for(Vector2& vertex : vertices){
            vertex[1] *= scalar;
        }
    }

    Polygon2D(std::initializer_list<ComponentType> init) {
        if (init.size() != SideCount) {
            throw std::invalid_argument("Wrong number of arguments in initializer list");
        }
        std::copy(init.begin(), init.end(), vertices.begin());
    }

    template <typename... Args>
    Polygon2D(Args... args) : vertices{ static_cast<Vector2>(args)... } {
        static_assert(sizeof...(args) == SideCount, "Wrong number of arguments");
        vertices = {static_cast<Vector2>(args)...};
    }

    Polygon2D(){}
};


template<typename ComponentType, int SideCount>
struct Polygon3D {
    using Vertex = Vertex3<ComponentType>;
    using Vector2 = Vector<ComponentType, 2>;
    using Vector3 = Vector<ComponentType, 3>;
    using Vector4 = Vector<ComponentType, 4>;

    std::array<Vertex, SideCount> vertices;
    mutable Vector3 normal;

   
    Vector3 GetNormal() const {
        if (normal.SquaredComponentSum() < 1e-10f) {
            normal = FindSurfaceNormal();
        }
        return normal;
    }

    void TransformByMatrix4x4(const Matrix<ComponentType, 4, 4>& matrix) {
        for (Vertex vertex : vertices) {
            // Convert to homogeneous coordinates
            Vector4 homoPos(vertex.position[0], vertex.position[1], vertex.position[2], 1.0f);
            Vector4 transformed = homoPos * matrix;
            
            // Perspective divide
            if (transformed[3] != 0.0f) {
                vertex.position[0] = transformed[0] / transformed[3];
                vertex.position[1] = transformed[1] / transformed[3];
                vertex.position[2] = transformed[2] / transformed[3];
            }
        }
        normal = FindSurfaceNormal();
    }

    Polygon3D CopyTransformedByMatrix4x4(const Matrix<ComponentType, 4, 4> &matrix) const {
        Polygon3D copy(*this);
        for (Vertex& vertex : copy.vertices) {
            // Convert to homogeneous coordinates
            Vector4 homoPos(vertex.position[0], vertex.position[1], vertex.position[2], 1.0f);
            Vector4 transformed = homoPos * matrix;
            
            // Perspective divide
            if (transformed[3] != 0.0f) {
                vertex.position[0] = transformed[0] / transformed[3];
                vertex.position[1] = transformed[1] / transformed[3];
                vertex.position[2] = transformed[2] / transformed[3];
            }
        }
        copy.normal = copy.FindSurfaceNormal();
        return copy;
    }

    Polygon2D<ComponentType, SideCount> ToPolygon2D() const {
        Polygon2D<ComponentType, SideCount> output;
        for(int v=0; v<SideCount; v++){
            output.vertices[v] = Vector2(vertices[v].position[0], vertices[v].position[1]) ;
        }
        return output;
    }

    template<typename MatrixComponentType, int MatrixRows> // can only multiply by matrices with same width as dimensions of vectors
    Polygon3D operator*(const Matrix<MatrixComponentType, MatrixRows, 3> &matrix) const {
        Polygon3D copy = *this;
        for(Vertex vertex : copy.vertices){
            vertex *= matrix;
        }
        return copy;
    }

    template<typename MatrixComponentType, int MatrixRows> // can only multiply by matrices with same width as dimensions of vectors
    void operator*=(const Matrix<MatrixComponentType, MatrixRows, 3> &matrix){
        for(Vertex vertex : vertices){
            vertex *= matrix;
        }
    }

    Vector3 FindSurfaceNormal() const {
        Vector3 side1 = vertices[1].position - vertices[0].position;
        Vector3 side2 = vertices[2].position - vertices[0].position;
        return (side1 % side2).Unit();
    }

    Polygon3D(Vertex input[SideCount]){
        for(int i=0; i < SideCount; i++) vertices[i] = input[i];
        normal = FindSurfaceNormal();
    }

    Polygon3D(std::array<Vertex, SideCount> input){
        for(int i=0; i < SideCount; i++) vertices[i] = input[i];
        normal = FindSurfaceNormal();
    }

    Polygon3D(){
        for(int i=0; i < SideCount; i++) vertices[i] = Vertex();
        normal = FindSurfaceNormal();
    }

    template <typename... Args>
    Polygon3D(Args... args) : vertices{ static_cast<Vertex>(args)... } {
        static_assert(sizeof...(args) == SideCount, "Wrong number of arguments");
        vertices = {static_cast<Vertex>(args)...};
        normal = FindSurfaceNormal();
    }

    Polygon3D(std::initializer_list<ComponentType> init) {
        if (init.size() != SideCount) {
            throw std::invalid_argument("Wrong number of arguments in initializer list");
        }
        std::copy(init.begin(), init.end(), vertices.begin());
        normal = FindSurfaceNormal();
    }

};

template<typename ComponentType>
struct NGon3D {
    using Vertex = Vertex3<ComponentType>;
    using Vector3 = Vector<ComponentType, 3>;

    std::vector<Vertex> vertices;
    mutable Vector3 normal;

    Vector3 GetNormal() const {
        if (normal.SquaredComponentSum() < 1e-10f) {
            normal = FindSurfaceNormal();
        }
        return normal;
    }

    Vector3 FindSurfaceNormal() const {
        Vector3 side1 = vertices[1].position - vertices[0].position;
        Vector3 side2 = vertices[2].position - vertices[0].position;
        return (side1 % side2).Unit();
    }

    NGon3D(Vertex input[], size_t SideCount){
        for(int i=0; i < SideCount; i++) vertices.push_back(input[i]);
        normal = FindSurfaceNormal();
    }

    NGon3D(std::vector<Vertex> input){
        vertices = input;
        normal = FindSurfaceNormal();
    }

    NGon3D(){
        vertices = std::vector<Vertex>();
        normal = Vector3();
    }
};

#endif
