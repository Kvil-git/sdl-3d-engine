#include <string>
#include <iostream>
#include "Vector.h"

#ifndef MATERIAL_H
#define MATERIAL_H

template <typename ComponentType>
struct Material {
    using Vector3 = Vector<ComponentType, 3>;

    Material() {
        name;
        specularExponent = 0.0f;
        dissolve = 0.0f;
        opticalDensity = 0.0f;
        illumination = 0;
    }

    // Material Name
    std::string name;
    std::string ambientColorMap;       // Ambient Texture Map
    std::string diffuseColorMap;       // Diffuse Texture Map
    std::string specularColorMap;      // Specular Texture Map
    std::string specularExponentMap;   // Specular Hightlight Map
    std::string dissolveMap;           // Alpha Texture Map
    std::string bumpMap;               // Bump Map
    
    Vector3 ambientColor;              // Ambient Color
    Vector3 diffuseColor;              // Diffuse Color
    Vector3 specularColor;             // Specular Color
    
    
    ComponentType specularExponent;    // Specular Exponent
    ComponentType dissolve;            // Dissolve
    ComponentType opticalDensity;      // Optical Density
    
    int illumination;                  // Illumination

};

#endif