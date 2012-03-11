#ifndef __lampMatl_h__      // ensures this module gets included only once
#define __lampMatl_h__      // during the build process.

//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
//
// Classes for openGL light sources (CLights) and materials (CMatl) useful for
// its built-in Phong lighting and materials.
//
// 2010.07.05 -- J. Tumblin created,tested in 'shadeTest05.4_matl' starter code.

// multi-platform includes for the GLUT/OpenGL libraries:
//------------------------------------------------------------------------------
#if defined(_WIN32)
#include <GL/freeglut.h>// open-source GLUT functions for windows, keybd, mouse
/*---*OR*--- use:
// #include <GL/glut.h> // for 100% backwards-compatibility with Mark Kilgard's
                        // SGI-copyrighted GLUT library. Careful! this excludes
                        // freeglut's callbacks for mouse wheel, etc.
*/
/*
// freeglut.h already contains these other includes that were necessary for
// using Kilgard's original GLUT library:
#include <GL/GL.h>      // OpenGL 3D graphics rendering functions
#include <GL/Glu.h>     // OpenGL utilties functions
#include <GL/glext.h>   // #defines for all OpenGL features on your machine.
*/
#elif defined(__APPLE__)
#include <GLUT/glut.h>  // GLUT functions for windows, keybd, mouse
#include <OpenGL/GL.h>  // OpenGL 3D graphics rendering functions
#include <OpenGL/Glu.h> // OpenGL utilties functions
#include <OpenGL/GLext.h>   // #defines for all OpenGL features on your machine.
// UNIX, Linux of all flavors;
#else
#include <unistd.h>         // defines symbolic constants, POSIX versions, etc.
#include <GL/glew.h>        // OpenGL 'extensions wrangler' for 'mesa' lib usage
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

//------------------------------------------------------------------------------
using namespace std;            // req'd for new <iostream> vs old <iostream.h>
#include <iostream>             // req'd for cin,cout use in lampMatl_01.cpp
#include "my_vecmat.h"          // home-made vector/matrix class.

// Pre-defined lamp-type values for use in CLight::createLamp()
#define LAMP_POINT          0   // user-spec'd point-src light (Phong lighting)
// three commonly-used point-light sources
#define LAMP_POINT_KEY      1   // point-source lamp infinitely far overhead
#define LAMP_POINT_L_FILL   2
#define LAMP_POINT_R_FILL   3
// advanced openGL light-source types (also useful for ray-tracing)
#define LAMP_SPOT           4   // spot-light with exponential falloff.
#define LAMP_TEXTURE        5   // emits light whose color varies with direction
                                // according to a texture map image.
#define LAMP_ENVIR_MAP      6   // Environment Map; any ray that hits no surface
                                // will get color that depends on direction,
                                // taken from a 'box-cube' image. Good for
                                // chrome and glass.  As a start on global
                                // illumination, consider how you might replace
                                // 'ambient' illumination with an average of the
                                // visible parts of the environment map...

class CLight
//==============================================================================
// Declare a class describe and apply a single light-source object for use
// in openGL.  Default position: openGL's origin (0,0,0,1) in the coordinate
// system in use when we call CLight::applyLamp(). Change I_pos for others.
// ( uses my_vecmat.h, my_vecmat.cpp; we'll see them again in ray-tracing)
{
    public:
    Vec4 I_pos, I_dir;          // 3D position & (if needed) orientation of the
                                // light source, defined in the openGL coord.
                                // system where we call CLight::applyLamp().

                                // Phong lighting Illumination parameters:
    Vec4 I_ambi, I_diff, I_spec;// Ambient, diffuse, and specular illumination
                                // colors.  (NOTE! openGL's glLight*() commands
                                // don't accept GLdouble! we'll cast these...)
    int lampID;                 // OpenGL light-source number for this object;
                                // set its value to GL_LIGHT0 or GL_LIGHT1 or
                                // any openGL value--they stop at GL_LIGHT7.
public:
    ~CLight(void);              // default destructor
     CLight(void);              // default constructor

    void createLamp(int myLampType, GLenum myLampID);
                                // Use lampType to select one of our pre-defined
                                // kinds of openGL lights; sets all parameters
                                // and sets this object's lampID to myLampID.
                                // Example:
                                // createLamp(LAMP_WHITE_KEY, GL_LIGHT3);
    void applyLamp(void);       // Apply contents of this CLight object to the
                                // openGL light source spec'd by CLight::lampID;
                                // (whose value is GL_LIGHT0....GL_LIGHT7).
    void removeLamp(void);      // Disable this light, but don't change any of
                                // its parameters. applyLamp() re-enables lamp.
};

// Material type values for CMatl::matlType; selects among a set of pre-defined
// Phong Lighting materials (values from pg. 51, "Advanced Graphics Programming"
// Tom McReynolds, David Blythe Morgan-Kaufmann Publishers (c)2005).
#define MATL_CUSTOM         0
#define MATL_RED_PLASTIC    1
#define MATL_GRN_PLASTIC    2
#define MATL_BLU_PLASTIC    3
#define MATL_BLACK_PLASTIC  4
#define MATL_BLACK_RUBBER   5
#define MATL_BRASS          6
#define MATL_BRONZE_DULL    7
#define MATL_BRONZE_SHINY   8
#define MATL_CHROME         9
#define MATL_COPPER_DULL    10
#define MATL_COPPER_SHINY   11
#define MATL_GOLD_DULL      12
#define MATL_GOLD_SHINY     13
#define MATL_PEWTER         14
#define MATL_SILVER_DULL    15
#define MATL_SILVER_SHINY   16
#define MATL_EMERALD        17
#define MATL_JADE           18
#define MATL_OBSIDIAN       19
#define MATL_PEARL          20
#define MATL_RUBY           21
#define MATL_TURQUOISE      22
#define MATL_MAX            23      // end-of-list indicator

#define MATL_STRMAX         80      // max. string length for a material name in
                                    // CMatl::matlName and CMatl::showName().

class CMatl
//==============================================================================
// Declare a class to describe and apply a single material
// WARNING! OpenGL accepts only floats (no doubles) for materials descriptors
{
    public:
    // Phong lighting reflectance parameters:
    Vec4 K_emit, K_ambi,K_diff, K_spec;// Emissive light from surface, ambient
                                // diffuse, specular; stored as r,g,b,a.
    GLfloat K_shiny;            // shinyness exponent for specular reflectance
    // Shading method:
    bool isFaceted;             // Used by CMatl::applyMatl() to select between
                                // GL_SMOOTH and GL_FLAT shading. If TRUE, uses
                                // glShadeModel(GL_SMOOTH).
    bool isTwoSided;            // FALSE for one-sided polys; applyMatl() uses
                                // this to select between GL_FRONT_AND_BACK and
                                // GL_FRONT in its glMaterial() calls.  If your
                                // object vanishes when isTwoSided==FALSE, your
                                // surface normals are probably reversed; see
                                // glFrontFace() function.

    int matlType;               // ID number for the kind of material: =0 for
                                // custom types; =1,2,3,... for pre-defined.
    char matlName[MATL_STRMAX+1];// ASCII text(up to 80 chars + NULL terminator)
                                // that describes this material; used by the
                                // 'showName()' and 'createMatl()' member fcns.
    public:
    ~CMatl(void);              // default destructor
     CMatl(void);              // default constructor

    void createMatl(int nuMatl);
                                // Set matlType and all member vars to one of
                                // our pre-defined kinds of openGL material.
    void applyMatl(void);       // Apply contents of this CMatl object to the
                                // openGL object currently being rendered.
    void showName(void);        // Display an on-screen name (up to 80 chars)for
                                // the material, shown at position (0,0) in
                                // current openGL coord system.
};

#endif                  // __lampMatl_h__
