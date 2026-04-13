#pragma once
#include "mesh.hpp"

class Rasterizer {
    Image& target;

public:
    Rasterizer(Image& img)
        : target(img) { }
    void draw_dot(vec2 v, Pixel color);
    // Линия по Брезенхэму
    void draw_line(Vertex v0, Vertex v1);

    void draw_triangle(Vertex v0, Vertex v1, Vertex v2);
    void draw_triangle_outline(Vertex v0, Vertex v1, Vertex v2);
private:
    std::vector<Vertex> v, v_stack;
    void draw_lineH(Vertex v0, Vertex v1);
    void draw_lineV(Vertex v0, Vertex v1);

    // Функция для заполнения треугольника
    void fill_horizontal_line(int y, float x1, float z1, float x2, float z2, Pixel color);

};
