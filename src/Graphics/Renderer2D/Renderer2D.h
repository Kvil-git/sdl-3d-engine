#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <vector>
#include <array>
#include <math.h>
#include <algorithm>
#include "../../Core/Math/Vector.h"
#include "../../Core/Geometry/Polygon.h"

#include <iostream>


class Renderer2D {
    using Color3 = Vector<uint8_t, 3>;
    using Color4 = Vector<uint8_t, 4>;
    using Triangle2D = Polygon2D<float, 3>;
    using Vector2 = Vector<float, 2>;
    private:
        SDL_Renderer* renderer;
        void PutCirclePoints(int xc, int yc, int x, int y){
            DrawPoint(xc+x, yc+y);
            DrawPoint(xc-x, yc+y);
            DrawPoint(xc+x, yc-y);
            DrawPoint(xc-x, yc-y);
            DrawPoint(xc+y, yc+x);
            DrawPoint(xc-y, yc+x);
            DrawPoint(xc+y, yc-x);
            DrawPoint(xc-y, yc-x);
        }

        void FillFlatTopTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3) {
            float invslope1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
            float invslope2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

            float curx1 = v1[0];
            float curx2 = v2[0];

            for (int scanlineY = static_cast<int>(v1[1]); scanlineY <= static_cast<int>(v3[1]); scanlineY++) {
                DrawLine(static_cast<int>(curx1), scanlineY, static_cast<int>(curx2), scanlineY);
                curx1 += invslope1;
                curx2 += invslope2;
            }
        }

        void FillFlatBottomTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3) {
            float invslope1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
            float invslope2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

            float curx1 = v1[0];
            float curx2 = v1[0];

            for (int scanlineY = static_cast<int>(v1[1]); scanlineY <= static_cast<int>(v2[1]); scanlineY++) {
                DrawLine(static_cast<int>(curx1), scanlineY, static_cast<int>(curx2), scanlineY);
                curx1 += invslope1;
                curx2 += invslope2;
            }
        }

    public:
        explicit Renderer2D(SDL_Renderer* sdlRenderer) : renderer(sdlRenderer) {}
        
        // Prevent copying
        Renderer2D(const Renderer2D&) = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        
        
        void Clear() { SDL_RenderClear(renderer); }
        void Present() { SDL_RenderPresent(renderer); }
        
        
        
        void SetDrawColor(const Color3& color) {
            SDL_SetRenderDrawColor(renderer, color.components[0], color.components[1], color.components[2], 255);
        }

        void SetDrawColor(const Color4& color) {
            SDL_SetRenderDrawColor(renderer, color.components[0], color.components[1], color.components[2], color.components[3]);
        }
        
                
        template <typename ComponentType>
        void DrawPoint(const Vector<ComponentType,2>& point) {
            SDL_RenderDrawPoint(renderer, point.components[0], point.components[1]);
        }

        void DrawPoint(int x, int y){
            SDL_RenderDrawPoint(renderer, x, y);
        }

        template <typename ComponentType>
        void DrawPointWithCustomWidth(const Vector<ComponentType,2>& point, float width) {
            DrawPointWithCustomWidth(point[0], point[1], width);
        }

        void DrawPointWithCustomWidth(float x, float y, float width){
            SDL_FRect pointRectangle{
                x,
                y,
                width,
                width
            };
            SDL_RenderDrawRectF(renderer, &pointRectangle);
        }

        template <typename ComponentType>
        void DrawLine(const Vector<ComponentType, 2>& start, const Vector<ComponentType, 2>& end) {
            SDL_RenderDrawLine(renderer, start.components[0], start.components[1], end.components[0], end.components[1]);
        }

        void DrawLine(int x1, int y1, int x2, int y2){
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }

        template <typename ComponentType>
        void DrawRect(const Vector<ComponentType, 2>& position, const Vector<ComponentType, 2>& size) {
            SDL_Rect rect = {
                position.components[0],
                position.components[1],
                size.components[0],
                size.components[1]
            };
            SDL_RenderDrawRect(renderer, &rect);
        }

        template <typename ComponentType>
        void FillRect(const Vector<ComponentType, 2>& position, const Vector<ComponentType, 2>& size) {
            SDL_Rect rect = {
                position.components[0],
                position.components[1],
                size.components[0],
                size.components[1]
            };
            SDL_RenderFillRect(renderer, &rect);
        }

        void FillRect(int x1, int y1, int x2, int y2){
            SDL_Rect rect = {
                x1, y1, x2 - x1, y2 - y1
            };
            SDL_RenderFillRect(renderer, &rect);
        }

        template<typename ComponentType>
        void DrawCircle(const Vector<ComponentType, 2> &center, int radius){
            DrawCircle(center[0], center[1], radius);
        }

        void DrawCircle(int centerX, int centerY, int radius){
            int x = 0, y = radius;
            int d = 3 - 2 * radius;
            PutCirclePoints(centerX, centerY, x, y);
            while (y >= x){
                if (d > 0) {
                    y--; 
                    d = d + 4 * (x - y) + 10;
                }
                else
                    d = d + 4 * x + 6;
                x++;
                PutCirclePoints(centerX, centerY, x, y);
            }
        }

        template<typename ComponentType>
        void FillCircle(const Vector<ComponentType, 2> &center, int radius){
            FillCircle(center[0], center[1], radius);
        }

        void FillCircle(int centerX, int centerY, int radius) {
            //http://fredericgoset.ovh/mathematiques/courbes/en/filled_circle.html
            int x = 0;
            int y = radius;
            int m = 5 - 4 * radius;

            while (x <= y) {
                FillRect(centerX - y, centerY - x, centerX + y + 1, centerY - x + 1);
                FillRect(centerX - y, centerY + x, centerX + y + 1, centerY + x + 1);

                if (m > 0) {
                    FillRect(centerX - x, centerY - y, centerX + x + 1, centerY - y + 1);
                    FillRect(centerX - x, centerY + y, centerX + x + 1, centerY + y + 1);
                    y--;
                    m -= 8 * y;
                }
                x++;
                m += 8 * x + 4;
            }

        }
        
        template <typename ComponentType>
        void DrawPolygon(const std::vector<Vector<ComponentType, 2>>& points) {
            if (points.size() < 2) return;
            
            for (size_t i = 0; i < points.size() - 1; i++) {
                DrawLine(points[i], points[i + 1]);
            }
            DrawLine(points.back(), points.front());
        }

        template <typename ComponentType>
        void DrawTriangle(const Polygon2D<ComponentType, 3> &triangle) {
            if (triangle.vertices.size() < 2) return;
            
            for (size_t i = 0; i < triangle.vertices.size() - 1; i++) {
                DrawLine(triangle.vertices[i], triangle.vertices[i + 1]);
            }
            DrawLine(triangle.vertices.back(), triangle.vertices.front());
        }

        template <typename ComponentType>
        void FillTriangle(const Polygon2D<ComponentType, 3>& triangle) {
            std::array<Vector2, 3> vertices = { triangle.vertices[0], triangle.vertices[1], triangle.vertices[2] };
            std::sort(vertices.begin(), vertices.end(), [](const Vector2& a, const Vector2& b) {
                return a[1] < b[1];
            });

            const Vector2 &v1 = vertices[0], &v2 = vertices[1], &v3 = vertices[2];
            if (v2[1] == v3[1]) {
                FillFlatTopTriangle(v1, v2, v3);
            } else if (v1[1] == v2[1]) {
                FillFlatBottomTriangle(v1, v2, v3);
            } else {
                float t = (v2[1] - v1[1]) / (v3[1] - v1[1]);
                Vector2 v4 = v1 + (v3 - v1) * t;
                FillFlatBottomTriangle(v1, v2, v4);
                FillFlatTopTriangle(v2, v4, v3);
            }
        }
                
        
};


#endif