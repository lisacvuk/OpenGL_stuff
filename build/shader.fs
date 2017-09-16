#version 330 core
in vec2 Texcoord;

out vec4 color;

uniform sampler2D tex;
void main(){
  color = texture(tex, Texcoord); //* vec4(vec3(1,0,0), 1.0);
}
