varying vec3 v;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	v = vec3(gl_Position);
    gl_FrontColor = gl_Color;
}

