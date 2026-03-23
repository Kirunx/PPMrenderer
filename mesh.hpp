#pragma once
#include "math.hpp"
#include "image.hpp"
#include <vector>

struct Vertex {
    vec4 position; // (x, y, z, 1.0)
    Pixel color;   // (r, g, b)
};

struct Triangle {
    Vertex v[3];
};

class Mesh {
public:
    std::vector<Triangle> triangles;

    static Mesh create_cube(float size, Pixel color);
    static Mesh create_sphere(float radius, int segments, Pixel color);
};