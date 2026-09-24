#version 330 core

/* input variables */
in float xpos;

/* uniforms */
uniform float u_time;

/* output variables */
out vec4 FragColor;

void main() {
    float speed = 2.0f;
    float frequency = 3.0f;
    float wave = sin((u_time * speed)+(xpos * frequency));
    float v = wave * 0.5 + 0.5;
    FragColor = vec4(v, v, v, 1.0);
}
