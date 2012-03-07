//help from online: http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php

#ifdef GL_ES
    precision mediump float;
#endif

//defined in VertexShader
varying vec3 N;
varying vec3 v;

void main (void)  
{  
   //using last light source
   vec3 L = normalize(gl_LightSource[7].position.xyz - v);
   
   //or you could normalize each negative
   vec3 R = normalize(reflect(L,N));
   vec3 E = normalize(v);

   //ambient, diffuse, specular Phong characteristics

   vec4 Iambient = gl_FrontLightProduct[0].ambient;

   vec4 Idiffuse = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiffuse = clamp(Idiffuse, 0.0, 1.0);

   vec4 Ispecular = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.75*gl_FrontMaterial.shininess);
   Ispecular = clamp(Ispecular, 0.0, 1.0); 

   gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iambient + Idiffuse + Ispecular;     
}
