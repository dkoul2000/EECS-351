//help from online http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php for Phong lighting model
//help from Yungmann starter code that was posted to Blackboard for geometric distortions

varying vec3 N;
varying vec3 v;
uniform float time;
uniform float xs, zs;

void main(void)  
{     
   N = normalize(gl_NormalMatrix * gl_Normal);
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   float s = 1.0 + 0.15*sin(xs*time)*sin(zs*time);	
   vec4 vpos = vec4(gl_Vertex);
   vpos.z = 0.15*sin(xs*time + 20.0*vpos.x) + vpos.z;
   gl_Position = gl_ModelViewProjectionMatrix * vpos;
}
