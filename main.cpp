#include "config.hpp"
#include "image.hpp"
#include "mesh.hpp"
#include "rasterizer.hpp"
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
    Mesh cube;
    cube.create_cube(1, { 0, 0, 0 });

    float radX = 0.0f * M_PI / 180.0f;
    float radY = 0.0f * M_PI / 180.0f;
    float radZ = 0.0f * M_PI / 180.0f;

    // 2. Создаем базовые матрицы
    mat4 T = mat4::translation({ -0.7f, 0.1f, -1.0f, 1.0f });
    mat4 S = mat4::scale({ 1.0f, 1.0f, 1.0f, 1.0f });
    mat4 RX = mat4::rotate_x(radX);
    mat4 RY = mat4::rotate_y(radY);
    mat4 RZ = mat4::rotate_z(radZ);

    mat4 model = RZ * RY * RX * S * T;

    mat4 view = look_at({ 0.0f, 0.0f, 5.0f, 1.0f }, { 0, 0, 0, 1 }, { 0, 1, 0, 0 });
    mat4 proj = perspective(0.33f, (float)WIDTH / HEIGHT, 0.1f, 12.0f);

    mat4 mvp = proj * view * model;

    Vertex screen_vertices[8];

    // Костыль чтобы получить красивую картину глубины. по сути это диапазон в которой находиться куб от камеры
    float min_dist = 4.0f;
    float max_dist = 7.0f;

    for (auto& triangle : cube.triangles) {
        for (auto& v : triangle.v) {
            vec4 p = mvp * v.position;

            float w_inv = 1.0f / p.w;
            float screen_x = (p.x * w_inv + 1.0f) * 0.5f * WIDTH;
            float screen_y = (1.0f - p.y * w_inv) * 0.5f * HEIGHT;

            // Маппинг не проектированной величины глубины в диапозоне 0-255
            float depth_0_255 = ((p.w - min_dist) / (max_dist - min_dist)) * 255.0f;
            depth_0_255 = std::clamp(depth_0_255, 0.0f, 255.0f);

            v = { { screen_x, screen_y, depth_0_255, 1.0f }, { 0, 255, 255 } };
        }
    }

    for (int i = 0; i < cube.triangles.size(); i += 1) {
        rstr.draw_triangle(cube.triangles[i].v[0], cube.triangles[i].v[1], cube.triangles[i].v[2]);
    }

    img.save_to_ppm("cube_depth.ppm");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    float ms = duration.count() * 1000.0f;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Render completed in: " << ms << " ms" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    return 0;
}