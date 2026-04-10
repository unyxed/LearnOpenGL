#version 330 core
layout (location = 0) in vec3 vertex_coordinates;
layout (location = 1) in vec2 texture2d_coordinates;

out vec2 tex2d_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(vertex_coordinates, 1.0);
	tex2d_coord = vec2(texture2d_coordinates.x, texture2d_coordinates.y);
}