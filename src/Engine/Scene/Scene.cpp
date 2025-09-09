#include <algorithm>
#include <string>
#include <exception>
#include <iostream>
#include "Scene.h"


#include "../../Core/Math/Vector.h"
#include "../../Core/Math/Matrix.h"
#include "../../Core/Utilities/MathFunctions.h"

#include "../../Enums/Colors.h"
#include "../../Enums/Constants.h"

#include "../../Engine/Window/Window.h"
#include "../../Engine/Clock/Clock.h"



bool Scene::LoadModel(const std::string &path){

    if(!modelLoader.LoadFromObj(path)){
        return false;
    }

    std::vector<Polygon3D<float, 3>> triangulatedTriangles;
    triangulatedTriangles.reserve(modelLoader.quadrilaterals.size() * 2);

    for(auto& quad : modelLoader.quadrilaterals){
        for(auto& tri : quad.Triangulate()){
            triangulatedTriangles.push_back(tri);
        }
    }

    triangles.reserve(modelLoader.triangles.size() + modelLoader.quadrilaterals.size() * 2);

    for(auto& tri : modelLoader.triangles){
        triangles.push_back(tri);
    }
    for(auto& tri : triangulatedTriangles){
        triangles.push_back(tri);
    }

    return true;
}


void Scene::Update(){
    
    Clock &clock = Clock::GetInstance();

    float t = clock.GetCurrentTime() / 1000.0f;

    std::cout<<"t = "<<t<<std::endl;

    float xAngle = 1.0f + t;
    float yAngle = 2.0f + t * 0.2;
    float zAngle = 1.5f + t * 0.2;
    
    float translateX = 0.0f, translateY = 0.0f, translateZ = 15.0f;

    
    rotationMatrix = MathFunctions::Matrices::CreateRotationMatrix(xAngle, yAngle, zAngle); 
    translationMatrix = MathFunctions::Matrices::CreateTranslationMatrix(translateX, translateY, translateZ);
    worldMatrix = Constants::Matrices::translationToWorldCenterInverse * rotationMatrix * Constants::Matrices::translationToWorldCenter;
}

Matrix<float, 4, 4> Scene::GetFinalTransformationMatrix(){
    return translationMatrix * worldMatrix;
}

Scene::Scene(){
    float width = 800.0f, height = 600.0f;
    float fov = 90.0f;
    float fovInRadians = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    float aspectRatio = width / height;


    Matrix<float, 4, 4> projectionMatrix = {
        aspectRatio * fovInRadians,    0.0f,               0.0f,                           0.0f,
        0.0f,                          fovInRadians,       0.0f,                           0.0f, 
        0.0f,                          0.0f,               Constants::Projection::r,       1.0f, 
        0.0f,                          0.0f,               Constants::Projection::k,       0.0f
    };


    Update();

}