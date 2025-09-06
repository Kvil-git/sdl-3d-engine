#ifndef POLYGON3D_H
#define POLYGON3D_H


#include <iostream>
#include <array>
#include <vector>
#include <stdexcept>
#include "Vertex.h"
#include "Vector.h"
#include "Matrix.h"


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

    Polygon2D(){}
};


template<typename ComponentType, int SideCount>
struct Polygon3D {
    using Vertex = Vertex3<ComponentType>;
    using Vector2 = Vector<ComponentType, 2>;
    using Vector3 = Vector<ComponentType, 3>;
    using Vector4 = Vector<ComponentType, 4>;

    std::array<Vertex, SideCount> vertices;
    Vector3 normal;

   
    Vector3 GetNormal(){
        if (normal.ComponentSum() == 0) normal = FindSurfaceNormal();
        return normal;
    }
    
    bool IsPointInTriangle(Vector<ComponentType, 2>& p, Vector<ComponentType, 2>& a, Vector<ComponentType, 2>& b, Vector<ComponentType, 2>& c) {
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
        
        // Check if point is in triangle (with tolerance for floating point errors)
        return (u >= -epsilon) && (v >= -epsilon) && (u + v <= 1 + epsilon);
    }

    std::vector<Polygon3D<ComponentType, 3>> Triangulate() {
        std::vector<Polygon3D<ComponentType, 3>> triangles;
        if (SideCount < 3) return triangles;

        // For triangles, just return ourselves
        if (SideCount == 3) {
            triangles.push_back(Polygon3D<ComponentType, 3>(vertices[0], vertices[1], vertices[2]));
            return triangles;
        }

        // Project 3D vertices to 2D using the normal's dominant axis
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

        // Create 2D points and indices
        std::vector<Vector<ComponentType, 2>> points2D;
        std::vector<int> indices;
        for (int i = 0; i < SideCount; ++i) {
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

                // Check if the angle is convex (using cross product)
                Vector<ComponentType, 2> ab = b - a;
                Vector<ComponentType, 2> bc = c - b;
                ComponentType cross = ab[0] * bc[1] - ab[1] * bc[0];
                
                // For counter-clockwise winding, we want positive cross product
                if (cross <= 0) continue; // Skip concave or colinear points

                // Check if any other point is inside the triangle
                bool isEar = true;
                for (size_t j = 0; j < indices.size(); ++j) {
                    if (j == prev || j == i || j == next) continue;
                    
                    if (IsPointInTriangle(points2D[indices[j]], a, b, c)) {
                        isEar = false;
                        break;
                    }
                }

                if (isEar) {
                    // Create triangle from vertices
                    Polygon3D<ComponentType, 3> triangle(
                        vertices[indices[prev]],
                        vertices[indices[i]],
                        vertices[indices[next]]
                    );
                    triangles.push_back(triangle);
                    
                    // Remove the ear point
                    indices.erase(indices.begin() + i);
                    earFound = true;
                    break;
                }
            }

            if (!earFound) {
                // Fallback: use fan triangulation if ear clipping fails
                for (int i = 1; i < indices.size() - 1; ++i) {
                    Polygon3D<ComponentType, 3> triangle(
                        vertices[indices[0]],
                        vertices[indices[i]],
                        vertices[indices[i + 1]]
                    );
                    triangles.push_back(triangle);
                }
                break;
            }
        }

        // Add final triangle
        if (indices.size() == 3) {
            Polygon3D<ComponentType, 3> triangle(
                vertices[indices[0]],
                vertices[indices[1]],
                vertices[indices[2]]
            );
            triangles.push_back(triangle);
        }

        return triangles;
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

    Polygon3D CopyTransformedByMatrix4x4(const Matrix<ComponentType, 4, 4> &matrix){
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

    Polygon2D<ComponentType, SideCount> ToPolygon2D(){
        Polygon2D<ComponentType, SideCount> output;
        for(int v=0; v<SideCount; v++){
            output.vertices[v] = Vector2(vertices[v].position[0], vertices[v].position[1]) ;
        }
        return output;
    }

    template<typename MatrixComponentType, int MatrixRows> // can only multiply by matrices with same width as dimensions of vectors
    Polygon3D operator*(const Matrix<MatrixComponentType, MatrixRows, 3> &matrix){
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

    Vector3 FindSurfaceNormal() {
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
    Vector3 normal;

    Vector3 GetNormal(){
        if (normal.SquaredComponentSum() < 1e-10f) {
            normal = FindSurfaceNormal();
        }
        return normal;
    }

    Vector3 FindSurfaceNormal() {
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
