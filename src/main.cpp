#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>

#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"
#include "Core/Geometry/Polygon.h"
#include "Core/Geometry/Vertex.h"
#include "Core/Utilities/OutputFunctions.h"
#include "Resources/ModelLoader/ModelLoader.h"

#include "Engine/Window/Window.h"
#include "Utilities/Enums/Colors.h"

#include "Utilities/Enums/Constants.h"

#include "Utilities/MathFunctions/MathFunctions.h"


using Triangle = Polygon3D<float, 3>;
using Vertex3F = Vertex3<float>;

int main(){
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


    ModelLoader<float> loader;
    if(!loader.LoadFromObj("../assets/models/cube.obj")){
        std::cerr<<"error loading model from obj file"<<std::endl;
    }


    std::vector<Window> windows(2, Window(width, height, "3d engine"));

    for(int i=0; i<windows.size(); i++){
        if (!windows[i].Init()) return -1;
    }


    
    float t = 0.0f;
    bool running = true;
    SDL_Event event;

    

    std::cout<<loader.quadrilaterals<<std::endl;
    std::cout<<loader.triangles<<std::endl;

    std::vector<Triangle> triangulatedTris;
    triangulatedTris.reserve(loader.quadrilaterals.size() * 2); 

    for(auto quad : loader.quadrilaterals){
        std::vector<Triangle> triangulated = quad.Triangulate();
        for(auto tri : triangulated){
            triangulatedTris.push_back(tri);
        }
    }



    Vector<float, 3> cameraCoordinates(0,0,0);



    while(running){
         while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                running = false;
            }
            
            for(int i=0; i<windows.size(); i++){
                windows[i].HandleEvent(event);
            }
        }


        t+=0.1f;

        float xAngle = 1.0f + t;
        float yAngle = 2.0f + t * 0.2;
        float zAngle = 1.5f + t * 0.2;
        
        
        Matrix<float, 4, 4> rotationMatrix = MathFunctions::Matrices::CreateRotationMatrix(xAngle, yAngle, zAngle); 

        float translateX = 0.0f, translateY = 0.0f, translateZ = 15.0f;

        Matrix<float, 4, 4> translationMatrix = MathFunctions::Matrices::CreateTranslationMatrix(translateX, translateY, translateZ);
        Matrix<float, 4, 4> worldMatrix = Constants::Matrices::translationToWorldCenterInverse * rotationMatrix * Constants::Matrices::translationToWorldCenter;
        Matrix<float, 4, 4> combinedTransformation = projectionMatrix * translationMatrix * worldMatrix;
        
        for(int i=0; i<2; i++){
            windows[i].renderer3D->Clear();
            windows[i].renderer3D->Render(loader.triangles, combinedTransformation, cameraCoordinates);
            windows[i].renderer3D->Render(triangulatedTris, combinedTransformation, cameraCoordinates);
            windows[i].renderer3D->Present();
        }

        SDL_Delay(100);
        
    }
    

    return 0;
}