#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>

class InputHandler {
    private:
        bool keyStates[SDL_NUM_SCANCODES] = {0};
        bool lastFrameKeyStates[SDL_NUM_SCANCODES] = {0};
        std::vector<SDL_Event> eventsToProcess;
        
        int mouseX, mouseY;
        int mouseRelativeX, mouseRelativeY;
        bool mouseButtonStates[5];
        bool lastFrameMouseButtonStates[5];

        
        InputHandler();
        InputHandler(const InputHandler&) = delete;
        InputHandler& operator=(const InputHandler&) = delete;
        
    public:
        static InputHandler& GetInstance() {
            static InputHandler instance;
            return instance;
        }
        
        void AddEventToProcessingQueue(const SDL_Event& event);
        void Update();
        
        bool GetKey(SDL_Keycode key) const;
        bool GetKeyDown(SDL_Keycode key) const;
        bool GetKeyUp(SDL_Keycode key) const;
        
        int GetMouseX() const { return mouseX; }
        int GetMouseY() const { return mouseY; }
        int GetMouseRelX() const { return mouseRelativeX; }
        int GetMouseRelY() const { return mouseRelativeY; }
        bool GetMouseButton(int button) const;
        bool GetMouseButtonDown(int button) const;
        bool GetMouseButtonUp(int button) const;
};



#endif 