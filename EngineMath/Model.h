#include "Polygon.h"
#include "Vertex.h"
#include "Material.h"
#include <vector>

#ifndef MODEL_H
#define MODEL_H

template <typename ComponentType>
class Model {
    private:
        using Triangle = Polygon<ComponentType, 3>;
        using Quadrilateral = Polygon<ComponentType, 4>;
        using NGon = NPolygon<ComponentType>;
        using Vector3 = Vector<ComponentType, 3>;
        using Vertex3 = Vertex3<ComponentType>;
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