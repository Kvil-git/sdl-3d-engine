#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>

#include "../../Core/Math/Vector.h"
#include "../../Core/Math/Matrix.h"
#include "../Clock/Clock.h"
#include "Engine.h"


bool Engine::Initialize(){
    
    windows = std::vector<Window>(2, Window(800, 600, "3d engine"));

    for(int i=0; i<windows.size(); i++){
        if (!windows[i].Init()) return false;
    }

    running = true;

    return true;
}


void Engine::Update(){
    Clock &clock = Clock::GetInstance();
    clock.Update();
    scene.Update();
}


void Engine::Run(){
    using Triangle = Polygon3D<float, 3>;
    using Vertex3F = Vertex3<float>;
    
    float t = 0.0f;
    running = true;
    SDL_Event event;

    
    scene.LoadModel("../assets/models/rizzard.obj");
    scene.Update();


    while(running){
         while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                running = false;
            }
            
            for(int i=0; i<windows.size(); i++){
                windows[i].HandleEvent(event);
            }
        }

        Update();

        auto triangles = scene.GetTriangles();

        for(int i=0; i<2; i++) {
            windows[i].renderer3D->Clear();
            windows[i].renderer3D->Render(triangles, scene.GetFinalTransformationMatrix(), Vector<float, 3>(0,0,0));
            windows[i].renderer3D->Present();
        }

        SDL_Delay(100);
        
    }
}