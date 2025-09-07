#ifndef MODEL_H
#define MODEL_H

#include "Polygon.h"
#include "Vertex.h"
#include "Material.h"
#include <vector>

template <typename ComponentType>
class Model {
    private:
        using Triangle = Polygon3D<ComponentType, 3>;
        using Quadrilateral = Polygon3D<ComponentType, 4>;
        using NGon = NGon3D<ComponentType>;
        using Vector3 = Vector<ComponentType, 3>;
    public:
        std::vector<Triangle> triangles;
        std::vector<Quadrilateral> quadrilaterals;
        std::vector<NGon> ngons;
        std::string modelName;
        Material<ComponentType> material;

        Model(){

        };
        
};

#endif