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
        auto triangles = MathFunctions::Polygons::Triangulate<float, std::array<Vertex3<float>, 4>>(
            quad.vertices, 
            quad.GetNormal()
        );
        for(auto& tri : triangles){
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

    
    rotationMatrix = MathFunctions::Matrices::CreateRotationMatrix(xAngle, yAngle, zAngle); 
    translationMatrix = MathFunctions::Matrices::CreateTranslationMatrix(0.0f, 0.0f, 0.0f);
    worldMatrix = Constants::Matrices::translationToWorldCenterInverse * rotationMatrix * Constants::Matrices::translationToWorldCenter;
}

Matrix<float, 4, 4> Scene::GetFinalTransformationMatrix(){
    return translationMatrix * worldMatrix;
}

Scene::Scene(){
    Update();

}