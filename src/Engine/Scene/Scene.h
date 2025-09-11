#ifndef SCENE_H
#define SCENE_H


#include <vector>
#include <string>
#include "../../Resources/ModelLoader/ModelLoader.h"
#include "../../Core/Math/Matrix.h"

class Scene {
    using Triangle3D = Polygon3D<float, 3>;
    private:
        ModelLoader<float> modelLoader;
        std::vector<Triangle3D> triangles;
        Matrix<float, 4, 4> worldMatrix, rotationMatrix, translationMatrix;

    public:
        Scene();
        bool LoadModel(const std::string& filepath);
        void Update();

        Matrix<float, 4, 4> GetFinalTransformationMatrix();

        const std::vector<Triangle3D>& GetTriangles() const {
            return triangles;
        };
};


#endif