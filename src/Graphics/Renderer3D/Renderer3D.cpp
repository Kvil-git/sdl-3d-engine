#include <algorithm>
#include "Renderer3D.h"
#include "../../EngineMath/Vector.h"
#include "../../Shared/Enums/Colors.h"

void Renderer3D::Render(const std::vector<Triangle3D> &triangles, const Matrix<float, 4, 4> &viewProjectionMatrix, const Vector<float, 3>& cameraPosition){

    std::vector<Triangle2D> projectedTriangles;

    for (auto& triangle : triangles) {
        Triangle3D transformed = triangle.CopyTransformedByMatrix4x4(viewProjectionMatrix);
        Vector<float, 3> normal = transformed.GetNormal();
        if (normal.SquaredComponentSum() < 1e-10f) {
            continue;
        }
        if ((normal * (transformed.vertices[0].position - cameraPosition)) < -0.01f) {
            continue;
        }

        auto projected = transformed.ToPolygon2D();
        projected *= 700.0f;
        projected += Vector<float, 2>(this->windowWidth/2, windowHeight/2);

        projectedTriangles.push_back(projected);
    }

    // Sort by z depth (painter's algorithm)
    std::sort(projectedTriangles.begin(), projectedTriangles.end(),
        [](const Polygon2D<float, 3>& a, const Polygon2D<float, 3>& b) {
            float z1 = (a.vertices[0][2] + a.vertices[1][2] + a.vertices[2][2]) / 3.0f;
            float z2 = (b.vertices[0][2] + b.vertices[1][2] + b.vertices[2][2]) / 3.0f;
            return z1 < z2;
        });

    for (auto& tri : projectedTriangles) {
        renderer2D->SetDrawColor(Colors::Green);
        renderer2D->FillTriangle(tri);
        renderer2D->SetDrawColor(Colors::Blue);
        renderer2D->DrawTriangle(tri);
    }
}