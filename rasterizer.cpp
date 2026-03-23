#include "rasterizer.hpp"
#include <iostream>

// Возможно стоит добавить второй цвет для интерполяции и перегрузить функцию
void Rasterizer::draw_line(vec2 p0, vec2 p1, Pixel color) {
    int m_new = 2 * (p1.y - p0.y);
    int slope_error_new = m_new - (p1.x - p0.x);
    for (int x = p0.x, y = p0.y; x <= p1.x; x++) {
        std::cout << "(" << x << "," << y << ")\n";
        target.set_pixel(x,y,0,color);
        slope_error_new += m_new;

        if (slope_error_new >= 0) {
            y++;
            slope_error_new -= 2 * (p1.x - p0.x);
        }
    }
}

void Rasterizer::draw_triangle(Vertex v0, Vertex v1, Vertex v2, Pixel color) {
    
}
