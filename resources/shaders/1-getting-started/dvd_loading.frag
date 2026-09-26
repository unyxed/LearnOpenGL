#version 330 core

/* inputs */
in vec2 TexCoord;

/* uniforms */
uniform sampler2D texture1;

/* outputs */
out vec4 FragColor;

void main() {
    FragColor = texture(texture1, TexCoord);
}
