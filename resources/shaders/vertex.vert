#version 330 core 
layout (location = 0) in vec3 vertex_position; 
layout (location = 1) in vec3 vertex_color; 
layout (location = 2) in vec2 texture_coord;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  //gl_Position = transform * vec4(vertex_position, 1.0);
	gl_Position = projection * view * model * vec4(vertex_position , 1.0);
  tex_coord = vec2(texture_coord.x, texture_coord.y);
}
