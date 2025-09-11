#ifndef ENGINE_H
#define ENGINE_H

#include "../Window/Window.h"
#include "../Scene/Scene.h"
#include "../Camera/Camera.h"
#include "../../Graphics/Renderer3D/Renderer3D.h"

#include "../../Events/InputEvents.h"
#include "../InputHandler/InputHandler.h"

class Engine {
    private:
        std::vector<Window> windows;
        Scene scene;
        bool running = false;

        EventController eventController;
        InputHandler inputHandler;
        Camera camera;
        
    public:
        bool Initialize();
        void Run();
        void ProcessInput(){};
        void Update();
        void Render(){};
        void Cleanup(){};
        
        Engine() : inputHandler(eventController), 
                camera(90.0f, 800.0f/600.0f, 0.1f, 1000.0f) {}
};

#endif