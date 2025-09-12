#include <algorithm>
#include "Renderer3D.h"
#include "../../Core/Math/Vector.h"
#include "../../Enums/Colors.h"

void Renderer3D::Render(const std::vector<Triangle3D> &triangles, const Matrix<float, 4, 4> &transformationMatrix,
            const Matrix<float, 4, 4> &projectionMatrix, const Vector<float, 3>& cameraPosition){

    std::vector<Triangle3D> transformedTriangles;
    transformedTriangles.reserve(triangles.size());

    for (auto& triangle : triangles) {
        Triangle3D transformed = triangle.CopyTransformedByMatrix4x4(projectionMatrix * transformationMatrix);
        Vector<float, 3> normal = transformed.GetNormal();
        if (normal.SquaredComponentSum() < 1e-10f) {
            continue;
        }
        if ((normal * (transformed.vertices[0].position - cameraPosition)) < -0.01f) {
            continue;
        }


        transformedTriangles.push_back(transformed);
    }



    // Sort by z depth (painter's algorithm)
    std::sort(transformedTriangles.begin(), transformedTriangles.end(),
    [](const Triangle3D &a, const Triangle3D &b) {
        float z1 = (a.vertices[0].position[2] + a.vertices[1].position[2] + a.vertices[2].position[2]) / 3.0f;
        float z2 = (b.vertices[0].position[2] + b.vertices[1].position[2] + b.vertices[2].position[2]) / 3.0f;
        return z1 < z2;
    });



    for (const auto& transformed : transformedTriangles) {
        auto projected = transformed.ToPolygon2D();
        
        for (int i = 0; i < 3; i++) {
            float x = transformed.vertices[i].position[0];
            float y = transformed.vertices[i].position[1];

            x = (x + 1.0f) * 0.5f * windowWidth;
            y = (y + 1.0f) * 0.5f * windowHeight;

            projected.vertices[i] = Vector<float, 2>(x, y);
        }
        
        renderer2D->SetDrawColor(Colors::White);
        renderer2D->FillTriangle(projected);

        renderer2D->SetDrawColor(Colors::Black);
        renderer2D->DrawTriangle(projected);
    }
}