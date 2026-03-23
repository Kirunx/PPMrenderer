#include "rasterizer.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>
// Возможно стоит добавить второй цвет для интерполяции и перегрузить функцию
void Rasterizer::draw_lineH(vec2 p0, vec2 p1, Pixel color) {
    if (p0.x > p1.x) {
        std::swap(p0.x,p1.x);
        std::swap(p0.y,p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    auto dir = (dy < 0) ? -1 : 1;
    dy *= dir;

    if (dx != 0) {
        int y = p0.y;
        int p = 2 * dy - dx;
        for (int i = 0; i < dx + 1; ++i) {
            target.set_pixel(p0.x + i, y, 0, color);
            if (p >= 0) {
                y += dir;
                p = p - 2 * dx;
            }
            p = p + 2 * dy;
        }
    }
}
void Rasterizer::draw_lineV(vec2 p0, vec2 p1, Pixel color) {
    if (p0.y > p1.y) {
        std::swap(p0.x,p1.x);
        std::swap(p0.y,p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    auto dir = (dx < 0) ? -1 : 1;
    dx *= dir;

    if (dy != 0) {
        int x = p0.x;
        int p = 2 * dx - dy;
        for (int i = 0; i < dy + 1; ++i) {
            target.set_pixel(x, p0.y + i, 0, color);
            if (p >= 0) {
                x += dir;
                p = p - 2 * dy;
            }
            p = p + 2 * dx;
        }
    }
}
void Rasterizer::draw_line(vec2 p0, vec2 p1, Pixel color) {
    if (std::abs(p1.x - p0.x) > std::abs(p1.y - p0.y)){
        draw_lineH(p0,p1,color);
    } else {
        draw_lineV(p0,p1,color);
    }

}

void Rasterizer::draw_triangle(Vertex v0, Vertex v1, Vertex v2) {
}
