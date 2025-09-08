#include "Window.h"
#include <iostream>
#include <SDL2/SDL.h>


bool Window::Init() {
    if(window != nullptr){
        std::cerr<<"window already exists"<<std::endl;
        return false;
    }

    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	} 

    
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer2D = new Renderer2D(renderer);
    renderer3D = new Renderer3D(renderer2D, width, height);
    windowId = SDL_GetWindowID(window);

    return true;
}

void Window::HandleEvent(SDL_Event &event) {
    if(event.type != SDL_WINDOWEVENT || event.window.windowID != windowId) return;
    switch(event.type){
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(window);
            break;
        case SDL_WINDOWEVENT_DISPLAY_CHANGED:
            SDL_GetWindowSize(window, &width, &height);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            minimized = true;
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            minimized = false;
            break;
        case SDL_WINDOWEVENT_SHOWN:
            shown = true;
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            shown = false;
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(renderer);
            break;
        case SDL_WINDOWEVENT_ENTER:
            mouseFocus = true;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            mouseFocus = false;
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            keyboardFocus = true;
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            keyboardFocus = false;
            break;
        case SDL_WINDOWEVENT_RESTORED:
            minimized = false;
            break;
            
    }
}


void Window::Focus(){
    if(!shown) {
        SDL_ShowWindow(window);
    }
    SDL_RaiseWindow(window);
}

void Window::Resize(int newWidth, int newHeight){
    SDL_SetWindowSize(window, newWidth, newHeight);
    width = newWidth;
    height = newHeight;
}


bool Window::HasMouseFocus(){
    return mouseFocus;
}

bool Window::HasKeyboardFocus(){
    return keyboardFocus;
}

bool Window::IsShown(){
    return shown;
}

bool Window::IsMinimized(){
    return minimized;
}