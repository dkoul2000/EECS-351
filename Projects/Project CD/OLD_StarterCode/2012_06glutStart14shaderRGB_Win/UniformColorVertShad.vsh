// -- UniformColorFragShad.vsh --
// set color from uniform vars r_shad,g_shad,b_shad.
uniform float r_shad;
uniform float g_shad;
uniform float b_shad;

void main()
{
vec4 myColor = vec4(r_shad, g_shad, b_shad, 1.0);
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
    //gl_FrontColor = gl_Color;
    gl_FrontColor = myColor;
}
