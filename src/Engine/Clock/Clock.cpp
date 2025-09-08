#include <SDL2/SDL.h>
#include <stdint.h>
#include "Clock.h"

Clock::Clock() : lastUpdateTime(SDL_GetTicks64()), deltaTime(0.0f) {}

void Clock::Update() {
    uint64_t currentTime = SDL_GetTicks64();
    deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;
}

float Clock::GetDeltaTime() const {
    return deltaTime;
}

uint64_t Clock::GetCurrentTime() const {
    return lastUpdateTime;
}