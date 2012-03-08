//help from online: http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php for Phong lighting model
//help from Yungmann starter code that was posted to Blackboard for geometric distortions

#ifdef GL_ES
    precision mediump float;
#endif

varying vec3 N;
varying vec3 v;

void main (void)  
{  
   //using light source
   vec3 L = normalize(gl_LightSource[7].position.xyz - v);
   
   vec3 R = normalize(reflect(L,N));
   vec3 E = normalize(v);

   //ambient, diffuse, specular Phong characteristics
   vec4 vAmbient = gl_FrontLightProduct[0].ambient;

   vec4 vDiffuse = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   vDiffuse = clamp(vDiffuse, 0.0, 1.0);

   vec4 vSpecular = gl_FrontLightProduct[0].specular*pow(max(dot(R,E),0.0),0.75*gl_FrontMaterial.shininess);
   vSpecular = clamp(vSpecular, 0.0, 1.0); 
	
   vec4 colorAddOn = vAmbient + vDiffuse + vSpecular;

   gl_FragColor = gl_FrontLightModelProduct.sceneColor + colorAddOn;     
}
