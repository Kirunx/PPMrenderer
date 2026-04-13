#include "config.hpp"
#include "image.hpp"
#include "mesh.hpp"
#include "rasterizer.hpp"
#include "renderer.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <vector>
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    Image img(WIDTH, HEIGHT);
    Rasterizer rstr(img);
    img.clear({ 0, 0, 0 });
    Mesh cube_mesh;
    cube_mesh.create_cube(0.5f, { 0, 0, 0 });
    SceneObject cube = { cube_mesh, { { 0.0f, 0.0f, -1.3f, 1.0f }, { 20.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } } };
    Renderer rndr;
    rndr.set_camera({ 0.0f, 0.0f, 5.0f, 1.0f }, { 0, 0, 0, 1 }, { 0, 1, 0, 0 });
    rndr.set_projection_perspective(0.33f, (float)WIDTH / HEIGHT, 0.1f, 12.0f);
    rndr.add_object(cube);
    rndr.render_frame(rstr);
    auto end = std::chrono::high_resolution_clock::now();

    img.save_to_ppm("cube_depth.ppm");

    std::chrono::duration<float> duration = end - start;
    float ms = duration.count() * 1000.0f;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Render completed in: " << ms << " ms" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    return 0;
}