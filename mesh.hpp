#pragma once
#include "image.hpp"
#include "math.hpp"
#include <vector>

struct Vertex {
    vec4 position; // (x, y, z, 1.0)
    Pixel color; // (r, g, b)

    vec2 get_2d_position() {
        return { position.x, position.y };
    }
};

struct Triangle {
    Vertex v[3];
};

class Mesh {
private:
    int add_midpoint(int i1, int i2);

public:
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    void create_cube(float size, Pixel color);
    void tesselate(int depth);
    Mesh create_sphere(float radius, int segments, Pixel color);
};
