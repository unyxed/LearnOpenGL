#version 330 core

/* input variables */
layout (location = 0) in vec3 aPos;

/* output variables */
out float xpos;

void main() {
    gl_Position = vec4(aPos, 1.0);
    xpos = aPos.x;
}
