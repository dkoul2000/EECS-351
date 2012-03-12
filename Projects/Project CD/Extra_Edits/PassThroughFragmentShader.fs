//help from online: http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php for Phong lighting model
//help from Yungmann starter code that was posted to Blackboard for geometric distortions
//help from online: http://zach.in.tu-clausthal.de/teaching/cg_literatur/glsl_tutorial/ for distortions and shading
//help from Khalid Aziz and Cassie Rommel for time-varying geometric distortions
//help from Khalid Aziz to help with the light shading

#ifdef GL_ES
    precision mediump float;
#endif

#define TOTAL_LIGHTS 2
varying vec3 N;
varying vec3 v;
uniform float time;
uniform float shadeX;
uniform float shadeZ;

void main (void)  
{  
   vec4 colorAddOn = vec4(0,0,0,0);
   vec3 normalN = normalize(N);   

   for (int i = 0; i < TOTAL_LIGHTS; i++)
   {
   	vec3 L = normalize(gl_LightSource[i].position.xyz - v);
  	vec3 R = normalize(reflect(L,N));        
	vec3 E = normalize(v);

   	//implement Phong model characteristics:

   	//calculate ambient
   	vec4 vAmbient = gl_FrontLightProduct[i].ambient;

   	//calculate diffuse
   	vec4 vDiffuse = gl_FrontLightProduct[i].diffuse * max(dot(N,L), 0.0);
   	vDiffuse = clamp(vDiffuse, 0.0, 1.0);

   	//calculate specular
   	vec4 vSpecular = gl_FrontLightProduct[i].specular*pow(max(dot(E,R),0.0),0.75*gl_FrontMaterial.shininess);
   	vSpecular = clamp(vSpecular, 0.0, 1.0); 
	
   	//sum of characteristics and final color
   	colorAddOn = colorAddOn + vAmbient + vDiffuse + vSpecular;
   }
   
   if (time > 20.0)
   	gl_FragColor = gl_FrontLightModelProduct.sceneColor + colorAddOn + sin(shadeX);
   else
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + colorAddOn;

}
