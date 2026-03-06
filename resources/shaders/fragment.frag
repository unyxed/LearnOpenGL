#version 330 core 

out vec4 frag_color;

in vec3 our_color;
in vec2 tex_coord;


// texture sampler
uniform sampler2D texture1; 
uniform sampler2D texture2; 

void main() {
  // linearly interpolate between both textures (80% texture1, 20% texture2)
  frag_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), 0.2);
}

