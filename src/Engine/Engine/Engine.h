#ifndef ENGINE_H
#define ENGINE_H

#include "../Window/Window.h"
#include "../Scene/Scene.h"
#include "../Camera/Camera.h"
#include "../../Graphics/Renderer3D/Renderer3D.h"

class Engine {
    private:
        std::vector<Window> windows;
        Scene scene;
        //Camera camera; TODO
        bool running = false;

    public:
        bool Initialize();
        void Run();
        void ProcessInput(){};
        void Update();
        void Render(){};
        void Cleanup(){};

        Engine(){};
};

#endif