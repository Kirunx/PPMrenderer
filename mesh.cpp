#include "mesh.hpp"
void Mesh::create_cube(float size, Pixel color) {
    vertices = {
        { {
              -size,
              -size,
              size,
              1,
          },
            { color } },
        { { size, -size, size, 1 }, { color } }, { { size, size, size, 1 }, { color } }, { { -size, size, size, 1 }, { color } },
        { { -size, -size, -size, 1 }, { color } }, { { size, -size, -size, 1 }, { color } }, { { size, size, -size, 1 }, { color } }, { { -size, size, -size, 1 }, { color } }
    };
    indices = {
        0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4
    };
}

void Mesh::tesselate(int depth) {
    for (int d = 0; d < depth; ++d) {
        std::vector<int> next_indices;
        for (size_t i = 0; i < indices.size(); i += 3) {
            int a = indices[i];
            int b = indices[i + 1];
            int c = indices[i + 2];

            // Создаем новые вершины на серединах сторон
            int ab = add_midpoint(a, b);
            int bc = add_midpoint(b, c);
            int ca = add_midpoint(c, a);

            // Один треугольник превращается в четыре
            int new_tris[] = {
                a, ab, ca,
                b, bc, ab,
                c, ca, bc,
                ab, bc, ca
            };
            next_indices.insert(next_indices.end(), std::begin(new_tris), std::end(new_tris));
        }
        indices = next_indices;
    }
}

int Mesh::add_midpoint(int i1, int i2) {
    Vertex v1 = vertices[i1];
    Vertex v2 = vertices[i2];
    Vertex mid;
    mid.position = (v1.position + v2.position) * 0.5f;
    mid.color = v1.color; 
    vertices.push_back(mid);
    return vertices.size() - 1;
}