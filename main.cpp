#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "Shared/Window/Window.h"

int main(){

    std::vector<Window> windows(10, Window(800, 600, "3d engine"));
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
            }

            SDL_Delay(100);
        }
    }
    

    return 0;
}