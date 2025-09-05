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
        void putCirclePoints(int xc, int yc, int x, int y){
            drawPoint(xc+x, yc+y);
            drawPoint(xc-x, yc+y);
            drawPoint(xc+x, yc-y);
            drawPoint(xc-x, yc-y);
            drawPoint(xc+y, yc+x);
            drawPoint(xc-y, yc+x);
            drawPoint(xc+y, yc-x);
            drawPoint(xc-y, yc-x);
        }
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

        void drawPoint(int x, int y){
            SDL_RenderDrawPoint(renderer, x, y);
        }

        template <typename ComponentType>
        void drawPointWithCustomWidth(const Vector<ComponentType,2>& point, float width) {
            drawPointWithCustomWidth(point[0], point[1], width);
        }

        void drawPointWithCustomWidth(int x, int y, float width){
            SDL_FRect pointRectangle{
                x,
                y,
                width,
                width
            };
            SDL_RenderDrawRectF(renderer, &pointRectangle);
        }

        template <typename ComponentType>
        void drawLine(const Vector<ComponentType, 2>& start, const Vector<ComponentType, 2>& end) {
            SDL_RenderDrawLine(renderer, start.components[0], start.components[1], end.components[0], end.components[1]);
        }

        void drawLine(int x1, int y1, int x2, int y2){
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
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

        void fillRect(int x1, int y1, int x2, int y2){
            SDL_Rect rect = {
                x1, y1, x2 - x1, y2 - y1
            };
            SDL_RenderFillRect(renderer, &rect);
        }

        template<typename ComponentType>
        void drawCircle(const Vector<ComponentType, 2> &center, int radius){
            drawCircle(center[0], center[1], radius);
        }

        void drawCircle(int centerX, int centerY, int radius){
            int x = 0, y = radius;
            int d = 3 - 2 * radius;
            putCirclePoints(centerX, centerY, x, y);
            while (y >= x){
                if (d > 0) {
                    y--; 
                    d = d + 4 * (x - y) + 10;
                }
                else
                    d = d + 4 * x + 6;
                x++;
                putCirclePoints(centerX, centerY, x, y);
            }
        }

        template<typename ComponentType>
        void fillCircle(const Vector<ComponentType, 2> &center, int radius){
            fillCircle(center[0], center[1], radius);
        }

        void fillCircle(int centerX, int centerY, int radius) {
            //http://fredericgoset.ovh/mathematiques/courbes/en/filled_circle.html
            int x = 0;
            int y = radius;
            int m = 5 - 4 * radius;

            while (x <= y) {
                fillRect(centerX - y, centerY - x, centerX + y + 1, centerY - x + 1);
                fillRect(centerX - y, centerY + x, centerX + y + 1, centerY + x + 1);

                if (m > 0) {
                    fillRect(centerX - x, centerY - y, centerX + x + 1, centerY - y + 1);
                    fillRect(centerX - x, centerY + y, centerX + x + 1, centerY + y + 1);
                    y--;
                    m -= 8 * y;
                }
                x++;
                m += 8 * x + 4;
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