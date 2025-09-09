#include "Engine/Engine/Engine.h"

int main(){
    
    Engine engine;
    if(!engine.Initialize()){
        return -1;
    }

    engine.Run();
    return 0;
}