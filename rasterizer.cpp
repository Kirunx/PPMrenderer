#include "rasterizer.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <vector>
void Rasterizer::draw_lineH(Vertex v0, Vertex v1) {
    if (v0.position.x > v1.position.x)
        std::swap(v0, v1);

    int dx = (int)v1.position.x - (int)v0.position.x;
    int dy = (int)v1.position.y - (int)v0.position.y;
    float z = v0.position.z;
    float dz = (dx != 0) ? (v1.position.z - v0.position.z) / (float)dx : 0;

    int dir = (dy < 0) ? -1 : 1;
    dy = std::abs(dy);

    int y = (int)v0.position.y;
    int p = 2 * dy - dx;

    for (int i = 0; i <= dx; ++i) {
        target.set_pixel((int)v0.position.x + i, y, z, v0.color);

        if (p >= 0) {
            y += dir;
            p -= 2 * dx;
        }
        p += 2 * dy;
        z += dz;
    }
}
void Rasterizer::draw_lineV(Vertex v0, Vertex v1) {
    if (v0.position.y > v1.position.y)
        std::swap(v0, v1);

    int dx = (int)v1.position.x - (int)v0.position.x;
    int dy = (int)v1.position.y - (int)v0.position.y;
    float z = v0.position.z;
    float dz = (dy != 0) ? (v1.position.z - v0.position.z) / (float)dy : 0;

    int dir = (dx < 0) ? -1 : 1;
    dx = std::abs(dx);

    int x = (int)v0.position.x;
    int p = 2 * dx - dy;

    for (int i = 0; i <= dy; ++i) {
        int y = (int)v0.position.y + i;
        target.set_pixel(x, y, z, v0.color);

        if (p >= 0) {
            x += dir;
            p -= 2 * dy;
        }
        p += 2 * dx;
        z += dz;
    }
}
void Rasterizer::draw_line(Vertex v0, Vertex v1) {
    if (std::abs(v1.position.x - v0.position.x) > std::abs(v1.position.y - v0.position.y)) {
        draw_lineH(v0, v1);
    } else {
        draw_lineV(v0, v1);
    }
}

void Rasterizer::draw_triangle(Vertex v1, Vertex v2, Vertex v3) {
    // Сортируем вершины по Y: v1 - верхняя
    if (v1.position.y > v2.position.y) std::swap(v1, v2);
    if (v1.position.y > v3.position.y) std::swap(v1, v3);
    if (v2.position.y > v3.position.y) std::swap(v2, v3);

    float total_height = v3.position.y - v1.position.y;
    if (total_height <= 0.0f) return; // Защита: треугольник вырожден в горизонтальную линию

    // Рисуем треугольник одним проходом
    for (int i = 0; i < total_height; i++) {
        // Определяем, в какой мы половине (верхней или нижней)
        bool second_half = i > v2.position.y - v1.position.y || v2.position.y == v1.position.y;
        float segment_height = second_half ? v3.position.y - v2.position.y : v2.position.y - v1.position.y;
        
        if (segment_height <= 0.0f) continue; // Защита от деления на 0

        float alpha = (float)i / total_height;
        // Коэффициент для короткой стороны
        float beta  = (float)(i - (second_half ? v2.position.y - v1.position.y : 0)) / segment_height;

        // X координаты
        float x_long = v1.position.x + (v3.position.x - v1.position.x) * alpha;
        float x_short = second_half ? 
                        v2.position.x + (v3.position.x - v2.position.x) * beta : 
                        v1.position.x + (v2.position.x - v1.position.x) * beta;

        // Z координаты
        float z_long = v1.position.z + (v3.position.z - v1.position.z) * alpha;
        float z_short = second_half ? 
                        v2.position.z + (v3.position.z - v2.position.z) * beta : 
                        v1.position.z + (v2.position.z - v1.position.z) * beta;

        int current_y = v1.position.y + i;

        fill_horizontal_line(current_y, x_long, z_long, x_short, z_short, v1.color);
    }
}
void Rasterizer::draw_dot(vec2 v, Pixel color) {
    target.set_pixel(v.x, v.y, 0, color);
}
void Rasterizer::fill_horizontal_line(int y, float x1, float z1, float x2, float z2, Pixel color) {
    if (x1 > x2) { std::swap(x1, x2); std::swap(z1, z2); }
    
    // Клиппинг по Y
    if (y < 0 || y >= target.height) return;
    
    // Клиппинг по X
    int start_x = std::max(0, (int)x1);
    int end_x = std::min(target.width - 1, (int)x2);
    if (start_x > end_x) return;

    float z = z1;
    float dz = (x2 != x1) ? (z2 - z1) / (x2 - x1) : 0;
    
    if (x1 < 0) z += dz * (0 - x1); 

    for (int x = start_x; x <= end_x; ++x) {
        uint8_t c = 255 - (uint8_t)z; 
        Pixel depth_color = {c, c, c}; 
        
        target.set_pixel(x, y, (int)z, depth_color);
        z += dz;
    }
}

void Rasterizer::draw_triangle_outline(Vertex v0, Vertex v1, Vertex v2) {
    draw_line(v0,v2);
    draw_line(v0,v1);
    draw_line(v1,v2);
}