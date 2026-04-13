#pragma once
#include "math.hpp"
#include "rasterizer.hpp"
#include <vector>

struct Transform {
    vec4 position;
    vec4 rotation; // Euler angles Roll(z) Pitch(x) Yaw(y)
    vec4 scale;
    mat4 get_matrix() const; // Combines translation * rotation * scale
};

struct SceneObject {
    Mesh mesh;
    Transform transform;
};
enum class ProjType { PERSPECTIVE,
    FISHEYE };

class Renderer {
    ProjType current_proj = ProjType::PERSPECTIVE;
    std::vector<SceneObject> scene;
    mat4 view_matrix;
    mat4 projection_matrix;
    bool transform_mesh(SceneObject& obj, std::vector<Vertex>& out_vertices);

public:
    void add_object(const SceneObject& obj);
    void set_camera(vec4 eye, vec4 center, vec4 up);
    void set_projection_perspective(float fov, float aspect, float near, float far);
    void set_projection_type(ProjType proj);
    
    void render_frame(Rasterizer& rasterizer);
};