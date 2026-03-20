#pragma once 
#include "math.hpp"
#include "rasterizer.hpp"
#include <vector>

struct Transform {
    vec4 position;
    vec4 rotation; // Euler angles
    vec4 scale;
    mat4 get_matrix() const; // Combines translation * rotation * scale
};

struct SceneObject {
    Mesh mesh;
    Transform transform;
};

class Renderer {
    std::vector<SceneObject> scene;
    mat4 view_matrix;
    mat4 projection_matrix;

public:
    void add_object(const SceneObject& obj);
    void set_camera(vec4 eye, vec4 center, vec4 up);
    void set_projection(float fov, float aspect, float near, float far);
    
    // The big one: processes the whole scene and draws it
    void render_frame(Rasterizer& rasterizer);
};