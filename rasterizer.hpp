#pragma once
#include "mesh.hpp"

class Rasterizer {
    Image& target;

public:
    Rasterizer(Image& img)
        : target(img) { }

    // Линия по Брезенхэму
    void draw_line(vec2 p0, vec2 p1, Pixel color);

    void draw_triangle(Vertex v0, Vertex v1, Vertex v2, Pixel color);

private:
    // Ищет Область внутри треугольника
    float edge_function(vec2 a, vec2 b, vec2 c);
};