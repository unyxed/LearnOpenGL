#version 330 core

/* inputs */
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

/* uniforms */
uniform mat4 transform;

/* ouputs */
out vec2 TexCoord;

void main() {
    gl_Position = transform * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
}
