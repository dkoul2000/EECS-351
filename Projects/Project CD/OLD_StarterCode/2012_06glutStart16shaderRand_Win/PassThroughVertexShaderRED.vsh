const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);

void main()
{  // This is a comment
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
    //gl_FrontColor = gl_Color;
    gl_FrontColor = red;
}
