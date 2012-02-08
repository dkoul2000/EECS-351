//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/* (JT: I prefer to keep line lengths <=80 chars for easy printing/display).

    glutPak.h

    Holds main() and all openGL/GLU/freeGLUT initialization, control, and
    callback functions needed for this program's GUI operations.  These fcns
    respond to anything users do with the keyboard, mouse, on-screen display,
    window resizing, cover/uncover, timers, etc.

    OpenGL and freeGLUT use C instead of C++. To keep your programs Object-
    Oriented, try to keep 'glutPak' as clean and uncluttered as possible.
    Instead of adding lots of global variables and complicated GUI procedures
    in the glutPak files, make them member variables and member functions of
    the C++ 'CTheApp' class. The 'glutPak' can then remain almost the same for
    all graphics programs, with just one global variable: our one CTheApp object
    called 'myApp', and no modifications to 'glutPak' files except calls to
    myApp member functions and setting/reading values of myApp member variables.

    ***CONFUSED? LOST?***
            Go to these files and start reading: glutPak.cpp and .h

*=============================================================================*/

// multi-platform includes for the OpenGL/GLUT/freeGLUT libraries:
#if defined(_WIN32)
#include <GL/glew.h>    // OpenGL 'extensions wrangler' for easy access.
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
#include <GLUT/glut.h>      // GLUT functions for windows, keybd, mouse
#include <OpenGL/GL.h>      // OpenGL core 3D graphics rendering functions
#include <OpenGL/Glu.h>     // OpenGL utilties functions
#include <OpenGL/GLext.h>   // OpenGL extensions (GLSL shading language,etc)
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
using namespace std;            // Simple-as-possible C++ usage.

#include <cstdlib>          // for exit() function
#include <cstdio>           // C++ for fscanf(), sprintf(), etc.
#include <stdint.h>         // C integer size definitions (uint32_t, etc)
                            // (C++ version not released/ready yet)
#include <iostream>         // C++ streams support;
#include <fstream>          // C++ file I/O streams
#include <string>           // C++ string-editing class; nice for filenames, etc
#include <cmath>            // C++ math: for M_PI and trig functions

//==============================================================================
// My own classes:
#include "CByteImg.h"       // My own openGL-compatible class; bytes for R,G,B.
//====================
//
// LITERALS:
//
//====================
#define NU_PAUSE 10 // default wait time (in milliseconds (mS)) we use in GLUT's
                    // timer callback function between each screen redraw.
                    // Most PCs refresh their screens every 16.6mS (60Hz); we
                    // do nothing for NU_PAUSE mS to let the operating system
                    // work on other tasks; if we finish our re-drawing in
                    // the remaining time, we get smooth animation on-screen.
                    // If animation looks jumpy/irregular/jittery,
                    // shorten NU_PAUSE.
    // NOTE!!  Longer NU_PAUSE values (e.g. 300mS) slow the animation;
    // use long values to see your animation step-by-step.


/*------------------------------------------------------------------------------
C-style function prototypes for GLUT callbacks.

A 'callback' is a function that YOU write, but YOU never call; instead,
something else calls it, such as another program, the operating system, or
other already-written functions.

The GLUT library uses callbacks.  GLUT provides nearly all OSs(Windows XP,Vista,
Win7; Apple MacOS, Unix, Solaris, Linux...) with the same simple set of
callbacks and other functions for GUI interactions with openGL drawings.  These
functions get called to respond to GUI events--when the user or OS decides to
open/close/move/resize or redraw a display window or respond to a keyboard,
mouse, timer, or menu action for your program. GLUT creates windows and menus
appropriate for you OS, with a frame, title bar, buttons, widgets and more.

For example, when the OS needs this program to re-draw its window contents,
GLUT calls the 'myDisplay()' function we registered in main() as the display
callback. When you press keys on the keyboard, GLUT calls the 'myKeyboard()'
or 'mySpecialKey()' functions we registered in main(). When you click or move
the mouse, GLUT calls the registered myMouseClik() or myMouseMove() functions.
--Note that GLUT uses function pointers to 'register' the name of your
    callback function; you must ensure your function has the exactly correct
    argument list and return type expected by GLUT for that callback function.
--I put callback registration in main() for simplicity. You may register,
    re-register or  'un-register' any callback at any place in your code to
    make it available to GLUT when you want it available (e.g. many programs
    selectively register or un-register the 'idle' callback). To 'un-register',
    just use NULL instead of a callback function name, e.g.
        glutMouseFunc(NULL);
--GLUT/freeGLUT offers more callbacks, too; search glut.h to find pop-up menus,
    joysticks, 'spaceball' (google with '3Dconnexion'), extra buttons and more.
--GLUT is copyrighted and unchanged since 2002; visit 'SourceForge' to find
    'freeGLUT', a free, open-source, actively-maintained work-alike. It includes
    callbacks for mouse-wheel, etc.  When possible, use freeGLUT instead.

------------------------------------------------------------------------------*/
// CALLBACK FUNCTION PROTOTYPES: called by GLUT/freeGLUT...
void myReshape( int nuWidth, int nuHeight );
                            // ...when user opens or re-sizes display window.
void myDisplay(void);		// ...when screen re-draw needed for any reason.
void myKeyboard(unsigned char key, int xw, int yw);
							// ...when users press any alphabetic
							// or numerical 'key' on keyboard, and gives current
							// mouse position xw,yw in window-system pixels.
                            // (most window systems put origin at UPPER left).
void myKeySpecial(int key, int xw, int yw);
							// ...when users press 'arrow' keys, 'Fcn' keys, or
							// any other non-alphanumeric keys; xw,yw values
							// gives mouse position in window-system pixels.
                            //  (most window systems put origin at UPPER left).
							// Search glut.h for #define literals for special
							// keys such as GLUT_KEY_F1, GLUT_KEY_UP, etc.
void myMouseClik(int buttonID, int upDown, int xw, int yw);
                            // ...when user pressed or released a mouse button.
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
// 1) stop any screen re-drawing when our display window isn't visible;
// 2) ensure this program continually re-draws the screen when running;
// 3) impose a delay of at least NU_PAUSE milliseconds between each re-draw,
//      to  a) let the operating system work on tasks other than our program,
//      and b) avoid revising screen contents more often than it gets displayed
//                  (typically every 16.6mSec -- 60Hz refresh).

void runAnim(int isOn);     //  --to STOP/PAUSE animation,call runAnimTimer(0);
                            //  --to RESUME animation,    call runAnimTimer(1);
void myTimer(int value);    // Timer callback by runAnim() to control animation
void myHidden(int isVisible);// Callback fcn called by GLUT whenever display
                            // gets covered (visibility==0) or uncovered (==1)



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
// together into a single class 'CtheApp'.  Just above main(), I declare just
// one global variable of type 'CtheApp' named 'myApp', and use it as our sole
// container for the program's shared state.
{
public:
char winTitle[128];                             // Window title text buffer
char **argvCopy;                                // copy of argv; lets us find
                                                // cmd-line args, directory,etc.
                                                // Assign it in main like this:
                                                // myApp.argvCopy = argv;
GLint display_width, display_height;            // display window size in pixels
GLint display_xpos,  display_ypos;              // position (in WINDOW coords--
                                                // expect origin at upper LEFT!)
int doClear;                                    // ==1 clear screen on re-draw
                                                // ==0 don't clear screen.
int KBflags;                                    // Keyboard 'Modifier' flags for
                                                // CTRL, ALT, SHIFT (see mouse,
                                                // & keyboard callback fcns.)
GLdouble clrColr4dv[4];                         // Screen-clearing color (RGBA)
GLdouble penColr4dv[4];                         // Shape-drawing-pen color(RGBA)
GLdouble txtColr4dv[4];                         // Text-drawing color (RGBA)
GLdouble errColr4dv[4];                         // Error-reporting color (RGBA)

GLint doAnim;                                   // ==1 to run animation,
                                                // ==0 to pause.
GLint animFrameTime;                            // waiting-time in milliseconds
                                                // between each animation frame

// Displayed shape:
int shapeNum;                                   // Selection# for GLUT shapes
GLdouble shape_xpos,shape_ypos,shape_zpos;      // shape's current position
                                                // (arrow keys, pg up/dn keys)
GLdouble shape_deg;                             // rotation angle for animation
GLdouble shape_degStep;                         // rotation rate
#define JT_IMG_MAX 32
CByteImg  myImg[JT_IMG_MAX];                    // (array of) images from file
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
void drawText3d( GLdouble x0, GLdouble y0, GLdouble z0,
                const char *pString, int fontNum=0);
                                    // draw pString text on-screen at x0,y0,z0
                                    // using current coord. system & pen color.
                                    // (fontNum; choose [0-4] inclusive).
void drawAxes(void);				// draw red x-axis, green yaxis, blue zaxis
                                    // for the current coordinate system.
void drawShape(void);               // Use 'penColr[]' to draw the GLUT shape
                                    // selected by 'shapeNum' at (xpos,ypos)
// OpenGL pixel-buffer modifing fcns:
void printPixelStoreReport(void);   // Print out all openGL state-machine values
                                    // that control how it packs/upacks pixels
                                    // for read/write to client memory.
void reportKeyModifiers(const char *pName);
                                    // text and graphical display of any
                                    // keyboard modifiers (SHIFT, CTRL, ALT).
                                    // Used by keyboarde & mouse callback fcns.
};
