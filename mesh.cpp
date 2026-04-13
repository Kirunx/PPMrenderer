#include "mesh.hpp"
void Mesh::create_cube(float size, Pixel color) {
    vertices = {
        {{ -size, -size, size, 1,},{color}}, {{ size, -size, size, 1 },{color}}, {{ size, size, size, 1 },{color}}, {{ -size, size, size, 1 },{color}},
        {{ -size, -size, -size, 1 },{color}}, {{ size, -size, -size, 1},{color}}, {{ size, size, -size, 1 },{color}}, {{ -size, size, -size, 1 },{color}}
    };
    indices = {
        0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4
    };
}
