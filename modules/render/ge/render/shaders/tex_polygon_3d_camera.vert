#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Mvp
{
    mat4 transform;
} mvp;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coord;

layout(location = 0) out vec2 frag_tex_coord;

void main()
{
    gl_Position = mvp.transform * vec4(in_position, 1.0);
    frag_tex_coord = in_tex_coord;
}
