#include "renderer.hpp"
#include "config.hpp"
#include "mesh.hpp"

// std
#include <algorithm>

#include <iostream>

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
    float radX = obj.transform.rotation.x * M_PI / 180.0f;
    float radY = obj.transform.rotation.y * M_PI / 180.0f;
    float radZ = obj.transform.rotation.z * M_PI / 180.0f;

    mat4 T = mat4::translation(obj.transform.position);
    mat4 S = mat4::scale(obj.transform.scale);
    mat4 RX = mat4::rotate_x(radX);
    mat4 RY = mat4::rotate_y(radY);
    mat4 RZ = mat4::rotate_z(radZ);

    mat4 model = T * RZ * RY * RX * S;

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
            }
        }
    }
}