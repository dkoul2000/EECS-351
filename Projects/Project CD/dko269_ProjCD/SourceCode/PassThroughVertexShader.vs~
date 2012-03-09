//help from online http://www.opengl.org/sdk/docs/tutorials1/ClockworkCoders/lighting.php for Phong lighting model
//help from Yungmann starter code that was posted to Blackboard for geometric distortions
//help from Khalid Aziz to get the distortions

varying vec3 N;
varying vec3 v;
uniform float time;
uniform float shadeX, shadeZ;

void main(void)  
{     
   N = normalize(gl_NormalMatrix * gl_Normal);
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       

   float shade = 1.0 + 0.15*sin(shadeX*time)*sin(shadeZ*time);	
   vec4 vPosition = vec4(gl_Vertex);
   vPosition.z = 0.15*sin(shadeX*time + 20.0*vPosition.x) + vPosition.z;
   vPosition.x = vPosition.x * sin(shadeX);

   gl_Position = gl_ModelViewProjectionMatrix * vPosition;
}
