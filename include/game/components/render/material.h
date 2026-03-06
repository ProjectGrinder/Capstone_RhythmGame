#pragma once

// TODO: Later map into real render component to optimize for rendering
namespace Game::Render
{
    // Should this struct be implemented somewhere else
    struct Color
    {
        float r, g, b, a;
        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
        {}
        Color(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a)
        {}
    };

    // Omit some not-so-easy field -> BlendType, Depth, Lighting field (Metallic, Shininess)
    struct Material
    {
        char *vert_shader{}; // Vertex shader name
        InputAttributeDescription *vert_shader_input_attributes{};
        size_t vert_shader_input_attributes_count{};
        char *pixel_shader{}; // Pixel shader name
        InputAttributeDescription *pixel_shader_input_attributes{};
        size_t pixel_shader_input_attributes_count{};

        bool visible{};          // Active State if this object should be rendered
        uint32_t render_prior{}; // Render Priority for GPU
        Color color{};           // Color Albedo to input in shader

        Material(
                char *vert_shader,
                InputAttributeDescription *vert_shader_input_attributes,
                size_t vert_shader_input_attributes_count,
                char *pixel_shader,
                InputAttributeDescription *pixel_shader_input_attributes,
                size_t pixel_shader_input_attributes_count) :
            vert_shader(vert_shader),
            vert_shader_input_attributes(vert_shader_input_attributes),
            vert_shader_input_attributes_count(vert_shader_input_attributes_count),
            pixel_shader(pixel_shader),
            pixel_shader_input_attributes(pixel_shader_input_attributes),
            pixel_shader_input_attributes_count(pixel_shader_input_attributes_count)
        {}

        Material(
                char *vert_shader,
                InputAttributeDescription *vert_shader_input_attributes,
                size_t vert_shader_input_attributes_count,
                char *pixel_shader,
                InputAttributeDescription *pixel_shader_input_attributes,
                size_t pixel_shader_input_attributes_count,
                uint32_t render_prior) :
            vert_shader(vert_shader),
            vert_shader_input_attributes(vert_shader_input_attributes),
            vert_shader_input_attributes_count(vert_shader_input_attributes_count),
            pixel_shader(pixel_shader),
            pixel_shader_input_attributes(pixel_shader_input_attributes),
            pixel_shader_input_attributes_count(pixel_shader_input_attributes_count),
            render_prior(render_prior)
        {}
    };
} // namespace Game::Render
