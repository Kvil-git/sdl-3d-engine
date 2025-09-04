#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "Shared/Window/Window.h"
#include "EngineMath/Vector.h"
#include "Shared/Enums/Colors.h"
int main(){

    std::vector<Window> windows(3, Window(800, 600, "3d engine"));
    for(int i=0; i<windows.size(); i++){
        if (!windows[i].Init()) return -1;
    }

    bool running = true;
    SDL_Event event;
    while(running){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                running = false;
            }
            
            for(int i=0; i<windows.size(); i++){
                windows[i].HandleEvent(event);
                windows[i].renderer2D->setDrawColor(Colors::White);
                Vector<int, 2> center({150, 150});
                windows[i].renderer2D->drawCircle(center, 25, 32);
                windows[i].renderer2D->present();
            }


            SDL_Delay(100);
        }
    }
    

    return 0;
}