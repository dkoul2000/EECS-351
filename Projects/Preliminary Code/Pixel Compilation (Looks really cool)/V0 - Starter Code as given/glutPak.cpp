//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/* (JT: I prefer to keep line lengths <=80 chars for easy printing/display).

	glutPak.cpp         **START READING HERE***

    Demo of GLUT API for operating system services and GUI callbacks for OpenGL.
    This file contains 'main()' and all the openGL callback functions we'll use.

2010.12.13-J. Tumblin: created for use in EECS351-1 at Northwestern University
2011.09.08-J. Tumblin: added openGL bitmap examples and image-file reading
2011.09.30-J. Tumblin: added 'PixelWorld' camera & viewport settings,

*=============================================================================*/

#include "glutPak.h"

//=====================
// GLOBAL VARIABLE(S): a bad idea, but forced by GLUT's fixed-arg callback fcns.
//                   Always use as FEW global vars as possible.  Best strategy
//                   I've found is to create a top-level class (e.g. CTheApp)
//                   that holds all the top-level data and functions for your
//                   program.  Then make just one object of this class to use as
//                   your program's one and only global variable (e.g. myApp).
//                   This 'top-level' class idea is sensible for ANY object-
//                   -oriented program, as it lets you make multiple instances
//                   of the entire application by just creating more objects of
//                   the top-level 'CTheApp' class.
//=====================
CTheApp myApp;              // Our one-and-only top-level object.  It holds all
                            // the app's state variables that would otherwise
                            // have to be declared as global variables.

#ifndef M_PI                // be sure pi exists
#define M_PI acos(-1.0)     // ugly, slow, but exactly accurate M_PI replacement
#endif

int main(int argc, char *argv[])
//=============================================================================
{
int i;

    //--Setup for GLUT/freeGLUT----------------------
    // BETTER IDEA: create your own member function of CTheApp class to hold
    // all this mess (perhaps 'myApp.glutSetup(argc, argv)' ? Make your own!)
	glutInit( &argc, argv );	    // init GLUT user-interface library;
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
                    // Set up all display-frame-buffers we may use;
					// Display buffer: single-buffering GLUT_SINGLE,
					// double-buffering GLUT_DOUBLE, where you'd need to call
					// glutSwapBuffers() after drawing operations to show
					// on-screen what you draw).
                    // full 32-bit/pixel RGBA (not RGB)color,(A =alpha=opacity),
					// enable depth testing for 3D drawing ('Z-buffering')
					// enable 'stencil' buffer (useful for masks, shadows, etc)
    //fGLglEnable(GL_DEPTH_TEST);    // Enable z-depth tests for all drawing.
                                // (SURPRISE! default setting is 'disable')
    glutInitWindowSize(myApp.display_width, myApp.display_height );
    glutInitWindowPosition(myApp.display_xpos, myApp.display_ypos );
                    // Initial display window size & position
	glutCreateWindow(myApp.winTitle); // set initial window title-bar name
	glClearColor(myApp.clrColr4dv[0],
                 myApp.clrColr4dv[1],
                 myApp.clrColr4dv[2],
                 myApp.clrColr4dv[3]);// set openGL's 'cleared-screen' color

	// Register all the GLUT callback functions this program uses:
	glutDisplayFunc( myDisplay );	// callback for display window redraw
	glutReshapeFunc( myReshape);    // callback to create or re-size window
	glutKeyboardFunc( myKeyboard);  // callback for alpha-numeric keystroke
	glutSpecialFunc ( myKeySpecial);// callback for all others: arrow keys,etc.
	glutMouseFunc( myMouseClik );   // callback for mouse button events
	glutMotionFunc( myMouseMove );  // callback for mouse dragging events
	glutVisibilityFunc(myHidden);   // callback for display window cover/uncover
	//-----------------------------------
    // File I/0 for text and image files:
    myApp.argvCopy = argv;          // make a copy of argv[] for later use by
                                    // functions that need our directory/path.
    myApp.imgMax = 3;               // set number of images to show on-screen
    myApp.imgCurrent = 0;           // currently-selected image#.
    for(i=0; i<myApp.imgMax; i++)
    {   // set initial positions for lower-left corner of each image
        myApp.imgXpos[i] = -0.99 + (double)i/(double)myApp.imgMax;
        myApp.imgYpos[i] = -0.99 + (double)i/(double)myApp.imgMax;
        myApp.imgZpos[i] = 0.0;     // (uses glRasterPos()...)
    }
    for(i=0; i<myApp.imgMax; i++)   // read in, prepare all images for display
    {
        myApp.myImg[i].setFileName(myApp.argvCopy, i); // i-th default filename
        myApp.myImg[i].readFile();
    }

	//-----------------------------------
    runAnim(1);                     // start our own little animation timer,
	glutMainLoop();	                // enter GLUT's event-handler; NEVER EXITS!
	return 0;                       // normal exit.
}

void myReshape( int nuWidth, int nuHeight )
//------------------------------------------------------------------------------
// Called by GLUT when user re-sizes the display window (e.g. dragged its
// corners). ALSO called by glutInitWindowSize() for window creation in main().
// Use this callback for openGL's glViewport() and to set the GL_PROJECTION
// matrix; the 'camera' that defines the 3D viewed volume it maps to the CVV.
{
    myApp.display_width    = nuWidth;    // save the new on-screen window size
    myApp.display_height   = nuHeight;   // reported by the callback arguments.

                                        // (Empty brackets to enclose long
                                        // comments and make them 'collapsible'
                                        // by 'folding' in the CodeBlocks editor).
    {   // long tutorial comment:
//==================ON_SCREEN_COORDINATES================
// Modern openGL programs use floating-point values to set (x,y,z) positions for
// all drawing primitives (points, lines, shapes, etc) that appear on-screen.
//
//      ****"What coordinates, what NUMBERS, will put something on-screen?"***
//              At first, this question is surprisingly tricky:
//
// SHORT ANSWER:
// ----------------
//  --By default, openGL does NO transformations(GL_MODELVIEW, GL_PROJECTION
//      set to identity matrix) fills the display screen with (x,y,z) values
//      between +1.0 and -1.0 and discards ('clips') the rest.
//      To see what you draw, keep all x,y,z position values between +/-1.0.
//
//  --This 3D cube-shaped range of xyz values, this 'Canonical View Volume'(CVV)
//      is fixed; no function or command can change it.
//
//  --The glViewport() function modifies how this 'CVV' fits into the on-screen
//      display window.  When the window shape changes, GLUT calls this, the
//      'reshape()' callback function, and here many programs call glViewport()
//      to change that transformation.
//
//  --We can't change the CVV, but we *CAN* change the values we feed into the
//      CVV.  When you issue an openGL drawing command, its vertices get
//      transformed first by the GL_MODELVIEW matrix, then by the GL_PROJECTION
//      matrix, and then clipped by the CVV (to discard values >1.0 or <-1.0).
//
//   --Most openGL programs use the reshape() callback function to change:
//         --the GL_MODELVIEW matrix (does the world-to-camera transformation);
//         --the GL_PROJECTION matrix (does the camera-to-CVV transformation);
//         --the viewport settings (does the CVV-to-screen transformation);
//      For example, you can stretch the CVV to fill the entire display window,
//      then change GL_PROJECTION to fill the CVV with a virtual 'camera' whose
//      field-of-view matches the display window, then change the GL_MODELVIEW
//      matrix to position that camera at a new 'eye-point' and aim it to look
//      in a new direction.
//
// LONG, DETAILED ANSWER:
//--------------------------
// OpenGL version 2.1 and earlier transforms all drawing primitives (vertices)
// twice before they arrive at 'the CVV' and get clipped to the fixed +/-1
// range. After this CVV clipping, the openGL 'viewport' transforms them once
// more to fit the contents of the CVV to the screen.  Specifically:
//
//  Before display, openGL transforms ALL vertex locations (x,y,z):
//  1)--first by the GL_MODELVIEW matrix, used for two purposes:
//          a)-- build,scale and place 3D jointed shapes (the 'MODEL' part),and
//          b)-- position and aim your camera (the 'VIEW' part).
//      In this program we won't bother with a), but you can use GL_MODELVIEW
//      matrices in a stack to make jointed objects on-screen, such as a robot
//      arm.  For b), we define a 'world' coordinate system, and within it
//      define a 'camera' coordinate system to position and aim the camera.
//      The  camera coord system's origin defines the camera's center-of-
//      projection or 'viewpoint'; the camera coord. system's -z axis (yes,
//      it's the NEGATIVE z, not +z!) defines the camera's aiming direction
//      (the direction of the center-most pixel), and the x,y axes define
//      the x,y axes of the image made by the camera. The OpenGL' 'gluLookAt()'
//      function makes 'camera-placement' easy: just select GL_MODELVIEW
//      matrix, clear out its old contents, and call gluLookAt() to position
//      and aim your camera. Many openGL programs do this at the start of
//      their 'myDisplay()' callbacks.
//  2)--second by the GL_PROJECTION matrix used to:
//          c)-- apply foreshortening, perspective, and field-of-view effects
//              on drawing primitives, mapping our camera's pyramid-like view
//              frustum (the volume of 3D space our camera can see) to fit
//              into the +/-1 range the CVV accepts: the rest gets clipped.
//
//  More specifically, the GL_PROJECTION matrix defines the 'camera'
//      transformations that let us make a 2D picture from 3D objects.
//      Its lens-like transformations apply 3D perspective and a pseudo-depth
//      transform to convert the coordinates of drawing primitives (points,
//      lines, triangles, etc)
//      --Fom the 'VIEW' or 'camera' coordinate system, which has the camera
//          center-of-projection or 'eyepoint' at origin, and has the camera
//          look down the -z axis, yes thats right, NEGATIVE z axis!)
//      --TO the CVV's coordinate system (clips all values outside +/- 1).
//  Details: The GL_PROJECTION matrix values we choose determine the 'camera'
//      openGL will use to convert a 3D volume to a 2D image. We keep these
//      choices simple by specifying a camera's 'viewing frustum', a 6-sided
//      truncated 3D pyramid that encloses the 3D volume of space that our
//      openGL camera can see. The cam's horizontal and vertical field-of-view
//      defines the pyramid's sides, the eyepoint (the camera's center of pro-
//      jection) defines the pyramid's peak, and the zFar clipping plane defines
//      the pyramid's base (4 sides + base = 5 sides).  The 'zNear' clipping
//      plane then clips off the peak of the pyramid to complete our 6-sided
//      viewing frustum.  The rather cryptic contents of the GL_PROJECTION
//      matrix made for us by openGL's glPerspective() or glFrustum() functions
//      do nothing more than transform this six-sided 'viewing frustum' to the
//      six-sided CVV (+/1 1.0 in x,y,z).
//  Thus the output of the GL_PROJECTION matrix defines a strangely warped
//      version of 3D space. Output (x,y) values define image position values
//      with (-1,-1) for lower-left, (+1,+1) for upper right corners, and the
//      output's z values are even stranger--a non-uniform but monotonic map
//      from input z values between zNear and zFar to output z values between
//      -1 and +1. Output z values aren't proportional to input z values or
//      depth, yet these 'pseudo-depth' values still specify a
//      wholly-consistent front-to-back ordering that keeps all 3D occlusions.
//  The openGL hardware then 'clips' all these drawing primitives to the CVV.
//          --it discards any part of any drawing primitive that leaves the
//          GL_PROJECTION matrix:parts with x,y,z values outside the range +/-1.
//          --This cube-shaped range (+/-1.0, +/-1.0, +/-1.0), is the
//          'Canonical View Volume' (CVV). Any part of any drawing primitive
//          outside the CVV cannot appear on-screen.
//          --The CVV is fixed in hardware: no command can change it, ever!
//  3) --Third and finally, the 'viewport' matrix (set by the glViewport()
//          command) maps the CVV's contents to a specified part (or all) of
//          the display window.
//--------------------!
//  CAREFUL! *ALL THREE* matrices can change the position of any vertex within
// the display window; if you change display window size & location, you may
//  need to change glViewport, GL_PROJECTION, and/or GL_MODELVIEW.
//--------------------!
//   So, what goes where?
//       What vertex coordinates actually arrive on-screen?
//                   ANSWER: all parts of all transformed primitives that
//                   fall within the CVV *AND* are within the portion of the
//                   CVV that is mapped to the display by the viewport matrix.
// !??! WHAT !?!?
// ==============
// ?!?! How can I do all that in my very first program ?!?!
//
// Fortunately, openGL sets sensible initial values for everything.
// Initially, GL_MODELVIEW and GL_PROJECTION both hold 4x4 indentity matrices
// that leave all vertex values unchanged.  Initially, the Viewport matrix holds
// values that (thanks to GLUT) fill your display window with the entire CVV;
// thus I like initial display windows that are square, not rectangular.
// With those initial settings, all vertices that specify (x,y,z) values
// within (+/-1, +/-1, +/-1) (inside the CVV) will create pictures on-screen;
// that's easy even for your first program.  Also, if you mess up any matrices,
// you can always return to this initial state by re-initializing all of them:
//          glMatrixMode(GL_MODELVIEW); // select and
//          glLoadIdentity();           // set to identity matrix.
//          glMatrixMode(GL_PROJECTION);// select and
//          glLoadIdentity();           // set to identity matrix.
//          glViewport(0,0,screenWidth,screenHeight); // fill screen with CVV.
// ===========================
// 1) HOW TO SET THE VIEWPORT
// ===========================
// The 'viewport' is the first of the 3 matrices that you'll want to specify.
// The glViewport() command specifies where to show the CVV contents within the
// on-screen display window.   The method is rather odd, its descriptions often
// neglected and sloppy, and the official openGL explanations are complicated
// because they're meant for videocard designers, so I suggest you remember the
// glViewport() arguments as:
//
//          glViewport(GLint xLL, GLint yLL, GLint CVVwidth, GLint CVVheight);
//
// --The (xLL,yLL) arguments specify the display window's pixel address for the
//   CVV's (-1,-1) (lower-left) corner; any vertex that leaves the GL_PROJECTION
//   matrix with value(-1,-1, -1.0<= z <1.0) will appear at (xLL,yLL) in your
//   display window's pixel coord. system (whose origin is at the lower left).
// --The 'CVVwidth' and 'CVVheight' specify how many pixels the entire CVV will
//   cover on-screen if your display window is large enough to hold it.  If the
//   display window is larger, some window regions will remain empty. If too
//   small, the display window will show only the portion of the CVV that fits
//   inside the display window.
//
// The CVV is always square (+/-1), but our display windows are not, so
//  you have several obvious choices for setting the viewport here in reshape():
//  Call the myApp.doViewportChoice() to choose between:
//      a) squash/stretch the CVV to fill the entire display window (yuck!).
//          This will squash/stretch your openGL picture if your display
//          window isn't square (looks bad; makes users mistrust your program).
//      b) Fit the entire CVV inside the display window (meh.)
//          You can scale the entire CVV to fit inside the window; scale it to
//          the SMALLER of the window width and height. This eliminates the
//          squash/stretch, but leaves a pair of empty rectangles on the sides
//          or the top and bottom of any non-square display window.
//      c) Fit the entire display window inside the CVV (meh.)
//          You can scale the entire CVV to fill the entire display; scale it to
//          the LARGER of the window width and height.  This eliminates both the
//          squash/stretch and the empty rectangles in the display window, but
//          it lets openGL waste time and resources rendering drawing primitives
//          in the CVV that end up off-screen and never seen.
//      d) Squash/stretch the CVV to fill the entire display window, **AND**
//          do the reverse, e.g. stretch/squash all the drawing primitives we
//          put into the CVV by setting the GL_PROJECTION matrix (it 'optics'
//          matrix for your openGL 3D/2D camera).  (good!)
//          Each of these two distortions cancel the other, so that:
//          --all drawing primitives appear correct and trustworthy on-screen,
//              (neither squashed nor stretched), and
//          --the CVV 'clips' all primitives at the display window edges for ANY
//              display window size or shape. Then openGL won't waste any time
//              drawing primitives that won't appear in the on-screen window.
//
// Try choices b) or c) first; they're easy to do and never confusing.
// You're not required to fill every pixel of the display window with openGL
// drawings, and nothing REQUIRES you to show the entire CVV contents
// within your display window.
}

//-------------------------------
// (UNCOMMENT ONLY ONE OF THESE!)
//-------------------------------
/*
// CHOICE a):

    glViewport(0,0,nuWidth, nuHeight);      // fill the whole window with CVV.
                        // (!will squash/stretch CVV contents on-screen!)
//-------------------------------
*/
/*
// CHOICE b):
    if(nuWidth < nuHeight)
    {   // 'tall' display window:
        glViewport(0,(nuHeight-nuWidth)/2,nuWidth,nuWidth);
    // Stretch CVV across entire display window width, and center the CVV
    // vertically within the window.  Places the CVV's lower-left corner at
    // (0, (nuHeight-nuWidth)/2), and leaves an empty rectangle at the top
    // and bottom sides of the display window.
    }
    else
    {   // 'wide' display window:
        glViewport((nuWidth-nuHeight)/2,0,nuHeight,nuHeight);
    // Stretch CVV across entire display window height, and center the CVV
    // horizontally within the window.  places the CVV's lower-left corner at
    // ((nuWidth-nuHeigh)/2,0), and leaves an empty rectangle at the left
    // and right sides of the display window.
    }
*/
/*
//------------------------------

// Choice c):
    // Scale, offset the CVV to center and enclose within it the entire
    // display window.  Instead of empty display regions, you lose some CVV:
    if(nuWidth > nuHeight)
    {   // 'wide' display window.  Start CVV at point BELOW window origin:
        glViewport(0, -(nuWidth-nuHeight)/2, nuWidth, nuWidth);
    }
    else
    {   // 'tall' display window.  Start CVV at point to LEFT of window origin:
        glViewport(-(nuHeight-nuWidth)/2, 0, nuHeight, nuHeight);
    }

*/
//------------------------------
// Choice d):  the BEST-LOOKING BUT MOST COMPLEX SOLUTION:
//      --Fill the (non-square) display window with the entire CVV, but then
//      --eliminate the squash/stretch by constructing a camera in GL_PROJECTION
//      matrix (below) that has a non-square field of view--a field of view
//      whose aspect ratio (width/height) MATCHES the display window's ratio.
//      --use GL_MODELVIEW matrix to properly POSITION and AIM the camera
//      (usually in myDisplay(), usually by using glLookAt(), etc) so that the
//      camera looks at the objects you want to see!  If you leave GL_MODELVIEW
//      at its default value, you'll have a camera at the world-space origin
//      that looks down the -z axis!
// Despite the camera and view setting requirements, this is the most
// 'highly recommended' method, because it draws undistorted images in the
// entire display window, because the CVV clips drawing primitives only at the
// display window limits, and because eventually you'll define a 3D camera and
// position it to get a nice 3D picture.

    glViewport(0,0,nuWidth, nuHeight);          // fill display with CVV,
    // ***BUT** YOU MUST ALSO set the GL_PERSPECTIVE matrix (below) to use
    // an openGL 2D/3D 'camera' whose aspect ratio (width/height) matches the
    // display window.
//==============================================================================
// SET UP OpenGL 'CAMERA'
//==============================================================================
// The GL_PROJECTION matrix on openGL's fixed-pipeline holds the transformations
// that define a 'camera'; the transformation from 3D x,y,z values to the CVV,
// the 'canonical view volume' measuring (+/1, +/-1, +/-1) that defines what
// drawing primitives can appear on-screen.

// CHOOSE NONE, or ONLY ONE:
//------CHOICE A: 'default'--do nothing, get an orthographic camera:

//------CHOICE B: centered, unit-height orthographic camera:
//          Display screen's vertical dimension always spans +/-1; horizontal
//          span is proportional to aspect ratio (nuWidth/nuHeight) to ensure
//          always-square pixels (presumes glViewport(0,0,nuWidth, nuHeight);).
    glOrtho(  -(GLdouble)nuWidth/(GLdouble)nuHeight,   // left side limit,
               (GLdouble)nuWidth/(GLdouble)nuHeight,    // right side limit,
               -1.0, 1.0,       // bottom,top
               -1.0, 1.0);      // near, far of this camera's view frustum.
/*
//------CHOICE C: 'default' perspective camera-------------
// --Define the camera's 'fixed' vertical field of view 'vFOV' in degrees,
// --Define the camera's (variable" horizontal field of view to match the
//      display window's aspect ratio (width/height),
// --Define the camera's 'znear' and 'zfar' as fixed values suitable for
//      exploring a 'world' coordinate system that fits entirely within the CVV:
//      where the unit cube (+/-1,+/-1,+/-1) centered at the origin encloses
//      all visually interesting objects we'll explore with the camera.

double myZnear = 1.0;
	// Select the projection matrix
	glMatrixMode(GL_PROJECTION);		// Select the Projection matrix,
	glLoadIdentity();			// (Clear out any previous camera settings)
	gluPerspective(				// Set camera's internal parameters:
		30.0,					// vertical (y-axis) field-of-view in degrees,
                                // (total FOV, top-to-bottom, not half-angle)
		(double)nuWidth/(double)nuHeight,
                                // display image aspect ratio (width/height),
		myZnear,5.0);		    // near,far clipping planes for camera depth.
    // POSITION and AIM the camera (later, move this to myDisplay so that you
    // can CHANGE these positions...
    glMatrixMode(GL_MODELVIEW);    // select the Model-View matrix
    glLoadIdentity();               // clear any previous settings
    gluLookAt(0.0, 0.0, 4.0,    // camera 'eye' position:
                                        // Fill camera image with CVV contents
                0.0, 0.0, 0.0,          // "look-at' point: look at origin
                0.0, 1.0, 0.0);         // 'up' direction: +y axis.
*/
//-----------------END CHOICE A:----------------


/*
//------CHOICE C: 'pixel-world'----------------
// --Define the camera's (fixed) vertical field of view 'vFov' in degrees,
// --Define the camera's (variable) horizontal field of view to match the
//      display window's aspect-ratio (width/height),
// --Define the camera's 'znear' value ALSO in pixels.  This means we'll measure
//  drawing primitives in units of pixels in the 'world' coordinate system as
//  well.  If you put the camera origin at (0,0,znear+1) in world coord system
//  and aim it at the origin, then a 100x100-pixel rectangle placed at z=0 will
//  appear as a 100x100pixel object in our display winodw.

double vFOV = 20.0;             // Define cam's total (top-to-bottom) vertical
                                // field-of-view.
double myZnear;                 // znear value, calculated in pixels, for the
                                // given vFOV and display window height.
    myZnear = ((double)nuHeight/2.0) / tan(vFOV/2.0);
	glLoadIdentity();			// (Clear out any previous camera settings)
	gluPerspective(				// Set camera's internal parameters:
		vFOV,					// vertical (y-axis) field-of-view in degrees,
                                // (total FOV, top-to-bottom, not half-angle)
		(double)nuWidth/(double)nuHeight,
                                // display image aspect ratio (width/height),
		myZnear, myZnear*1000.0);// near,far clipping planes for camera depth.
//------------END CHOICE A:------------------
*/

	glutPostRedisplay();			// request redraw--we changed window size!
}

void myDisplay( void )
//------------------------------------------------------------------------------
// (REGISTERED AS GLUT 'display' CALLBACK--don't call this fcn in your code!)
// GLUT calls this fcn. whenever it needs to re-draw the display window.
// OPENGL TRANSFORMATION SYNTAX HINTS:
//	glRotated(-123, 1,-2,1);	// rotate by -123 degrees around 3D axis(1,-2,1)
//	glScaled(0.2, 0.3, 0.4);	// shrink along x,y,z axes;
//  glTranslated(0.1, 0.3, 0.5);// translate along x,y,z axes.
{
int i;

    if(myApp.doClear==1)
    {   // toggled by 'c' or 'C' key, ==1 shows shapes, ==0 shows time-trails.
        glClear( GL_COLOR_BUFFER_BIT |      // clear the color image buffer,
                 GL_DEPTH_BUFFER_BIT |      // clear the depth ('z') buffer,
                 GL_STENCIL_BUFFER_BIT );   // clear the stencil mask, if any.
    }
    //------------------------------ANIMATION:
    myApp.shape_deg += myApp.shape_degStep;  // Move forwards by one step.
    // angular wraparound; keep angles between +/-180 degrees.
    if(myApp.shape_deg > 180.0) myApp.shape_deg -= 360.0;
    if(myApp.shape_deg <-180.0) myApp.shape_deg += 360.0;
    //----------------------------------------

    myApp.drawAxes();               // Draw current coordinate axes to show
                                    // origin at window center. (r,g,b==x,y,z)
    myApp.drawShape();              // Draw our selected shape.
                                    // (try function keys to change shapes).
    myApp.drawText3d(-0.9, -0.60, -0.9,
                     "Arrow keys, PgUp/PgDn, Home key move selected CByteImg in x,y,z;", 1);
    myApp.drawText3d(-0.9, -0.65, -0.9,
                     //"F1--change object; F2/F3 change spin rate;",
                        "F5,F6--select next CByteImg/next animated shape; ",1);
    myApp.drawText3d(-0.9, -0.70, -0.9,
                     "Try mouse; Try A,C,P, and other keys", 1);
    myApp.drawText3d(-0.9, -0.75, -0.9,
                     "Hit 'f' or 'F' key for PixelStoreReport and filename prompt", 1);
    myApp.drawText3d(-0.9, -0.80, -0.9,
                     "Hit 'H','J','K','L' or 'M', for pixel-buffer test patterns",1);
    myApp.drawText3d(-0.9, -0.85, -0.9,
                     "To quit, hit Q, q, ESC or SPACE bar.", 1);

// Currently we're drawing in the CVV (+/-1, +/-1,+/-1), so put image's lower
// left corner near the display window's lower-left corner:
//    glRasterPos3d(-0.9, -0.3, 0.0);     // set 3D position for any raster-ops
                                        // in the current coordinate system.
                                        // (CAREFUL! this position gets xformed
                                        // by GL_MODELVIEW and GL_PROJECTION
                                        // before use!
    for(i=0; i<myApp.imgMax; i++)
    {
        glRasterPos3d(myApp.imgXpos[i], myApp.imgYpos[i], myApp.imgZpos[i]);
        myApp.myImg[i].drawPixelsGL();  // draw onscreen images from files
                                        // at current glRasterPos() position.
    }
	//===============DRAWING DONE.
	glFlush();	                // do any and all pending openGL rendering.
//	cout << "*";                // tick-mark for each sensed position change
	glutSwapBuffers();			// For double-buffering: show what we drew.
}

void myKeyboard(unsigned char key, int xw, int yw)
//------------------------------------------------------------------------------
// (REGISTERED AS GLUT 'keyboard' CALLBACK--don't call this fcn in your code!)
// GLUT calls this fcn when users press an alphanumeric key on the keyboard.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left)
//  (NOTE!  freeGLUT also provides glutKeyboardUpFunc() to register a callback
//      called when users RELEASE keyboard key.
//      SEE: http://freeglut.sourceforge.net/docs/api.php#Introduction
{
int xpos,ypos;  // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = myApp.display_height - yw;//(window system puts origin at UPPER left)
    myApp.reportKeyModifiers("myKeyboard()"); //Report any SHIFT,ALT,CTRL usage
	switch(key)
	{
		case 'A':       // User pressed the 'A' key...
		case 'a':
		cout << "that *IS* the 'A' key!\n";
			break;
        case 'c':
        case 'C':       // toggle 'isCleared' to enable/disable screen clearing
                        // in myDisplay before screen redrawing.
            cout <<"C key toggles screen-clearing on/off" << endl;
            if(myApp.doClear==0) myApp.doClear = 1;
                            else myApp.doClear = 0;
            break;
        case 'f':
        case 'F':       // Prompt for filename
            cout << "F key calls CTheApp::printPixelStoreReport()";
            cout << "and asks user to choose an input image filename." << endl;
            myApp.printPixelStoreReport();
            myApp.myImg[0].askFileName(myApp.argvCopy);
//            myA
            break;
        case 'h':
        case 'H':
            myApp.myImg[myApp.imgCurrent].makeChecker(19);
            break;
        case 'j':
        case 'J':
            myApp.myImg[myApp.imgCurrent].makeColorRamps();
            break;
        case 'k':
        case 'K':
            myApp.myImg[myApp.imgCurrent].makeConstant(188);
            break;
        case 'l':
        case 'L':
            myApp.myImg[myApp.imgCurrent].makeRandom(9);
            break;
        case 'm':
        case 'M':
            myApp.myImg[myApp.imgCurrent].readFile();
            break;
        case 'p':
        case 'P':       // Toggle animation on/off.
            cout << "P key toggles animation on/off" << endl;
            if(myApp.doAnim ==1) runAnim(0);
                            else runAnim(1);
            break;
		case ' ':		// User pressed the spacebar.
		case 27:		// User pressed the 'Esc'  key...
		case 'Q':		// User pressed the 'Q' key...
		case 'q':
			exit(0);    // Done! quit the program.
			break;
        default:
            cout << "Keyboard: you pressed the '" << key << "' key."<<endl;
	        myApp.drawText3d(0.0, 0.80, -0.9, "!!WHAT??", 4);
            myApp.drawText3d(0.0, 0.72, -0.9, "!!WHAT??", 4);
            myApp.drawText3d(0.0, 0.64, -0.9, "!!WHAT??", 4);
            //===============DRAWING DONE.
            glFlush();	        // do any and all pending openGL rendering.
            glutSwapBuffers();	// For double-buffering: show what we drew.
        break;
	}
}

void myKeySpecial(int key, int xw, int yw)
//------------------------------------------------------------------------------
// (REGISTERED AS GLUT 'keySpecial' CALLBACK--don't call this fcn in your code!)
// Called by GLUT when users press 'arrow' keys, 'Fcn' keys, or any other
// non-alphanumeric key.  A quick summary:
//    GLUT_KEY_F1, GLUT_KEY_F2, ..., GLUT_KEY_F12 - F1 through F12 keys
//    GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN - Page Up and Page Down keys
//    GLUT_KEY_HOME, GLUT_KEY_END - Home and End keys
//    GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN - Arrow keys
//    GLUT_KEY_INSERT - Insert key
// Search glut.h to find other #define literals for
// special keys, such as GLUT_KEY_F1, GLUT_KEY_UP, etc.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left!)
//
//  (NOTE!  freeGLUT also provides glutSpecialUpFunc() to register a callback
//      called when users RELEASE 'special' key on the keyboard)
//      SEE: http://freeglut.sourceforge.net/docs/api.php#Introduction
{
int xpos,ypos;      // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = myApp.display_height - yw;//(window system puts origin at UPPER left)
    myApp.reportKeyModifiers("myKeySpecial()");//Report any SHIFT,ALT,CTRL usage
	switch(key)
	{
		case GLUT_KEY_LEFT:		// left arrow key
            //myApp.shape_xpos -= 0.025;
            myApp.imgXpos[myApp.imgCurrent] -= 0.025;
            cout << "left-arrow key: move myImg[" << myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
			break;
		case GLUT_KEY_RIGHT:	// right arrow key
            /* myApp.shape_xpos += 0.025;
            cout << "right-arrow key. \t myApp.shape_xpos = "
                 << myApp.shape_xpos << endl;
            */
            myApp.imgXpos[myApp.imgCurrent] += 0.025;
            cout << "right-arrow key: move myImg[" << myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
			break;
		case GLUT_KEY_DOWN:		// dn arrow key
            /* myApp.shape_ypos -= 0.025;
            cout << "dn-arrow key. \t myApp.shape_ypos = "
                 << myApp.shape_ypos << endl;
            */
            myApp.imgYpos[myApp.imgCurrent] -= 0.025;
            cout << "down-arrow key: move myImg[" << myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
			break;
		case GLUT_KEY_UP:		// up arrow key
            /* myApp.shape_ypos += 0.025;
            cout << "up-arrow key. \t myApp.shape_ypos = "
                 << myApp.shape_ypos << endl;
            */
            myApp.imgYpos[myApp.imgCurrent] += 0.025;
            cout << "up-arrow key: move myImg["<< myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
			break;
        case GLUT_KEY_PAGE_UP:
            /* myApp.shape_zpos += 0.025;
            cout << "page-up key. \t myApp.shape_zpos = "
                 << myApp.shape_zpos << endl;
            */
            myApp.imgZpos[myApp.imgCurrent] += 0.025;
            cout << "PG-UP key: move myImg["<<myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
            break;
        case GLUT_KEY_PAGE_DOWN:
            /* myApp.shape_zpos -= 0.025;
            cout << "page-dn key. \t myApp.shape_zpos = "
                 << myApp.shape_zpos << endl;
            */
            myApp.imgZpos[myApp.imgCurrent] -= 0.025;
            cout << "PG-UP key: move myImg["<<myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
            break;
		// SEARCH glut.h for more special-key #define statements.
		case GLUT_KEY_HOME:
            /*
            myApp.shape_xpos = 0.0;     // reset position
            myApp.shape_ypos = 0.0;
            myApp.shape_zpos = 0.0;
            myApp.shape_deg = 0.0;
            */
            myApp.imgXpos[myApp.imgCurrent] = 0.0;
            myApp.imgYpos[myApp.imgCurrent] = 0.0;
            myApp.imgZpos[myApp.imgCurrent] = 0.0;
            cout << "HOME key: move myImg["<<myApp.imgCurrent << "] to ";
            cout << myApp.imgXpos[myApp.imgCurrent];
            cout << myApp.imgYpos[myApp.imgCurrent];
            cout << myApp.imgZpos[myApp.imgCurrent] << endl;
            break;
        case GLUT_KEY_F5:
            myApp.imgCurrent = (myApp.imgCurrent+1)%myApp.imgMax;
            cout << "F5 Key: select next image: " << myApp.imgCurrent << endl;
            break;
        case GLUT_KEY_F6:
             myApp.shapeNum = (++myApp.shapeNum) % 8;   // 8 choices only.
            cout << "F6 key: change shapes: shapeNum = "
                 << myApp.shapeNum << endl;
            break;
        case GLUT_KEY_F7:
            myApp.shape_degStep += 0.123456;
            cout << "F7 key: accelerate CCW: shape_degStep = "
                 << myApp.shape_degStep << endl;
            break;
		default:
			cout << "Special key; integer code value"<< (int)key << "\n)";
			break;
	}
    myApp.drawText3d(0.5, -0.7, -0.9, "!That's a SPECIAL key!", 0);
    //===============DRAWING DONE.
    glFlush();	        // do any and all pending openGL rendering.
    glutSwapBuffers();	// For double-buffering: show what we drew.
}

void myMouseClik(int buttonID, int upDown, int xw, int yw)
//-----------------------------------------------------------------------------
// (REGISTERED AS GLUT 'mouse' CALLBACK--don't call this fcn in your code!)
// Called by GLUT when users press or release a mouse button;
//                          buttonID== 0 for left mouse button,
//                                 (== 1 for middle mouse button?)
//	                                == 2 for right mouse button;
//	                        upDown  == 0 if mouse button was pressed,
//	                                == 1 if mouse button released.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left!)
{
int xpos,ypos;  // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = myApp.display_height - yw;//(window system puts origin at UPPER left)
    myApp.reportKeyModifiers("myMouseClik()");//Report any SHIFT,ALT,CTRL usage
    cout << "click; buttonID=" << buttonID <<", upDown=" << upDown;
    cout << ", at xpos,ypos=(" << xpos <<"," << ypos << ")\n";
    myApp.drawText3d(0.4, 0.8, -0.9, "!MOUSE CLICK!", 4);
    //===============DRAWING DONE.
    glFlush();	        // do any and all pending openGL rendering.
    glutSwapBuffers();	// For double-buffering: show what we drew.
}

void myMouseMove(int xw, int yw)
//-----------------------------------------------------------------------------
// (REGISTERED AS GLUT 'motion' CALLBACK--don't call this fcn in your code!)
// Called by GLUT when user moves mouse while pressing any mouse button.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left)
{
int xpos,ypos;  // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = myApp.display_height - yw;
    //(window system puts origin at UPPER left; myApp puts origin at LOWER right

    myApp.reportKeyModifiers("myMouseMove()"); //Report any SHIFT,ALT,CTRL usage
    cout << ".";                    // tick-mark for each sensed position change
    glColor3d(1.0, 0.5, 0.5);	//bright red text:
    myApp.drawText3d(-0.3 + 0.001*xpos,
                     -0.3 + 0.001*ypos, -0.9, "!MOUSE MOVE!", 4);

    //===============DRAWING DONE.

    glFlush();	        // do any and all pending openGL rendering.
    glutSwapBuffers();	// For double-buffering: show what we drew.
//    glutPostRedisplay();
}

void runAnim(int isOn)
//-----------------------------------------------------------------------------
// Call runAnim(0) to STOP animation;
//      runAnim(1) to START or CONTINUE it.
{
    myApp.doAnim = isOn;   // Update global var to the most-recent state;
    if(isOn == 1)          // Start or continue animation?
    {   //YES. ANIMATE!   register a GLUT timer callback:
        glutTimerFunc(myApp.animFrameTime, myTimer, 1);
        // This registers 'myTimer()' as a timer callback function.
        // GLUT will now pause NU_PAUSE milliseconds (longer if busy drawing),
        // call myTimer(1), and then 'un-register' the callback; it won't call
        // myTimer() again unless we register it again.
        // myTimer(1) checks to be SURE no other 'stop animation' requests
        // arrived (e.g. keyboard input, mouse input, visibility change, etc.)
        // while we waited.  If global variable nu_Anim_isOn is STILL 1,
        // it calls runAnimTimer(1) to continue animation.
    }
    // NO.  STOP animation-- don't register a GLUT timer for myTimer().
}

void myTimer (int value)
//------------------------------------------------------------------------------
// Don't call this function yourself; use runAnim() instead!
// runAnim(1) causes GLUT to wait NU_PAUSE milliseconds (longer if busy)
// and then call this function.
// TRICKY: We allow users to choose very long NU_PAUSE times to see what happens
// in animation.  During GLUT's wait time, user events (keyboard, mouse, or
// window cover/uncover) may make runAnim(0) calls intended to STOP
// animation.  GLUT can't cancel a previously-issued GLUT timer callback, so
// in the TimerCallback() function we check global variable nu_Anim_isON.
{
                                // GLUT finished waiting.
    if(value != myApp.doAnim)
        cout << "\n(Anim cancelled during GLUT wait)\n";
    if(myApp.doAnim==1)         // Go on with animation?
    {                           // YES.
        glFlush();              // Finish any pending drawing requests,
        glutPostRedisplay();    // tell GLUT to re-draw the screen, and
        runAnim(1);             // continue with the next time interval.
    }
                                // NO. Do nothing. No further animation.
}

void myHidden (int isVisible)
//------------------------------------------------------------------------------
// (REGISTERED AS GLUT 'visibility' CALLBACK--don't call this fcn in your code!)
// Called by GLUT when our on-screen window gets covered or un-covered. When
// called, it turns off animation and re-display when our display window is
// covered or stowed (not visible). SEE ALSO: runAnimTimer() and myTimer()
{
    if(isVisible == GLUT_VISIBLE)  runAnim(1);      // Visible! YES! Animate!
    else runAnim(0);                                // No. Stop animating.
}

CTheApp::CTheApp(void)
//------------------------------------------------------------------------------
// default constructor
{
    display_width = 640;	            // global variables that hold current
    display_height = 640;	            // display window position and size;
    display_xpos = 200;
    display_ypos = 100;
    strncpy(winTitle, "2011 NU EECS 351-1 GlutPak", 127); // (buffer size:128)
    doAnim  = 1;                      // ==1 to run animation, ==0 to pause.
    doClear = 1;
    animFrameTime = NU_PAUSE;           // default: wait between frames in mS
    // screen-clear color;
    clrColr4dv[0]= 0.7;
    clrColr4dv[1]= 0.7;
    clrColr4dv[2]= 0.7;
    clrColr4dv[3]= 0.7;
    // drawing (pen) color;
    penColr4dv[0]= 82.0/255.0;      // color; 0.0<=RGB<=1.0, GLdouble
    penColr4dv[1]= 0.0;             // Official NU purple; R== 82, G== 0, B== 99
    penColr4dv[2]= 99.0/255.0;
    penColr4dv[3]= 0.0;
    // text-writing color;
    txtColr4dv[0] = 0.1;
    txtColr4dv[1] = 0.4;
    txtColr4dv[2] = 0.1;
    txtColr4dv[3] = 0.5;
    // error-reporting color;
    errColr4dv[0] = 0.0;    // PURE BLACK
    errColr4dv[1] = 0.0;
    errColr4dv[2] = 0.0;
    errColr4dv[3] = 1.0;

    shapeNum = 0;                       // Shape-selector number.
    shape_xpos = shape_ypos = 0.0;      // initial position (arrow keys)
    shape_deg = 0.0;                    // initial orientation
    shape_degStep = 0.51;               // rotation (degrees) per time-step

    //  Start up the FreeImage library for image-file I/0:
#ifdef FREEIMAGE_LIB
	// call this ONLY when linking with FreeImage as a static library
		FreeImage_Initialise();
#endif
}

CTheApp::~CTheApp(void)
//------------------------------------------------------------------------------
// default destructor
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif
    return;
}

void CTheApp::reportKeyModifiers(const char *pName)
//------------------------------------------------------------------------------
// Text and graphical display of any keyboard modifiers (SHIFT, CTRL, ALT).
// Used by keyboard & mouse callback fcns.
// Usage:  within myKeyboard() --   myApp.reportKeyModifiers("myKeyboard()");
{
    KBflags = glutGetModifiers();           // GLUT's keyboard-mod reader
    if(KBflags & GLUT_ACTIVE_SHIFT)
    {
        drawText3d(-0.9, 0.9, -0.9, "!!!SHIFT KEY!!!", 4);  // on-screen
        cout << pName << ": SHIFT-key" << endl;             // in text window
    }
    if(KBflags & GLUT_ACTIVE_CTRL )
    {
        drawText3d(-0.9, 0.8, -0.9, "!!! CTRL KEY!!!", 4);
        cout << pName << ": CTRL-key" << endl;
    }
    if(KBflags & GLUT_ACTIVE_ALT  )
    {
        drawText3d(-0.9, 0.7, -0.9, "!!! ALT  KEY!!!", 4);
        cout << pName << ": ALT-key" << endl;
    }
}

void CTheApp::drawAxes(void)
//------------------------------------------------------------------------------
// Draw small +x,+y,+z axes in red, green, blue:
{
	glColor3d(1.0, 0.0, 0.0);	// red
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +x-axis
		glVertex3d(0.4,0.0,0.0);
	glEnd();
	drawText3d(0.4, 0.0, 0.0, "X", 2);       // label it.

	glColor3d(0.0, 1.0, 0.0);	// green
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +y-axis
		glVertex3d(0.0,0.4,0.0);
	glEnd();
    drawText3d(0.0, 0.4, 0.0, "Y", 2);       // label it.

	glColor3d(0.0, 0.5, 1.0);	// bright blue
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +z-axis
		glVertex3d(0.0,0.0,0.4);
	glEnd();
	drawText3d(0.0, 0.0, 0.4, "Z" ,2);       // label it.
}

void CTheApp::drawText3d( GLdouble x0, GLdouble y0, GLdouble z0,
                         const char *pString, int fontNum)
//-----------------------------------------------------------------------------
//  Use GLUT to write (bitmapped) text string that starts at pointer 'pString'.
//  --Use current pen color (CTheApp::penColrv4), but if the text string begins
//      with an exclamation point, it uses error color (CTheApp::errColrv4).
//  --Places the lower left corner of the first line of text at screen position
//      x0,y0,z0 in the current coordinate system.
//      Example:  DrawText3D(0.3, 0.4, 0.0,"Hello!", 4);
//  -- uses 'fontNum' to select among these GLUT pre-defined fonts:
// Font#
//  0   --  10pt HELVETICA
//  1   --  12pt HELVETICA
//  2   --  18pt HELVETICA
//  3   --  10pt TIMES_ROMAN
//  4   --  24pt TIMES_ROMAN
//-------------------------------
// NOTE: Unlike GLUT, the freeGLUT library offers support for stroke-font chars
// and functions for strings; you may want to use that instead
// SEE: glutBitmapString(), glutStrokeCharacter(), glutStrokeString() at this
//  site:  http://freeglut.sourceforge.net/docs/api.php#Introduction
{
int i, imax;					// character counters
void *pFont;                    // function ptr for fonts

    // IF string begins with '!' (exclamation point)
    if(pString[0]=='!' || pString[1]=='!')
         glColor4dv(errColr4dv);        // use the 'error' text color
    else glColor4dv(txtColr4dv);	    // otherwise, ordinary text color
    switch(fontNum)
    {
        case 0: pFont = GLUT_BITMAP_HELVETICA_10;   break;
        case 1: pFont = GLUT_BITMAP_HELVETICA_12;   break;
        case 2: pFont = GLUT_BITMAP_HELVETICA_18;   break;
        case 3: pFont = GLUT_BITMAP_TIMES_ROMAN_10; break;
        case 4:     // (the default choice)
        default:pFont = GLUT_BITMAP_TIMES_ROMAN_24; break;
    }
	glRasterPos3d(x0, y0, z0);	// set text's lower-left corner position
	imax = 1023;				// prevent overflow.
	for(i=0; pString[i] != '\0' && i<imax; i++)	// for each char,
	{
		glutBitmapCharacter(pFont, pString[i]);
	}
}

void CTheApp::drawShape(void)
//------------------------------------------------------------------------------
// Use 'penColr[]' to draw the GLUT shape selected by 'shapeNum' at (xpos,ypos)
// using the current coordinate system.
// TRANSFORMATION SYNTAX HINTS:
//	glRotated(-123, 1,-2,1);	// rotate by -123 degrees around 3D axis vector;
//	glScaled(0.2, 0.3, 0.4);	// shrink along x,y,z axes
//  glTranslated(0.1,0.3,0.5);	// translate along x,y,z axes.
{
    glMatrixMode(GL_MODELVIEW);     // use this matrix to change coord systems
    glPushMatrix();                 // save its current value,

        glTranslated(shape_xpos, shape_ypos, shape_zpos);   // change coords.
        // Draw a selected shape from those GLUT supplies 'ready-made':
        glRotated(shape_deg, 0.0, 0.0, 1.0);       // rotate around Z axis.

        // Set drawing color:
        glColor4dv(penColr4dv);      // Set pen color,
        switch(shapeNum)            // Draw the (selected) GLUT shape
        {
            case 0: glutWireCube( 1.0 ); break; // length of cube's edges
            case 1: glutSolidCube ( 1.0 ); break;
            // How is glRectd() different?.... (HINT: it's 2D, not 3D)
            //      glRectd(-0.5, -0.5,     // Lower-left x,y
            //               0.5,  0.5);     // Upper-right x,y.
                             break;
            case 2:
                glPushMatrix();             // save current tranformation matrix
                    glRotated(80.0,0.0, 1.0, 0.0);  // turn 80 deg on y axis,
                    glutWireCone( 1.0, 1.0, 10, 6 );// draw 3D cone
                glPopMatrix();              // restore prev. transform. matrix
            break;
            case 3:
                glPushMatrix();             // save current tranformation matrix
                    glRotated(80.0,0.0, 1.0, 0.0);  // turn 80 deg on y axis,
                    glutSolidCone( 1.0, 1.0, 10, 6);// draw 3D cone
                glPopMatrix();              // restore prev. transform. matrix
            break;
            case 4:
                glPushMatrix();             // save current tranformation matrix
                    glRotated(40.0,0.0, 1.0, 0.0);  // turn 40 deg on y axis,
                    glutWireTorus(
                    0.3,            // inner radius, (smaller of the 2 radii;
                                    // radius of rings enclosing 'core-circle' )
                    0.7,            // outer radius, (larger of the 2 radii;
                                    // for 'core circle' enclosed within torus)
                    16,             // number of sides for each ring;
                    32);            // number of rings around center-circle.
                glPopMatrix();              // restore prev. transform. matrix
                break;
            case 5:
                glutSolidTorus(
                0.3,            // inner radius, (smaller of the 2 radii;
                                // radius of rings enclosing 'core-circle' )
                0.7,            // outer radius, (larger of the 2 radii;
                                // for 'core circle' enclosed within torus)
                16,             // number of sides for each ring;
                32);            // number of rings around center-circle.
                break;
/*
Other GLUT shapes for you to try... found in GLUT.h (or freeglut_std.h)
case XX:glutWireSphere( GLdouble radius, GLint slices, GLint stacks );
case XX:glutSolidSphere( GLdouble radius, GLint slices, GLint stacks );
case XX:glutWireDodecahedron( void );
case XX:glutSolidDodecahedron( void );
case XX:glutWireOctahedron( void );
case XX:glutSolidOctahedron( void );
case XX:lutWireTetrahedron( void );
case XX:glutSolidTetrahedron( void );
case XX:glutWireIcosahedron( void );
case XX:glutSolidIcosahedron( void );
*/
            case 6:
                glPushMatrix();             // save current tranformation matrix
                glRotated(11.0, 1.0, 0.0, 0.0);  // turn 80 deg on x axis,
                    glutWireTeapot( 0.5 );
                glPopMatrix();              // restore prev. transform. matrix
                    break;      // size.
            case 7:    // (default value)
            default:    glutSolidTeapot( 0.5 );         // size.
                break;
        }
    glPopMatrix();                  // restore its previous coord. system.
    return;
}

/*===============================================================================
 OpenGL Pixel-Block Operations: --"BLITTING"--
 =============================
OpenGL gives you exquisitely flexible tools for manipulating blocks of pixels
held in your computers memory and in its openGL graphics hardware.  However,
the default settings are not always correct for all uses on all machines; if
not, use these formatting functions first:

FORMATTING FUNCTIONS
=====================
--glPixelStore() lets openGL read & write pixel blocks in the
                correct format for YOUR computer's memory arrays.
  **CAREFUL! Don't assume default values for glPixelStore() are what you want!
  **DETAILS: glPixelStore() lets you change the settings openGL uses to pack or
    unpack blocks of pixels stored in your computer's memory (in 'client side'
    buffers).  It can accomodate almost any client memory requirements;
    for example, it lets you specify that that each scanline read/write begins
    on 8, 16, or 32 bit boundaries (nice for MS Windows); it lets you specify
    byte-order swaps (nice for Apple OS-X or any little-endian vs. big-endian
    mismatches:     http://en.wikipedia.org/wiki/Endianness.

--glPixelTransfer() changes colors, indices, maps, scales & biases.
  **CAREFUL! Pgms using only true-color images rarely need glPixelTransfer().
  **DETAILS: glPixelTransfer() lets you change openGL settings that enable it
    to modify or rearrange pixel values during pixel-block transfers between
    buffers.  For example, it could rearrange BGR-format pixels to RBG format
    as it transfers them from a client-side buffer (your computer's memory) to
    a texture buffer.  It can use color index tables, gamma correction lookup
    tables,change stencil bits and more.  All specified changes apply to all
    transfers, including those by glDrawPixels(), glReadPixels(),
    glCopyPixels(), and glTexImage() etc.

--'glPixelMap() stores maps and tables used by glPixelTransfer().
  **NOTE: If you don't need glPixelTransfer(), you don't need glPixelMap().
  **DETAILS:glPixelMap() lets to create the lookup tables used by
    glPixelTransfer(). These lookup tables change pixel values, such as
    gamma-correction tables applied to R,G,B,A values,
    or tables that set stencil values.

BUFFER SELECTION FUNCTIONS
==========================
The openGL 'Pixel-Block' manipulating functions give us, by default, access to
the openGL color buffer(s) used for drawing on-screen.
For single-buffered display, they access the GL_FRONT buffer by default;
for double-buffered display, they access the GL_BACK  buffer by default.
To change buffers, call
    --glReadBuffer() to set the source buffer for pixel-block reading,
    --glDrawBuffer() to set the destination buffer for pixel-block writing.

Available Color Buffer Choices:
--In addition to GL_FRONT and GL_BACK, you may find the GL_FRONT_AND_BACK
    choice useful for some interactive, double-buffered applications. After the
    function call glDrawBuffer(GL_FRONT_AND_BACK), pixel-block functions will
    write to BOTH the front and back buffers simultaneously.
--Double-buffered stereo display implemented in openGL gives you four usable
    color buffers: GL_RIGHT_FRONT, GL_LEFT_FRONT,GL_RIGHT_BACK,GL_LEFT_BACK.
--Better graphics hardware may provide 'auxiliary' color buffers (GL_AUX0,
    GL_AUX1, GL_AUX2, ... GL_AUXn) to hold partial renderings for tasks such as
    picking,shadows, transparency, and anti-aliasing.
--Texture Buffers: the long-awaited 'render-to-texture' functions that let you
    use your on-screen display image as a texture map.

PIXEL-MOVING FUNCTIONS
=======================
--glBitmap() unpacks a rectangle of ***1-bit*** values from your computer's
    memory (a client-side buffer) to the openGL display. The default 'unpacking'
    method usually works on all machines if you store sequential bits in
    sequential bytes in sequential rows ('row-major' order), starting with the
    bottom-most row and working upwards (e.g. origin at lower left).  Otherwise,
    use the glPixelStore() function to select other non-default formatting
    options for reading from your computer's memory.
  **DETAILS: glBitmap() draws the lower-left corner of the bitmap rectangle at
    the (transformed) GL_CURRENT_RASTER_POSITION vector value set by
    glRasterPos*() functions. (Note this position gets transformed bu
    GL_MODELVIEW and GL_PROJECTION before use).
    It draws the rectangles' contents on-screen (1 bit per pixel), and then
    changes the GL_CURRENT_RASTER_POSITION value by an amount you specify
    (often it's just (0,0)).
        OpenGL offers 16 different ways to 'draw' a bitmap on-screen, but the
    default 'overlay' method is very simple. Where the bitmap==0, leave pixel
    values unchanged; where the bitmap==1, replace the existing display color
    with openGL's current bitmap-drawing color (usually the glColor() value).
    SEE: http://www.opengl.org/sdk/docs/man/xhtml/glBitmap.xml

    -----Bitmap-Drawing Color:-----
        When you call 'glRasterPos*()' it saves the most recent glColor() value
    as the bitmap-drawing color; when you call glBitmap(), it uses this color
    and clears the bitmap-drawing color.  If you call you call glBitmap() while
    the bitmap-drawing color is cleared, it uses the current glColor() instead.

    -----Other Bitmap Drawing Ops-----
    To use any non-default drawing method, you must:
     a) enable it: call glEnable(GL_COLOR_LOGIC_OP);  (disabled by default)
     b) specify it: call glLogicOp() with the desired drawing method as argument.
        Besides the default GL_OVERLAY arg, try GL_COPY, GL_OR, or GL_XOR.
        See:   http://www.opengl.org/sdk/docs/man/xhtml/glLogicOp.xml

--glDrawPixels() 'unpacks' a rectangular block of pixels from your computer's
    memory (a client-side buffer) and copies them to the openGL display at the
    (transformed) CURRENT_RASTER_POSITION value set by glRasterPos().
    The default 'unpacking' method usually works on all machines if you store
    sequential pixel-components (RGB, RGBA, etc) in sequential pixels in
    sequential rows ('row-major' order), e.g. GLubyte buf[HEIGHT][WIDTH][COLOR];
    If not, see glPixelStore() to specify your machine's formatting.
    glDrawPixels() places the lower-left corner of the user's block of pixels
    on-screen at the (transformed) glRasterPos*() position (the position gets
    transformed by GL_MODELVIEW and GL_PROJECTION before use).
        SEE:   http://www.opengl.org/sdk/docs/man/xhtml/glDrawPixels.xml

--glReadPixels() selects a rectangular block of pixels from openGL's display
    buffer from absolute window position (x,y) and 'packs' them into a user-
    supplied memory buffer (a client-side buffer).  The default 'packing' method
    usually works for all machines if you read the result using sequential
    pixel-components (RGB, RGBA, etc), in sequential pixels in sequential rows
    ('row-major' order), e.g. GLubyte buf[HEIGHT][WIDTH][COLOR];
    If not, use the glPixelStore() fcn. to specify your machine's formatting.
    CAREFUL!  glReadPixels() does NOT use glRasterPos() position (screen position

--glCopyPixels() copies a rectangular block of pixels at the absolute window
    position (x,y) and PASTES (writes) them to the (transformed) glRasterPos()
    position in the same openGL buffer.  It needs no client-side memory buffer,
    and thus is MUCH faster than calling glReadPixels() then glDrawPixels().
*/


void CTheApp::printPixelStoreReport(void)
//------------------------------------------------------------------------------
// OpenGL gives you exquisitely flexible tools for manipulating blocks of pixels
// in any arrangement in memory on any machine, but it's default settings aren't
// always correct for all uses on all machines.  This function reports the
// current settings openGL will use to pack or unpack blocks of pixels you store
// in your computer's memory ( in 'client side' buffers, not in the GPU).
// To modify these settings, call glPixelStore(), explained here:
//          http://www.opengl.org/sdk/docs/man/xhtml/glPixelStore.xml
{
GLboolean isTrueA, isTrueB;
GLint  countA, countB;

    cout << "===================printPixelStoreTest()==================" <<endl;
    glGetBooleanv(  GL_PACK_SWAP_BYTES, &isTrueA);
    glGetBooleanv(GL_UNPACK_SWAP_BYTES, &isTrueB);
    cout << "      GL_PACK_SWAP_BYTES\t";
    if(isTrueA == true) cout << "TRUE\t";
                else    cout << " no \t";
    cout << "    GL_UNPACK_SWAP_BYTES\t";
    if(isTrueB == true) cout << "TRUE" << endl;
                else    cout << " no  " << endl;
    //------------
    glGetBooleanv(  GL_PACK_LSB_FIRST, &isTrueA);
    glGetBooleanv(GL_UNPACK_LSB_FIRST, &isTrueB);
    cout << "      GL_PACK_LSB_FIRST  \t";
    if(isTrueA == true) cout << "TRUE \t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_LSB_FIRST  \t";
    if(isTrueB == true) cout << "TRUE" << endl;
                else    cout << " no  " << endl;
    //------------
    glGetIntegerv(  GL_PACK_ROW_LENGTH, &countA);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &countB);
    cout << "      GL_PACK_ROW_LENGTH \t";
    if(countA != 0) cout << countA <<"\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_ROW_LENGTH\t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
    //------------
    // THIS NEXT VALUE ISN'T AVAILABLE.
//    glGetIntegerv(  GL_PACK_IMAGE_HEIGHT, &countA);
//    glGetIntegerv(GL_UNPACK_IMAGE_HEIGHT, &countB);
//    cout << "      GL_PACK_IMAGE_HEIGHT\t";
//    if(countA != 0) cout << countA << "\t";
//                else    cout << " no  \t";
//    cout << "    GL_UNPACK_IMAGE_HEIGHT\t";
//    if(countB != 0) cout << countB << endl;
//                else    cout << " no  " << endl;
   //------------
    glGetIntegerv(  GL_PACK_SKIP_PIXELS, &countA);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &countB);
    cout << "      GL_PACK_SKIP_PIXELS \t";
    if(countA != 0) cout << countA << "\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_SKIP_PIXELS\t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
    //------------
    glGetIntegerv(  GL_PACK_SKIP_ROWS, &countA);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &countB);
    cout << "      GL_PACK_SKIP_ROWS  \t";
    if(countA != 0) cout << countA << "\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_SKIP_ROWS  \t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
    //------------
//    glGetIntegerv(  GL_PACK_SKIP_IMAGES, &countA);
//    cout << "      GL_PACK_SKIP_IMAGES\t";
//    if(countA != 0) cout << countA << endl;
//                else    cout << " no " << endl;;
    //------------
    glGetIntegerv(  GL_PACK_ALIGNMENT, &countA);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &countB);
    cout << "      GL_PACK_ALIGNMENT  \t";
    if(countA != 0) cout << countA << "\t";
                else    cout << " no  \t";
    cout << "    GL_UNPACK_ALIGNMENT   \t";
    if(countB != 0) cout << countB << endl;
                else    cout << " no  " << endl;
}
