#ifndef __shadeTest_h__      // ensures this module gets included only once
#define __shadeTest_h__      // during the build process.

//===============  OpenGL and GLUT on *ANY* CodeBlocks platform  ===============
//
// How do I set up CodeBlocks, openGL and GLUT on my machine?
//	See:	2011.01.18-CodeBlocksGLUT-installNotes.txt
//		(in this directory

// multi-platform includes for the GLUT/OpenGL libraries:
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

//==============================================================================
using namespace std;        // req'd for modern <iostream> vs old <iostream.h>
#include <iostream>

#include "nu_progShader.h"          // Classes for GLSL programmable-shaders.
#include "lampMatl_01.h"    // CLights and CMatl classes for openGL's Phong
                            // materials and lighting.

//====================
//
// Globally Defined LITERALS:
//
//====================

#define NU_TITLE	"EECS351 Shade Starter"	// Display window's title bar:
#define NU_WIDTH	480						// window size in pixels
#define NU_HEIGHT	480
#define NU_XPOS		  0						// initial window position
#define NU_YPOS		256
#define NU_ZNEAR	0.1						// near, far clipping planes for
#define NU_ZFAR		50.0					// a 3D camera.

#define NU_SAVEFILENAME "screenShot.bmp"	// default save-the-display filename
#define NU_TEXFILENAME  "sunsetImage.bmp"	// default texture image filename.

// font strings for drawText2D();
// (use as value for pFont argument)
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

//====================
//
//	C Function Prototypes  (these belong in a '.h' file if we have one)
//
//====================

void my_glutSetup(int *argc, char **argv);	// GLUT initialization

					// GLUT callback functions. Called when:
void display(void);						// GLUT says re-draw the display window.
void reshape(int w, int h);				// GLUT says window was re-sized to w,h
void keyboard(unsigned char key, int x, int y);	//GLUT says user pressed a key
void keySpecial(int key, int x, int y);	// GLUT says user pressed a 'special'key
void mouseMove(int xpos,int ypos);		// GLUT says user moved the mouse to..
void mouseClik(int,int,int,int);		// GLUT says user click/dragged mouse to
//void idle(void);

void drawAxes(int colr);                // Draw +x,+y,+z coordinate axes:
                                        // if colr==0, draw red,green,blue,
                                        // else draw cyan, magenta,yellow.
void drawText3D(void * pFont, double x0, double y0, const char* pString);
                                        // Write text 'pString' on-screen at
                                        // position x0,y0 using current color
                                        // and coord system. Example:
                                        // drawText3D(helv18, 0.3,0.4,"Hello!");
void drawText2D(void *pFont, double x0, double y0, const char* pString);
                                        // Write text 'pString' on-screen in
                                        // white on top of any other contents in
                                        // the CVV; be sure x0,y0 fit into +/-1

void makePyramid(void);          // create (global) arrays of verts,norms,colors
void drawPyramid(void);          // used by 'drawPyramid' to draw a 3D shape.

class CTransRot
//==============================================================================
// Declare a new class that records/accumulates translate and rotate amounts,
// usually adjusted by mouse & keyboard inputs.  I use an instance of this class
// for each user-adjustable joint in a jointed object.Its 'applyMatrix()' member
// then makes openGL calls to apply these translate/rotate amounts to the openGL
// matrix currently selected(e.g. glMatrixMode(GL_PROJECTION or GL_MODELVIEW)).
{
public:
double	x_pos, y_pos, z_pos;	// cumulative position offset
double	x_rot, y_rot, z_rot;	// cumulative rotation on x,y,z axes
int		isDragging;				// TRUE if user is holding down the mouse button
								// that affects our value(s); else FALSE.
int m_x,m_y;					// last mouse-dragging position.

~CTransRot(void);				// default destructor
 CTransRot(void);				// default constructor
void reset(void);				// reset everything to zero.
void apply_TR_Matrix(void);		// apply to openGL's current coordinate system;
								// --translate, then rotate to make a new one.
void apply_RT_Matrix(void);		// --rotate, then translate to make a new one.
};


//3D VERTEX ARRAY SHAPES - GOTTEN FROM MY PROJECT B CODE
class squarePrism {
    public:
        void draw();
        squarePrism(int height);
        float colorsArray[18];
        unsigned int indexes[13];
        int vertices[18];
};

#endif  // __shadeTest_h__
