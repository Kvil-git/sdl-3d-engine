#ifndef COLORS_H
#define COLORS_H


#include <stdint.h>
#include "../../Core/Math/Vector.h"


namespace Colors{
    using Color3 = Vector<uint8_t, 3>;
    const Color3 Red(255, 0, 0);
    const Color3 Green(0, 255, 0);
    const Color3 Blue(0, 0, 255);
    const Color3 White(255, 255, 255);
    const Color3 Black(0, 0, 0);
};


#endif