#pragma once
#include "mesh.hpp"

class Rasterizer {
    Image& target;

public:
    Rasterizer(Image& img)
        : target(img) { }
    void draw_dot(vec2 v, Pixel color);
    // Линия по Брезенхэму
    void draw_line(vec2 p0, vec2 p1, Pixel color);
    // Перегрузка для растеризатора
    void draw_line(vec2 p0, vec2 p1, Pixel color,std::vector<Vertex> &v_stack);

    void draw_triangle(Vertex v0, Vertex v1, Vertex v2);

private:
    // Ищет Область внутри треугольника
    float edge_function(vec2 a, vec2 b, vec2 c);
    void draw_lineH(vec2 p0, vec2 p1, Pixel color);
    void draw_lineH(vec2 p0, vec2 p1, Pixel color,std::vector<Vertex> &v_stack);
    void draw_lineV(vec2 p0, vec2 p1, Pixel color);
    void draw_lineV(vec2 p0, vec2 p1, Pixel color, std::vector<Vertex> &v_stack);

    // Helper функция для сортировки перед растеризации
    static bool compare_by_y(const Vertex& a, const Vertex& b);
};