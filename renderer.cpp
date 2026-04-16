#include "renderer.hpp"
#include "config.hpp"
#include "mesh.hpp"

// std
#include <algorithm>

#include <iostream>
mat4 Transform::get_matrix() const {
    float radX = rotation.x * M_PI / 180.0f;
    float radY = rotation.y * M_PI / 180.0f;
    float radZ = rotation.z * M_PI / 180.0f;

    mat4 T = mat4::translation(position);
    mat4 S = mat4::scale(scale);
    mat4 RX = mat4::rotate_x(radX);
    mat4 RY = mat4::rotate_y(radY);
    mat4 RZ = mat4::rotate_z(radZ);

    mat4 model = T * RZ * RY * RX * S;
    return model;
}
void Renderer::add_object(const SceneObject& obj) {
    this->scene.push_back(obj);
}
void Renderer::set_camera(vec4 eye, vec4 center, vec4 up) {
    view_matrix = look_at(eye, center, up);
}
void Renderer::set_projection_perspective(float fov, float aspect, float near, float far) {
    projection_matrix = perspective(fov, aspect, near, far);
}
bool Renderer::transform_mesh(SceneObject& obj, std::vector<Vertex>& out_vertices) {

    switch (current_proj) {
    case ProjType::PERSPECTIVE: {
        mat4 model = obj.transform.get_matrix();
        mat4 mvp = projection_matrix * view_matrix * model;

        float min_dist = 4.0f;
        float max_dist = 12.0f;

        for (size_t i = 0; i < obj.mesh.vertices.size(); ++i) {

            vec4 p = mvp * obj.mesh.vertices[i].position;

            if (p.w < 0.1f)
                return false;
            float w_inv = 1.0f / p.w;
            float screen_x = (p.x * w_inv + 1.0f) * 0.5f * WIDTH;
            float screen_y = (1.0f - p.y * w_inv) * 0.5f * HEIGHT;

            // Маппинг не проектированной величины глубины в диапазоне 0-255
            float depth_0_255 = ((p.w - min_dist) / (max_dist - min_dist)) * 255.0f;
            depth_0_255 = std::clamp(depth_0_255, 0.0f, 255.0f);

            out_vertices[i].position = { screen_x, screen_y, depth_0_255, 1.0f };
            out_vertices[i].color = obj.mesh.vertices[i].color;
        }
        return true;
        break;
    }
    case ProjType::FISHEYE: {
        mat4 model = obj.transform.get_matrix();
        mat4 mv = view_matrix * model;

        for (size_t i = 0; i < obj.mesh.vertices.size(); ++i) {
            vec4 p = mv * obj.mesh.vertices[i].position;

            // В стандартной камере видимые объекты имеют p.z < 0
            // Инвертируем Z, чтобы работать с расстоянием "вперед"
            float depth_z = -p.z;

            if (depth_z <= 0.1f)
                return false; // Ближняя отсечка

            float r = sqrtf(p.x * p.x + p.y * p.y + depth_z * depth_z);

            float theta = acosf(depth_z / r);
            float phi = atan2f(p.y, p.x);

            // Параметры для настройки
            float intensity = 1.2f; // Степень "выпуклости" 
            float zoom = 5.0f; // Общий масштаб

            // Нормализуем угол (0 в центре, 1 на краю полусферы 90 градусов)
            float norm_theta = theta / (M_PI / 2.0f);

            // Применяем нелинейность
            // powf(norm_theta, 1.0f / intensity) заставит центр расширяться к краям
            float distorted_theta = powf(norm_theta, 1.0f / intensity) * (M_PI / 2.0f);

            float nx = (distorted_theta * cosf(phi)) / (M_PI / 2.0f) * zoom;
            float ny = (distorted_theta * sinf(phi)) / (M_PI / 2.0f) * zoom;

            float screen_x = (nx + 1.0f) * 0.5f * WIDTH;
            float screen_y = (1.0f - ny) * 0.5f * HEIGHT;

            // Используем реальное расстояние r для Z-буфера (чтобы не было артефактов)
            float z_val = std::clamp(((r - 0.1f) / 20.0f) * 255.0f, 0.0f, 255.0f);

            out_vertices[i].position = { screen_x, screen_y, z_val, 1.0f };
            out_vertices[i].color = obj.mesh.vertices[i].color;
        }
        return true;
        break;
    }
    }
    return false;
}
void Renderer::set_projection_type(ProjType proj) {
    current_proj = proj;
}
void Renderer::render_frame(Rasterizer& rasterizer) {
    static std::vector<Vertex> tv_buffer;
    for (auto& object : scene) {
        tv_buffer.resize(object.mesh.vertices.size());

        if (transform_mesh(object, tv_buffer)) {
            const auto& indices = object.mesh.indices;
            for (size_t i = 0; i < indices.size(); i += 3) {
                rasterizer.draw_triangle(
                    tv_buffer[indices[i]],
                    tv_buffer[indices[i + 1]],
                    tv_buffer[indices[i + 2]]);
                rasterizer.draw_triangle_outline(
                    tv_buffer[indices[i]],
                    tv_buffer[indices[i + 1]],
                    tv_buffer[indices[i + 2]]);
            }
        }
    }
}