#include "rasterizer.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <vector>
// Возможно стоит добавить второй цвет для интерполяции и перегрузить функцию
void Rasterizer::draw_lineH(vec2 p0, vec2 p1, Pixel color) {
    if (p0.x > p1.x) {
        std::swap(p0.x, p1.x);
        std::swap(p0.y, p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    auto dir = (dy < 0) ? -1 : 1;
    dy *= dir;

    if (dx != 0) {
        int y = p0.y;
        int p = 2 * dy - dx;
        for (int i = 0; i < dx + 1; ++i) {
            target.set_pixel(p0.x + i, y, 0, color);
            if (p >= 0) {
                y += dir;
                p = p - 2 * dx;
            }
            p = p + 2 * dy;
        }
    }
}
void Rasterizer::draw_lineV(vec2 p0, vec2 p1, Pixel color) {
    if (p0.y > p1.y) {
        std::swap(p0.x, p1.x);
        std::swap(p0.y, p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;

    auto dir = (dx < 0) ? -1 : 1;
    dx *= dir;

    if (dy != 0) {
        int x = p0.x;
        int p = 2 * dx - dy;
        for (int i = 0; i < dy + 1; ++i) {
            target.set_pixel(x, p0.y + i, 0, color);
            if (p >= 0) {
                x += dir;
                p = p - 2 * dy;
            }
            p = p + 2 * dx;
        }
    }
}
void Rasterizer::draw_line(vec2 p0, vec2 p1, Pixel color) {
    if (std::abs(p1.x - p0.x) > std::abs(p1.y - p0.y)) {
        draw_lineH(p0, p1, color);
    } else {
        draw_lineV(p0, p1, color);
    }
}

void Rasterizer::draw_lineH(Vertex v0, Vertex v1, std::vector<Vertex>& v_stack) {
    if (v0.position.x > v1.position.x)
        std::swap(v0, v1);

    int dx = (int)v1.position.x - (int)v0.position.x;
    int dy = (int)v1.position.y - (int)v0.position.y;
    float z = v0.position.z;
    float dz = (dx != 0) ? (v1.position.z - v0.position.z) / (float)dx : 0;

    int dir = (dy < 0) ? -1 : 1;
    dy = std::abs(dy);

    int y = (int)v0.position.y;
    int p = 2 * dy - dx;

    for (int i = 0; i <= dx; ++i) {
        v_stack.push_back({ { (float)((int)v0.position.x + i), (float)y, z, 1.0f }, v0.color });

        if (p >= 0) {
            y += dir;
            p -= 2 * dx;
        }
        p += 2 * dy;
        z += dz;
    }
}

void Rasterizer::draw_lineV(Vertex v0, Vertex v1, std::vector<Vertex>& v_stack) {
    if (v0.position.y > v1.position.y)
        std::swap(v0, v1);

    int dx = (int)v1.position.x - (int)v0.position.x;
    int dy = (int)v1.position.y - (int)v0.position.y;
    float z = v0.position.z;
    float dz = (dy != 0) ? (v1.position.z - v0.position.z) / (float)dy : 0;

    int dir = (dx < 0) ? -1 : 1;
    dx = std::abs(dx);

    int x = (int)v0.position.x;
    int p = 2 * dx - dy;

    for (int i = 0; i <= dy; ++i) {
        int y = (int)v0.position.y + i;
        v_stack.push_back({ { (float)x, (float)y, z, 1.0f }, v0.color });

        if (p >= 0) {
            x += dir;
            p -= 2 * dy;
        }
        p += 2 * dx;
        z += dz;
    }
}
// Не рисует а лишь добавляет точки в стек для заполнения позже
void Rasterizer::draw_line(Vertex v0, Vertex v1, std::vector<Vertex>& v_stack) {
    if (std::abs(v1.position.x - v0.position.x) > std::abs(v1.position.y - v0.position.y)) {
        draw_lineH(v0, v1, v_stack);
    } else {
        draw_lineV(v0, v1, v_stack);
    }
}
bool Rasterizer::compare_by_y(const Vertex& a, const Vertex& b) {
    return a.position.y < b.position.y;
}
void Rasterizer::draw_triangle(Vertex v0, Vertex v1, Vertex v2) {
    this->v.push_back(v0);
    this->v.push_back(v1);
    this->v.push_back(v2);
    std::sort(this->v.begin(), this->v.end(), compare_by_y);
    this->draw_line(this->v[0], this->v[1], this->v_stack);
    this->draw_line(this->v[0], this->v[2], this->v_stack);
    this->draw_line(this->v[1], this->v[2], this->v_stack);
    std::sort(this->v_stack.begin(), this->v_stack.end(), compare_by_y);

    for (size_t i = 0; i < v_stack.size();) {
        int curr_y = v_stack[i].position.y;
        float min_x = v_stack[i].position.x, max_x = v_stack[i].position.x;
        float min_z = v_stack[i].position.z, max_z = v_stack[i].position.z;
        // Ищем все пиксели с одинаковой y
        while (i < v_stack.size() && (int)v_stack[i].position.y == curr_y) {
            if (v_stack[i].position.x < min_x) {
                min_x = v_stack[i].position.x;
                min_z = v_stack[i].position.z;
            }
            if (v_stack[i].position.x > max_x) {
                max_x = v_stack[i].position.x;
                max_z = v_stack[i].position.z;
            }
            i++;
        }
        draw_line_simple((int)min_x, min_z, (int)max_x, max_z, curr_y, v0.color);
    }
    this->v.clear();
    this->v_stack.clear();
}
void Rasterizer::draw_dot(vec2 v, Pixel color) {
    target.set_pixel(v.x, v.y, 0, color);
}
void Rasterizer::draw_line_simple(int x1, float z1, int x2, float z2, int y, Pixel color) {
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(z1, z2);
    }

    float z = z1;
    float dz = (x2 != x1) ? (z2 - z1) / (x2 - x1) : 0;

    for (int x = x1; x <= x2; ++x) {
        // Тестовое заполнение цвета по глубине
        uint8_t color_val = 255 - (uint8_t)std::clamp(z, 0.0f, 255.0f);
        Pixel z_color = { color_val, color_val, color_val };

        target.set_pixel(x, y, z, z_color);

        z += dz;
    }
}