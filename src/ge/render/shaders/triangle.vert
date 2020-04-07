#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform ViewProj
{
    vec2 pos;
    vec2 ortho_proj;
} camera;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = vec4((inPosition - camera.pos) * camera.ortho_proj, 0.0, 1.0);
    fragColor = inColor;
}
