#include "config.hpp"
#include "image.hpp"
#include "mesh.hpp"
#include "rasterizer.hpp"
#include "renderer.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <stdio.h>
int main() {
    char buf[256];
    for (int frames = 0; frames < 360*2; ++frames) {
        auto start = std::chrono::high_resolution_clock::now();

        Image img(WIDTH, HEIGHT);
        Rasterizer rstr(img);
        img.clear({ 255, 255, 255 });
        Mesh cube_mesh;
        cube_mesh.create_cube(0.5f, { 0, 0, 0 });
        cube_mesh.tesselate(5);
        SceneObject cube = { cube_mesh, { { 0.0f, 0.0f, 0.0f, 1.0f }, { 10.0f+frames/2, 0.0f+frames*2, 0.0f+frames, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } } };
        Renderer rndr;
        // rndr.set_projection_type(ProjType::FISHEYE);
        rndr.set_camera({ 0.0f, 0.0f, 5.0f, 1.0f }, { 0, 0, 0, 1 }, { 0, 1, 0, 0 });
        rndr.set_projection_perspective(0.33f, (float)WIDTH / HEIGHT, 0.1f, 12.0f);
        rndr.add_object(cube);
        rndr.render_frame(rstr);
        auto end = std::chrono::high_resolution_clock::now();

        snprintf(buf, sizeof(buf), "output-%03d.ppm", frames);
        const char *output = buf;
        img.save_to_ppm(output);

        std::chrono::duration<float> duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << "------------------------------------" << std::endl;
        std::cout << "Frame render completed in: " << ms << " ms" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
    return 0;
}