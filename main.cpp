#include "config.hpp"
#include "image.hpp"
#include "rasterizer.hpp"
int main() {
    Image img(WIDTH, HEIGHT);
    Rasterizer rstr(img);

    img.clear({ 0, 0, 0 });
    Vertex v1 = { { 400, 150, 0, 1 }, { 255, 0, 0 } };
    Vertex v2 = { { 650, 650, 20, 1 }, { 0, 255, 0 } };
    Vertex v3 = { { 150, 650, 200, 1 }, { 0, 0, 255 } };

    rstr.draw_triangle(v1, v2, v3);

    img.save_to_ppm("test.ppm");
    return 0;
}