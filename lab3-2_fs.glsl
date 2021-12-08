#version 440

out vec3 frag_colour;
in vec3 normal;
in vec3 position;
uniform samplerCube env_sampler;
vec4 tex = texture(env_sampler, reflect(position, normal));

void main () {
  
  frag_colour = tex.rgb;
};
