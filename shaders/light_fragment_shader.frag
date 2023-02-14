#version 110

varying vec3 v_Color;
varying vec2 texCoord;

uniform float uCircle;
uniform float uTime;
uniform sampler2D tex0;

vec2 rotate(vec2 uv, float th) {
  return mat2(cos(th), -sin(th), sin(th), cos(th)) * uv;
}

float sdStar5(vec2 p, float r, float rf) {
  const vec2 k1 = vec2(0.809016994375, -0.587785252292);
  const vec2 k2 = vec2(-k1.x, k1.y);

  p.x = abs(p.x);
  p -= 2.0 * max(dot(k1, p), 0.0) * k1;
  p -= 2.0 * max(dot(k2, p), 0.0) * k2;
  p.x = abs(p.x);
  p.y -= r;
  
  vec2 ba = rf * vec2(-k1.y, k1.x) - vec2(0, 1);
  float h = clamp(dot(p,ba) / dot(ba,ba), 0.0, r);
  
  return length(p-ba*h) * sign(p.y * ba.x - p.x * ba.y);
}

void main()
{
  float pulse = sin(uTime * 3.0) * 1.5 + 1.5;
  vec2 uv = texCoord;
  uv -= 0.5;

  float d = 0.0; 
  if (uCircle == 1.0) {
    d = length(rotate(uv, pulse)) - 0.2;
  }
  if (uCircle == 2.0){
    d = sdStar5(rotate(uv, uTime), 0.24, 0.45);
  } 

  vec3 col = vec3(step(0., -d));
  float glow = 0.01 / d;
  glow = clamp(glow, 0., 1.);
  
  col += glow * pulse;
  col += clamp(vec3(0.001 / d), 0., 1.) * 12.0;
  col *= cos(v_Color);

  vec4 tex = texture2D(tex0, texCoord);
  gl_FragColor = vec4(tex.rgb * col.rgb, uv);
}
