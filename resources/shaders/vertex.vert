#version 330 core 
layout (location = 0) in vec3 vertex_position; 
layout (location = 1) in vec3 vertex_color; 
layout (location = 2) in vec2 texture_coord;

out vec3 our_color;
out vec2 tex_coord;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(vertex_position, 1.0);
  our_color = vertex_color;
  tex_coord = vec2(texture_coord.x, texture_coord.y);
}
