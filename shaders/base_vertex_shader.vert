#version 110

uniform mat4 MVP;

attribute vec2 vPos;
attribute vec3 vCol;
attribute vec2 vUV;

varying vec3 v_Color;
varying vec2 texCoord;

void main() {
  gl_Position = MVP * vec4(vPos, 0.0, 1.0);

  v_Color = vCol;
  texCoord = vUV;
}
