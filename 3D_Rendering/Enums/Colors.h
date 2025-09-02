#ifndef COLORS_H
#define COLORS_H
#include "../../EngineMath/Vector.h"
#include "stdint.h"


namespace Colors{
    using Color3 = Vector<uint8_t, 3>;
    
    inline constexpr Color3 Red({255, 0, 0});
    inline constexpr Color3 Green({0, 255, 0});
    inline constexpr Color3 Blue({0, 0, 255});
    inline constexpr Color3 White({255, 255, 255});
    inline constexpr Color3 Black({0, 0, 0});
};


#endif