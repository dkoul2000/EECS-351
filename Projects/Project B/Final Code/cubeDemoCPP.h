#include "CByteImg.h"
#include <stdlib.h>
#include <time.h>


//===============  OpenGL and GLUT on *ANY* CodeBlocks platform  ===============
//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (J. Tumblin: I like consistent 80-char column width)
//
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
#include <iostream>       // for cin, cout, etc.
#include <math.h>         // for sin, cos.

#define NU_PAUSE 10 // wait time (in milliseconds (mS)) that we use for GLUT's
                    // timer callback function between each screen redraw.
                    // Most PCs refresh their screens every 16.6mS (60Hz); we
                    // do nothing for NU_PAUSE mS to let the operating system
                    // work on other tasks; if we finish our re-drawing in
                    // the remaining time, we get smooth animation on-screen.
                    // If animation looks jumpy/irregular/jittery,
                    // shorten NU_PAUSE.
                    // Longer NU_PAUSE values (e.g. 300mS) slow the animation;
                    // use long values to see your animation step-by-step.

// font strings for drawText2D();
// (use as value for pFont argument)
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

/*------------------------------------------------------------------------------
C-style function prototypes for GLUT callbacks.

A 'callback' is a function that YOU write, but YOU never call; instead,
something else calls it, such as another program, the operating system, or
other already-written functions.

The GLUT library uses callbacks.  GLUT provides nearly all OSs(Windows XP,Vista,
Win7; Apple MacOS, Unix, Solaris, Linux...) with the same simple set of
callbacks and other functions for GUI interactions with openGL drawings.  These
functions open/close/move/resize and draw within a display window, and call
back when users cause a keyboard, mouse, timer, or menu event. They create
windows appropriate for you OS, with a frame, title bar, buttons, widgets and
drag-able corners for resizing.

For example, when the OS needs this program to re-draw its window contents,
GLUT calls the 'myDisplay()' function it registered in main() as the display
callback. When you press keys on the keyboard, GLUT calls the 'myKeyboard()'
or 'mySpecialKey()' functions we registered in main(). When you click or move
the mouse, GLUT calls the registered myMouseClik() or myMouseMove() functions.
--Note that GLUT uses function pointers to 'register' the name of your
    callback function; you must ensure your function has the exactly correct
    argument list and return type expected by GLUT for the callback.
--All callback registration is in main(); you may re-register or 'un-register'
    any callback at any time (e.g. the 'idle' callback). To 'un-register', use
    NULL instead of a callback function name, e.g.    glutMouseFunc(NULL);
--GLUT offers more callbacks, too; search glut.h to find pop-up menus,
    joysticks, buttons and more.
--GLUT is proprietary and unchanged since 2002; visit 'SourceForge' to find
'freeGLUT', a free, open-source, actively-maintained work-alike that includes
callbacks for mouse-wheel, etc.  Future classes will use freeGLUT.

------------------------------------------------------------------------------*/
void myReshape( int width, int height );
                            // Called by GLUT when user opens or re-sizes the
                            // display window.
void myDisplay(void);		// Called by GLUT when screen re-draw needed;
void myKeyboard(unsigned char key, int xw, int yw);
							// Called by GLUT when users press any alphabetic
							// or numerical 'key' on keyboard, and gives
							// current mouse position xw,yw in window pixels.
                            //  (most window systems put origin at UPPER left).
void myKeySpecial(int key, int xw, int yw);
							// Called by GLUT when users press 'arrow' keys,
							// 'Fcn' keys, or other non-alphanumeric keys;
							// xw,yw value gives mouse position (window pixels).
                            //  (most window systems put origin at UPPER left).
							// Search glut.h for #define literals for special
							// keys such as GLUT_KEY_F1, GLUT_KEY_UP, etc.
void myMouseClik(int buttonID, int upDown, int xw, int yw);
                            // Called by GLUT on mouse button click or un-click
                            // When user clicks a mouse button,
                            //  buttonID== 0 for left mouse button,
                            //		   (== 1 for middle mouse button?)
                            //			== 2 for right mouse button;
                            //	upDown  == 0 if mouse button was pressed down,
                            //	        == 1 if mouse button released.
                            //	xw,yw == mouse position in window-system pixels.
                            //  (most window systems put origin at UPPER left).
void myMouseMove(int xw, int yw);
							// Called by GLUT when user moves mouse while
							// while pressing one or more mouse buttons, giving
							// current mouse position xw,yw in window pixels.
                            //  (most window systems put origin at UPPER left).

// runAnimTimer(), myTimer), and myHidden() functions work together to:
// 1) ensure this program continually re-draws the screen when running;
// 2) impose a delay of at least NU_PAUSE milliseconds between each re-draw,
//      to  a) let the operating system work on tasks other than our program,
//      and b) avoid revising screen contents more often than it gets displayed
//                  (typically every 16.6mSec -- 60Hz refresh).
// 3) stop any screen re-drawing when our display window isn't visible.
void runAnimTimer(int isOn);//  --to STOP/PAUSE animation,call runAnimTimer(0);
                            //  --to RESUME animation,    call runAnimTimer(1);
void myTimer(int value);    // Used by runAnimTimer() to control animation
void myHidden(int isVisible);   // Called by GLUT each time the display
                            // gets covered (visibility==0) or uncovered (==1)

//===============================================================================
// C-style function prototypes; better to use C++ classes for drawing.
// HINTS:
// --(Creating prototypes here allows us to arrange the functions in any order;
// without them you must put 'main()' at the end of the glutStart.c file.)
// --(Please organize your own functions into C++ classes, not C as done here.)

// Read the current size and position of the graphics display window
int getDisplayHeight(void);
int getDisplayWidth(void);
int getDisplayXpos(void);
int getDisplayYpos(void);


//==============================================================================
#define NUM_CUBE_FACES 6
#define NUM_VERTICES_PER_FACE 4
#define NUM_VERTICES 8

class Ccube
{
// simple arrays; OK because ALL cubes have these.  For more general
// shape-describing classes, better to use dynamic allocation...
float facecolor[NUM_CUBE_FACES][3];					// RGB values
int face[NUM_CUBE_FACES][NUM_VERTICES_PER_FACE];
float vertex[NUM_VERTICES][3];

public:
	Ccube();				// default constructor,
	~Ccube();				// default destructor

	void makecube(void);			// create a unit cube.
	void drawSolid(void);           // draw cube as a solid object.
	void drawWireframe(void);       // draw cube as a wireframe object.
	void drawPoints(int diam);      // draw cube as isolated vertices of
									// a given diameter (as used by OpenGL)

};

//=============================================================================
// Handy drawing functions
void drawText2D(void *pFont, double x0, double y0, const char *pString);
                                    // draw text on-screen at x0,y0.
                                    // (See #defines for pFont values)
void drawAxes(void);				// draw red x-axis, green yaxis, blue zaxis
void drawSquarePoints(void);		// Draw square vertices: x,y = +/-1,+/-1
void drawSquareEdges(void);			// draw that square's edges;
void drawSquareFace(void);			// draw that square's solid-colored face;
void doCamChoice(void);             // change to openGL camera specified by
                                    // global variable 'camChoice'
void drawScene();
void drawPlane(GLdouble, GLdouble);
void askForHelp(void);
void printInstructions(void);

//HELP FROM KHALID AZIZ

class cube {
    public:
        void draw();
        cube(int height);
        float colorsArray[24];
        unsigned int indexes[16];
        int vertices[24];
};

class squarePrism {
    public:
        void draw();
        squarePrism(int height);
        float colorsArray[18];
        unsigned int indexes[13];
        int vertices[18];
};

class triangularPrism {
    public:
        void draw();
        triangularPrism(int height);
        float colorsArray[15];
        unsigned int indexes[11];
        int vertices[15];
};

//////PROFESSOR TUMBLIN'S STARTER CODE
class CTheApp
//==============================================================================
// CTheApp class holds all the program's globally-accessible state information.
// We declare only one instance, the global variable 'myApp', just above main().
//
// GLUT/freeGLUT *forces* us to use global variables to share state info between
// different callback functions: with global variables, the keyboard callback
// function can change what the display callback function draws, even though
// GLUT pre-specifies these functions' arguments and return values.  Instead of
// scattering many global variables throughout the program, I gather all of them
// together into a single class 'CTheApp'.  Just above main(), I declare just
// one global variable of type 'CTheApp' named 'myApp', and use it as our sole
// container for the program's shared state.
{
public:
char **argvCopy;                                // copy of argv; lets us find

GLint doAnim;                                   // ==1 to run animation,
                                                // ==0 to pause.
GLint animFrameTime;                            // waiting-time in milliseconds
                                                // between each animation frame

#define JT_IMG_MAX 32
CByteImg myImg[JT_IMG_MAX];                    // (array of) images from file
GLfloat imgXpos[JT_IMG_MAX];                    // image position (glRasterPos()
GLfloat imgYpos[JT_IMG_MAX];
GLfloat imgZpos[JT_IMG_MAX];
int imgMax;                                     // # valid images we hold.
int imgCurrent;                                 // 'current' image selected.
#undef JT_IMG_MAX

//----------------------------Member Functions
CTheApp(void);                              // default constructor, destructor
~CTheApp(void);

public:
};
