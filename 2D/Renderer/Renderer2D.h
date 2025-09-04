#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <vector>
#include "../../EngineMath/Vector.h"
#include <math.h>



class Renderer2D {
    using Color3 = Vector<uint8_t, 3>;
    using Color4 = Vector<uint8_t, 4>;
    private:
        SDL_Renderer* renderer;

    public:
        explicit Renderer2D(SDL_Renderer* sdlRenderer) : renderer(sdlRenderer) {}
        
        // Prevent copying
        Renderer2D(const Renderer2D&) = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;
        
        
        void clear() { SDL_RenderClear(renderer); }
        void present() { SDL_RenderPresent(renderer); }
        
        
        
        void setDrawColor(const Color3& color) {
            SDL_SetRenderDrawColor(renderer, color.components[0], color.components[1], color.components[2], 255);
        }

        void setDrawColor(const Color4& color) {
            SDL_SetRenderDrawColor(renderer, color.components[0], color.components[1], color.components[2], color.components[3]);
        }
        
                
        template <typename ComponentType>
        void drawPoint(const Vector<ComponentType,2>& point) {
            SDL_RenderDrawPoint(renderer, point.components[0], point.components[1]);
        }

        template <typename ComponentType>
        void drawLine(const Vector<ComponentType, 2>& start, const Vector<ComponentType, 2>& end) {
            SDL_RenderDrawLine(renderer, start.components[0], start.components[1], end.components[0], end.components[1]);
        }

        template <typename ComponentType>
        void drawRect(const Vector<ComponentType, 2>& position, const Vector<ComponentType, 2>& size) {
            SDL_Rect rect = {
                position.components[0],
                position.components[1],
                size.components[0],
                size.components[1]
            };
            SDL_RenderDrawRect(renderer, &rect);
        }

        template <typename ComponentType>
        void fillRect(const Vector<ComponentType, 2>& position, const Vector<ComponentType, 2>& size) {
            SDL_Rect rect = {
                position.components[0],
                position.components[1],
                size.components[0],
                size.components[1]
            };
            SDL_RenderFillRect(renderer, &rect);
        }


        template <typename ComponentType>
        void drawCircle(const Vector<ComponentType, 2>& center, float radius, int segments) {
            Vector<double, 2> copy(center);
            for (int i = 0; i < segments; i++) {
                float angle1 = 2.0f * M_PI * i / segments;
                float angle2 = 2.0f * M_PI * (i + 1) / segments;
                
                Vector<ComponentType, 2> point1({
                    copy.components[0] + radius * cos(angle1),
                    copy.components[1] + radius * sin(angle1)
                });
                
                Vector<ComponentType, 2> point2 ({
                    copy.components[0] + radius * cos(angle2),
                    copy.components[1] + radius * sin(angle2)
                });
                
                drawLine(point1, point2);
            }
        }


        template <typename ComponentType>
        void drawPolygon(const std::vector<Vector<ComponentType, 2>>& points) {
            if (points.size() < 2) return;
            
            for (size_t i = 0; i < points.size() - 1; i++) {
                drawLine(points[i], points[i + 1]);
            }
            drawLine(points.back(), points.front());
        }
                
};


#endif