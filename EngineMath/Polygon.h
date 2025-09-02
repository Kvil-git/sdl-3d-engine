#include <iostream>
#include <array>
#include "Vertex.h"
#include "Vector.h"


#ifndef POLYGON_H
#define POLYGON_H


template<typename ComponentType, int SideCount>
struct Polygon {
    using Vertex3 = Vertex3<ComponentType>;
    using Vector3 = Vector<ComponentType, 3>;

    Vertex3 vertices[SideCount];
    Vector3 normal;

    Vector3 GetNormal(){
        if (normal.ComponentSum() == 0) normal = FindSurfaceNormal();
        return normal;
    }

    Vector3 FindSurfaceNormal() {
        Vector3 side1 = vertices[1].position - vertices[0].position;
        Vector3 side2 = vertices[2].position - vertices[0].position;
        return (side1 % side2).Unit();
    }

    Polygon(Vertex3 input[SideCount]){
        for(int i=0; i < SideCount; i++) vertices[i] = input[i];
        normal = FindSurfaceNormal();
    }

    Polygon(std::array<Vertex3, SideCount> input){
        for(int i=0; i < SideCount; i++) vertices[i] = input[i];
        normal = FindSurfaceNormal();
    }

    Polygon(){
        for(int i=0; i < SideCount; i++) vertices[i] = Vertex3();
        normal = Vector3();
    }

};

template<typename ComponentType>
struct NPolygon {
    using Vertex3 = Vertex3<ComponentType>;
    using Vector3 = Vector<ComponentType, 3>;

    std::vector<Vertex3> vertices;
    Vector3 normal;

    Vector3 GetNormal(){
        if (normal.ComponentSum() == 0) normal = FindSurfaceNormal();
        return normal;
    }

    Vector3 FindSurfaceNormal() {
        Vector3 side1 = vertices[1].position - vertices[0].position;
        Vector3 side2 = vertices[2].position - vertices[0].position;
        return (side1 % side2).Unit();
    }

    NPolygon(Vertex3 input[], size_t SideCount){
        for(int i=0; i < SideCount; i++) vertices.push_back(input[i]);
        normal = FindSurfaceNormal();
    }

    NPolygon(std::vector<Vertex3> input){
        vertices = input;
        normal = FindSurfaceNormal();
    }

    NPolygon(){
        vertices = std::vector<Vertex3>();
        normal = Vector3();
    }
};

#endif
