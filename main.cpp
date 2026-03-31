#include "config.hpp"
#include "image.hpp"
#include "rasterizer.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

int main() {
    Image img(WIDTH, HEIGHT);
    Rasterizer rstr(img);
    
    img.clear({ 0, 0, 0 });
	// Вершины куба
    float s = 1.0f;
    vec4 cube_points[8] = {
        { -s, -s,  s, 1 }, {  s, -s,  s, 1 }, {  s,  s,  s, 1 }, { -s,  s,  s, 1 },
        { -s, -s, -s, 1 }, {  s, -s, -s, 1 }, {  s,  s, -s, 1 }, { -s,  s, -s, 1 }
    };
	// Индексы для построения треугольников
    int indices[36] = {
        0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4 
    };

    mat4 view = look_at({ 2.0f, 2.0f, 5.0f, 1.0f }, { 0, 0, 0, 1 }, { 0, 1, 0, 0 });
    mat4 proj = perspective(1.0f, (float)WIDTH / HEIGHT, 0.1f, 12.0f);
    mat4 mvp = proj * view;

    Vertex screen_vertices[8];
    
    // Костыль чтобы получить красивую картину глубины. по сути это диапазон в которой находиться куб от камеры
    float min_dist = 4.0f; 
    float max_dist = 7.0f; 

    for (int i = 0; i < 8; ++i) {
        vec4 p = mvp * cube_points[i];
        
        float w_inv = 1.0f / p.w;
        float screen_x = (p.x * w_inv + 1.0f) * 0.5f * WIDTH;
        float screen_y = (1.0f - p.y * w_inv) * 0.5f * HEIGHT;

        // Маппинг не проектированной величины глубины в диапозоне 0-255
        float depth_0_255 = ((p.w - min_dist) / (max_dist - min_dist)) * 255.0f;
        depth_0_255 = std::clamp(depth_0_255, 0.0f, 255.0f);

        screen_vertices[i] = { { screen_x, screen_y, depth_0_255, 1.0f }, { 255, 255, 255 } };
    }

    for (int i = 0; i < 36; i += 3) {
        rstr.draw_triangle(
            screen_vertices[indices[i]],
            screen_vertices[indices[i + 1]],
            screen_vertices[indices[i + 2]]
        );
    }

    img.save_to_ppm("cube_depth.ppm");
    return 0;
}