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

using Triangle = Polygon3D<float, 3>;
using Vertex3F = Vertex3<float>;

int main(){

    Triangle cubeTriangles[12] = {
        // SOUTH
		Triangle(Vertex3F(0.0f, 0.0f, 0.0f),    Vertex3F(0.0f, 1.0f, 0.0f),    Vertex3F(1.0f, 1.0f, 0.0f)),
		Triangle(Vertex3F(0.0f, 0.0f, 0.0f),    Vertex3F(1.0f, 1.0f, 0.0f),    Vertex3F(1.0f, 0.0f, 0.0f)),

		// EAST                                                      
		Triangle(Vertex3F( 1.0f, 0.0f, 0.0f),   Vertex3F(1.0f, 1.0f, 0.0f),   Vertex3F(1.0f, 1.0f, 1.0f)),
		Triangle(Vertex3F(1.0f, 0.0f, 0.0f),    Vertex3F(1.0f, 1.0f, 1.0f),    Vertex3F(1.0f, 0.0f, 1.0f)),

		// NORTH                                                     
		Triangle(Vertex3F(1.0f, 0.0f, 1.0f),    Vertex3F(1.0f, 1.0f, 1.0f),    Vertex3F(0.0f, 1.0f, 1.0f)),
		Triangle(Vertex3F(1.0f, 0.0f, 1.0f),    Vertex3F(0.0f, 1.0f, 1.0f),    Vertex3F(0.0f, 0.0f, 1.0f)),

		// WEST                                                      
		Triangle(Vertex3F(0.0f, 0.0f, 1.0f),    Vertex3F(0.0f, 1.0f, 1.0f),    Vertex3F(0.0f, 1.0f, 0.0f)),
		Triangle(Vertex3F(0.0f, 0.0f, 1.0f),    Vertex3F(0.0f, 1.0f, 0.0f),    Vertex3F(0.0f, 0.0f, 0.0f)),

		// TOP                                                       
		Triangle(Vertex3F(0.0f, 1.0f, 0.0f),    Vertex3F(0.0f, 1.0f, 1.0f),    Vertex3F(1.0f, 1.0f, 1.0f)),
		Triangle(Vertex3F(0.0f, 1.0f, 0.0f),    Vertex3F(1.0f, 1.0f, 1.0f),    Vertex3F(1.0f, 1.0f, 0.0f)),

		// BOTTOM                                                    
		Triangle(Vertex3F(1.0f, 0.0f, 1.0f),    Vertex3F(0.0f, 0.0f, 1.0f),    Vertex3F(0.0f, 0.0f, 0.0f)),
		Triangle(Vertex3F(1.0f, 0.0f, 1.0f),    Vertex3F(0.0f, 0.0f, 0.0f),    Vertex3F(1.0f, 0.0f, 0.0f)),
    };
    
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

    std::vector<Window> windows(2, Window(width, height, "3d engine"));

    for(int i=0; i<windows.size(); i++){
        if (!windows[i].Init()) return -1;
    }

    
    float t = 0.0f;
    bool running = true;
    SDL_Event event;
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



        float translateX = 0.0f, translateY = 0.0f, translateZ = 1.0f;

        Matrix<float, 4, 4> translationMatrix = {
            1.0f, 0.0f, 0.0f, translateX,
            0.0f, 1.0f, 0.0f, translateY,
            0.0f, 0.0f, 1.0f, translateZ,
            0.0f, 0.0f, 0.0f, 1.0f,
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

        Matrix<float, 4, 4> worldMatrix = translationToWorldCenterInverse * rotationMatrix_Z * rotationMatrix_Y * rotationMatrix_X * translationToWorldCenter;
        
        Matrix<float, 4, 4> combinedTransformation = projectionMatrix * translationMatrix * worldMatrix;
        

        std::vector<Polygon2D<float, 3>> projectedTriangles;
        for(int i=0; i<12; i++){
            Triangle transformed = cubeTriangles[i].CopyTransformedByMatrix4x4(combinedTransformation);
            projectedTriangles.push_back(transformed.ToPolygon2D());
            projectedTriangles[i] *= 200.0f;
            projectedTriangles[i] += Vector<float, 2>(width/2, height/2);
            
        }






        for(int i=0; i<windows.size(); i++){
            windows[i].renderer2D->SetDrawColor(Colors::Black);
            windows[i].renderer2D->Clear();
            windows[i].renderer2D->SetDrawColor(Colors::Blue);
            
            for(int t=0; t<12; t++){
                windows[i].renderer2D->DrawTriangle(projectedTriangles[t].vertices);
            }

            windows[i].renderer2D->Present();
        }


        SDL_Delay(100);
        
    }
    

    return 0;
}