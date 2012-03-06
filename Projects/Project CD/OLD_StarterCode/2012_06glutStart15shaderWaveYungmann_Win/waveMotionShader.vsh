// Thanks to student Chris Yungmann and his group
// who succeeded in their Tues 2/21 shader exercise.
// I've done a little 'cleanup' here, but this is their work.

uniform float time;
uniform float xs, zs;

void main()
{
    float s;
    s = 1.0 + 0.3*sin(xs*time)*sin(zs*time);
// CAUSES  COMPILE ERROR: '(cannot modify an attribute)'
//  gl_Vertex.z = s*gl_Vertex.z;
//
// NICE TRICK from http://zach.in.tu-clausthal.de/teaching/cg_literatur/glsl_tutorial/#flatten
//  Declare a local variable, copy glVertex to it, modify the copy.
//
    vec4 vpos = vec4(gl_Vertex);
    vpos.z = 0.2*sin(xs*time + 20.0*vpos.x) + vpos.z;
    gl_Position = gl_ModelViewProjectionMatrix*vpos;
    gl_FrontColor = gl_Color;
}
