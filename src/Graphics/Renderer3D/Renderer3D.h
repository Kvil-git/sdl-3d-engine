#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <vector>
#include "../Renderer2D/Renderer2D.h"
#include "../../Core/Geometry/Polygon.h"
#include "../../Core/Math/Matrix.h"


class Renderer3D {
    using Triangle2D = Polygon2D<float, 3>;
    using Triangle3D = Polygon3D<float, 3>;

    public:

        Renderer3D(Renderer2D* renderer2D, float windowWidth, float windowHeight) : renderer2D(renderer2D),
                                                                                    windowWidth(windowWidth),
                                                                                    windowHeight(windowHeight){};

        void Render(const std::vector<Triangle3D> &triangles, const Matrix<float, 4, 4> &viewProjectionMatrix, const Vector<float, 3>& cameraPosition);

        void SetWindowDimensions(float width, float height);

    private:
        Renderer2D* renderer2D;
        float windowWidth;
        float windowHeight;
};



#endif