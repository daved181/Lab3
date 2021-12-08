#version 440

out vec3 frag_colour;
in vec3 normal;
in vec3 position;
in vec2 texcoord;
uniform sampler2D tex_sampler;
uniform samplerCube env_sampler;
vec4 tex = texture(tex_sampler, texcoord);

void main () {
  ;
  frag_colour = tex.rgb;
};
