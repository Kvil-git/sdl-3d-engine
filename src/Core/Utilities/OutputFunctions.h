#ifndef OUTPUT_FUNCTIONS
#define OUTPUT_FUNCTIONS

#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>

#include "../Geometry/Polygon.h"
#include "../Geometry/Model.h"


template<typename Type>
std::ostream& operator<<(std::ostream &os, const std::vector<Type> &vector){
    os << "||std::vector <"<< typeid(Type).name() <<">|| [";
    for(int i=0; i < vector.size(); i++) os << vector[i] << (i == vector.size() - 1 ? "" : ", ");
    os << "]";
    return os;
}


template <typename ComponentType, size_t Dimensions>
std::ostream& operator<<(std::ostream &os, const Vector<ComponentType, Dimensions> &vector) {
    os << "||Vector"<<Dimensions<<"|| [";
    for(int i=0; i < Dimensions; i++) os << vector.components[i] << (i == Dimensions - 1 ? "" : ", ");
    os << "]";
    return os;
}


template <typename ComponentType>
std::ostream& operator<<(std::ostream &os, const Vertex3<ComponentType> &vertex) {
    os << "||Vertex3D||\n";
    os << "Position: " << vertex.position << std::endl;
    os << "Normal: " << vertex.normal << std::endl;
    os << "Texture Coordinates: " << vertex.textureCoordinates << std::endl;
    return os;
}

template <typename ComponentType>
std::ostream& operator<<(std::ostream &os, const NGon3D<ComponentType> &ngon) {
    os << "||NGon3D "<< ngon.vertices.size() << "||\n";
    for(int i=0; i < ngon.vertices.size(); i++) os << ngon.vertices[i] << std::endl;
    return os;
}


template <typename ComponentType, int VertexCount>
std::ostream& operator<<(std::ostream &os, const Polygon3D<ComponentType, VertexCount> &polygon) {
    os << "||Polygon3D" << VertexCount << "||\n";
    for(int i=0; i < VertexCount; i++) os << polygon.vertices[i] << std::endl;
    return os;
}

template <typename ComponentType, int VertexCount>
std::ostream& operator<<(std::ostream &os, const Polygon2D<ComponentType, VertexCount> &polygon) {
    os << "||Polygon2D" << VertexCount << "||\n";
    for(int i=0; i < VertexCount; i++) os << polygon.vertices[i] << std::endl;
    return os;
}


template <typename ComponentType>
std::ostream& operator<<(std::ostream &os, const Material<ComponentType> &material) {
    os << "||Material|| " << material.name << std::endl;
    os << "Ambient Color: " << material.ambientColor << std::endl;
    os << "Diffuse Color: " << material.diffuseColor << std::endl;
    os << "Specular Color: " << material.specularColor << std::endl;
    os << "Specular Exponent: " << material.specularExponent << std::endl;
    os << "Dissolve: " << material.dissolve << std::endl;
    os << "Optical Density: " << material.opticalDensity << std::endl;
    os << "Illumination: " << material.illumination << std::endl;
    os << "Ambient Color Map: " << material.ambientColorMap << std::endl;
    os << "Diffuse Color Map: " << material.diffuseColorMap << std::endl;
    os << "Specular Color Map: " << material.specularColorMap << std::endl;
    os << "Specular Exponent Map: " << material.specularExponentMap << std::endl;
    os << "Dissolve Map: " << material.dissolveMap << std::endl;
    os << "Bump Map: " << material.bumpMap << std::endl;
    return os;
}
#endif