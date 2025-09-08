#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include "../../Graphics/Renderer2D/Renderer2D.h"
#include "../../Graphics/Renderer3D/Renderer3D.h"

struct Window {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        int windowId;

        int width, height;
        bool shown = true, mouseFocus = true, keyboardFocus = true, minimized = false;
        std::string title;

    public:
        Renderer2D* renderer2D = nullptr;
        Renderer3D* renderer3D = nullptr;

        bool Init();
        void HandleEvent(SDL_Event& event);

        void Focus();
        
        bool HasMouseFocus();
        bool HasKeyboardFocus();
        bool IsMinimized();
        bool IsShown();

        void Resize(int newWidth, int newHeight);
        void ChangeTitle(std::string newTitle);

        Window(int width, int height, std::string title) : window(nullptr),
                                                           renderer(nullptr),
                                                           width(width),
                                                           height(height),
                                                           title(title),
                                                           renderer2D(nullptr) {};
        ~Window(){ 
            delete renderer2D;
            delete renderer3D;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        };
    
};




#endif