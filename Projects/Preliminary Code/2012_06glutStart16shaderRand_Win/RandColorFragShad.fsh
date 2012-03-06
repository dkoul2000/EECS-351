// VERY COOL GLSL noise-generator from:
//          http://www.ozone3d.net/blogs/lab/20110427/glsl-random-generator/
//
// NOTE: be sure to choose 'solid' display;
// fragment shader doesn't do much for lines or points...

varying vec3 v;
float rand(vec2 n)
{
  return 0.5 + 0.5 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main(void)
{
  float x = rand(v.xz);     // demo of 'swizzling'. Choose x,z from 'varying'
  gl_FragColor = vec4(x, x, x, 1.0);
}
