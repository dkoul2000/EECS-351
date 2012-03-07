//help from online: http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php

#ifdef GL_ES
    precision mediump float;
#endif

varying vec3 N;
varying vec3 v;

void main (void)  
{  
   //using last light source
   vec3 L = normalize(gl_LightSource[7].position.xyz - v);
   
   //or you could normalize each negative
   vec3 R = normalize(reflect(L,N));
   vec3 E = normalize(v);

   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   vec4 Iamb = gl_FrontLightProduct[0].ambient;

   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.75*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 

   gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;     
}
