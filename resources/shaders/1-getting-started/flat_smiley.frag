#version 330  core
/* inputs */
in vec3 OutColor;
in vec2 TexCoord;

/* uniforms */
uniform sampler2D texture1;
uniform sampler2D texture2;

/* outputs */
out vec4 FragColor;

void main() {
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
