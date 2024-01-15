#include "Engine.h"
#include "core/Log.h"

int main(int, char**)
{
    Log::Init();
    Engine engine;
    if (engine.init()) engine.run();
    return 0;
}