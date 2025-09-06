#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "Shared/Window/Window.h"
#include "EngineMath/Vector.h"
#include "Shared/Enums/Colors.h"
#include "EngineMath/Matrix.h"
#include "EngineMath/Polygon3D.h"
#include "EngineMath/Vertex.h"
#include <math.h>
#include "EngineMath/ModelLoader.h"
#include <string>
#include "EngineMath/OutputFunctions.h"
#include <algorithm>

using Triangle = Polygon3D<float, 3>;
using Vertex3F = Vertex3<float>;

int main(){
    float width = 800.0f, height = 600.0f;

    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float fov = 90.0f;
    float fovInRadians = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    float aspectRatio = width / height;

    float q = 1 / (farPlane - nearPlane);

    Matrix<float, 4, 4> projectionMatrix = {
        aspectRatio * fovInRadians,    0.0f,               0.0f,                         0.0f,
        0.0f,                          fovInRadians,       0.0f,                         0.0f, 
        0.0f,                          0.0f,               (farPlane + nearPlane) * q,   1.0f, 
        0.0f,                          0.0f,               -2 * farPlane * q,            0.0f
    };

    Matrix<float,4,4> translationToWorldCenter = {
        1, 0, 0, -0.5,
        0, 1, 0, -0.5,
        0, 0, 1, -0.5,
        0, 0, 0, 1
    };

    Matrix<float,4,4> translationToWorldCenterInverse = {
        1, 0, 0, 0.5,
        0, 1, 0, 0.5,
        0, 0, 1, 0.5,
        0, 0, 0, 1
    };


    ModelLoader<float> loader;
    if(!loader.LoadFromObj("assets/models/cube.obj")){
        std::cerr<<"oshibka stop 000000000"<<std::endl;
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

        Matrix<float, 4, 4> rotationMatrix_X = {
            1.0f,    0.0f,           0.0f,       0.0f,
            0.0f,    cos(xAngle),   -sin(xAngle), 0.0f,
            0.0f,    sin(xAngle),    cos(xAngle), 0.0f,
            0.0f,    0.0f,           0.0f,       1.0f
        };

        Matrix<float, 4, 4> rotationMatrix_Y = {
            cos(yAngle),     0.0f,           sin(yAngle),        0.0f,
            0.0f,            1.0f,           0.0f,               0.0f,
            -sin(yAngle),    0.0f,           cos(yAngle),        0.0f,
            0.0f,            0.0f,           0.0f,               1.0f
        };
        
        Matrix<float, 4, 4> rotationMatrix_Z = {
            cos(zAngle),  -sin(zAngle),   0.0f,      0.0f,
            sin(zAngle),   cos(zAngle),   0.0f,      0.0f,
            0.0f,          0.0f,          1.0f,      0.0f,
            0.0f,          0.0f,          0.0f,      1.0f
        };



        float translateX = 0.0f, translateY = 0.0f, translateZ = 15.0f;

        Matrix<float, 4, 4> translationMatrix = {
            1.0f, 0.0f, 0.0f, translateX,
            0.0f, 1.0f, 0.0f, translateY,
            0.0f, 0.0f, 1.0f, translateZ,
            0.0f, 0.0f, 0.0f, 1.0f,
        };

        Matrix<float, 4, 4> worldMatrix = translationToWorldCenterInverse * rotationMatrix_Z * rotationMatrix_Y * rotationMatrix_X * translationToWorldCenter;
        
        Matrix<float, 4, 4> combinedTransformation = projectionMatrix * translationMatrix * worldMatrix;
        

        std::vector<Polygon2D<float, 3>> projectedTriangles;

        auto sortingCriteria = [](Triangle& one, Triangle& two){
            float z1 = (one.vertices[0].position[2] + one.vertices[1].position[2] + one.vertices[2].position[2]) / 3.0f;
            float z2 = (two.vertices[0].position[2] + two.vertices[1].position[2] + two.vertices[2].position[2]) / 3.0f;
            return z1 < z2;
        };

        std::sort(loader.triangles.begin(), loader.triangles.end(), sortingCriteria);
        std::sort(triangulatedTris.begin(), triangulatedTris.end(), sortingCriteria);

        for(auto triangle : loader.triangles){
            Triangle transformed = triangle.CopyTransformedByMatrix4x4(combinedTransformation);
            Vector<float,3> normal = transformed.GetNormal();
            if (normal.SquaredComponentSum() < 1e-10f) {
                continue; // skip degenerate triangle
            }
            if ((normal * (transformed.vertices[0].position - cameraCoordinates)) < -0.01f) continue;
            
            auto projected = transformed.ToPolygon2D();
            projected *= 700.0f;
            projected += Vector<float, 2>(width/2, height/2);

            projectedTriangles.push_back(projected);
        }
        for(auto triangle : triangulatedTris){
            Triangle transformed = triangle.CopyTransformedByMatrix4x4(combinedTransformation);

            Vector<float,3> normal = transformed.GetNormal();
            if (normal.SquaredComponentSum() < 1e-10f) {
                continue; // skip degenerate triangle
            }
            if ((normal * (transformed.vertices[0].position - cameraCoordinates)) < -0.01f) continue;
            
            auto projected = transformed.ToPolygon2D();
            projected *= 700.0f;
            projected += Vector<float, 2>(width/2, height/2);

            projectedTriangles.push_back(projected);
        }


        for(int i=0; i<windows.size(); i++){
            windows[i].renderer2D->SetDrawColor(Colors::Black);
            windows[i].renderer2D->Clear();
            windows[i].renderer2D->SetDrawColor(Colors::Blue);

            for(int t=0; t<projectedTriangles.size(); t++){
                windows[i].renderer2D->DrawTriangle(projectedTriangles[t].vertices);
            }

            windows[i].renderer2D->Present();
        }


        SDL_Delay(100);
        
    }
    

    return 0;
}