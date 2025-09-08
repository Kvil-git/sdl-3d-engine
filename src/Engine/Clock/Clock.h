#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL.h>
#include <stdint.h>

class Clock {
    public:
        static Clock& GetInstance() {
            static Clock instance;
            return instance;
        }

        void Update();
        float GetDeltaTime() const;
        uint64_t GetCurrentTime() const;

    private:
        Clock();
        uint64_t lastUpdateTime;
        float deltaTime;
};

#endif