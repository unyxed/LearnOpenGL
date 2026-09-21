#version 330 core

in vec2 tex2d_coord;

out vec4 frag_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	frag_color = mix(texture(texture1, tex2d_coord), texture(texture2, tex2d_coord), 0.2);
}