//help from online http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php for Phong lighting model
//help from Yungmann starter code that was posted to Blackboard for geometric distortions

varying vec3 N;
varying vec3 v;
uniform float time;
uniform float xs, zs;

void main(void)  
{     
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   float s;
   s = 1.0 + 0.2*sin(zs*time)*sin(xs*time);	

   vec4 vpos = vec4(gl_Vertex);
   vpos.z = 0.2*sin(xs*time + 25.0*vpos.x) + vpos.z;
   gl_Position = gl_ModelViewProjectionMatrix * vpos;
   gl_FrontColor = gl_Color;
}
