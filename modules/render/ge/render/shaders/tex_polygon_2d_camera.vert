#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform ViewProj
{
    vec2 pos;
    vec2 ortho_proj;
} camera;

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coord;

layout(location = 0) out vec2 frag_tex_coord;

void main()
{
    gl_Position = vec4((in_position - camera.pos) * camera.ortho_proj, 0.0, 1.0);
    frag_tex_coord = in_tex_coord;
}
