#pragma once

// TODO: Later map into real render component to optimize for rendering
namespace Game::Render
{
    // Should this struct be implemented somewhere else
    struct Color
    {
        float r,g,b,a;
        explicit Color(const float r = 1.f, const float g = 1.f, const float b = 1.f, const float a = 1.f) : r(r), g(g), b(b), a(a) {}
    };

    // Omit some not-so-easy field -> BlendType, Depth, Lighting field (Metallic, Shininess)
    struct Material
    {
        char *vert_shader;  // Vertex shader name
        char *pixel_shader; // Pixel shader name

        bool visible; // Active State if this object should be rendered
        uint32_t render_prior; // Render Priority for GPU

        // Field for default shader
        Color color; // Color Albedo to input in shader
        Material() :
            vert_shader(nullptr),
            pixel_shader(nullptr),
            visible(false),
            render_prior(0),
            color(Color())
        {}
        explicit Material(char *vert_shader, char *pixel_shader) :
            vert_shader(vert_shader),
            pixel_shader(pixel_shader),
            visible(true),
            render_prior(0),
            color(Color())
        {}
        explicit Material(char *vert_shader, char *pixel_shader, const Color color) :
                    vert_shader(vert_shader),
                    pixel_shader(pixel_shader),
                    visible(true),
                    render_prior(0),
                    color(color)
        {}
        explicit Material(char *vert_shader, char *pixel_shader, const uint32_t render_prior) :
            vert_shader(vert_shader),
            pixel_shader(pixel_shader),
            visible(true),
            render_prior(render_prior),
            color(Color())
        {}
        explicit Material(char *vert_shader, char *pixel_shader, Color color, const uint32_t render_prior) :
            vert_shader(vert_shader),
            pixel_shader(pixel_shader),
            visible(true),
            render_prior(render_prior),
            color(color)
        {}
    };
}