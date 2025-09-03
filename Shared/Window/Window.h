#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

struct Window {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        int windowId;

        int width, height;
        bool shown = true, mouseFocus = true, keyboardFocus = true, minimized = false;
        std::string title;

    public:
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
                                                           title(title) {};
        ~Window(){ 
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        };
    
};




#endif