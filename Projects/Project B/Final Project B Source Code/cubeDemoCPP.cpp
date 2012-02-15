//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (JT: I prefer to keep line lengths <=80 chars for easy printing/display.
//
// EECS 351-1 'Starter Code' for Project B: Moving Views
//
#include "cubeDemoCPP.h"
#include <iostream>
using namespace std;

//=====================
// DISPLAY GLOBAL VARIABLES: bad idea, but forced by GLUT's callback fcns.
//                   Do your best to keep the number of global vars low;
//                   Make a single C++ object that holds entire program's state.
//=====================
static int nu_display_width = 640;	// global variables that hold current
static int nu_display_height = 640;	// display window position and size;
static int nu_display_xpos = 200;	// See getDisplayXXX() functions.
static int nu_display_ypos = 100;
static int nu_Anim_isOn = 1;        // ==1 to run animation, == 0 to pause.
                        // !DON'T MESS WITH nu_Anim_isOn; call runAnimTimer().
//========================
// Global vars for our application:
Ccube c;
GLdouble xclik = 0, yclik = 0;      // mouse button down position, in pixels
GLdouble xtheta = 0, ytheta = 0;    // mouse-driven rotation angles in degrees.

cube cube1 = cube(2);
cube cube2 = cube(4);
squarePrism sp1 = squarePrism(4);
squarePrism sp2 = squarePrism(5);
triangularPrism tp1 = triangularPrism(2);
triangularPrism tp2 = triangularPrism(3);
triangularPrism tp3 = triangularPrism(4);
CTheApp app;

int camChoice = 0, viewChoice = 1;                 // change using 'v' key  (1,2, or 3)
int rotationNumber = 0, rotationIncrement = 5;
int Htangle = 90, HtangleIncrement = 5;
int pictures = 0;
//int helpButton = 0;             //toggle help instructions button

double Xcamera = 0, Ycamera = 0, Zcamera = 0;
double aRotation = 0, aRotationationIncrement = 1;


int main( int argc, char *argv[] )
//==============================================================================
{
    //--GLUT/openGL Setup-------------------
	glutInit( &argc, argv );	    // init GLUT user-interface library;
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH  );
                    // set rendering mode to RGB (not RGBA) color,
					// single-buffering GLUT_SINGLE, or
					// double-buffering GLUT_DOUBLE, where you'd need to call
					// glutSwapBuffers() after drawing operations to show
					// on-screen what you draw)
					//  enable depth testing for 3D drawing ('Z-buffering')
	glutInitWindowSize(getDisplayWidth(), getDisplayHeight() );
    glutInitWindowPosition(getDisplayXpos(),getDisplayYpos() );
                    // set display window size & position from global vars
	glutCreateWindow( "EECS351 - Dhruv Koul - Project B" ); // set window title-bar name
	glClearColor( 0, 0, 0, 1 );	    // 'cleared-screen' color: black (0,0,0)

	// Register our callback functions in GLUT:
	glutDisplayFunc( myDisplay );	// callback for display window redraw
	glutReshapeFunc( myReshape);    // callback to create or re-size window
	glutVisibilityFunc(myHidden);   // callback for display window cover/uncover
	glutKeyboardFunc( myKeyboard);  // callback for alpha-numeric keystroke
	glutSpecialFunc ( myKeySpecial);// callback for all others: arrow keys,etc.
	glutMouseFunc( myMouseClik );   // callback for mouse button events
	glutMotionFunc( myMouseMove );  // callback for mouse dragging events

	//================================
    glEnable( GL_DEPTH_TEST );	    // enable hidden surface removal
	glDisable(GL_LIGHTING);         // (for our next project)
	//================================

    app.argvCopy = argv;          // make a copy of argv[] for later use by
    app.imgMax = 3;               // only need 3 images max
    app.imgCurrent = 0;           // currently-selected image#.

    //SELECT THE IMAGES TO PUT ON SCREEN
    for(int i = 0; i < app.imgMax; i++)
    {   // set initial positions for lower-left corner of each image
        app.imgXpos[i] = 5;
        app.imgYpos[i] = 5;
        app.imgZpos[i] = 0;     // (uses glRasterPos()...)
    }
    for(int i = 0; i < app.imgMax; i++)   // read in, prepare all images for display
    {
        app.myImg[i].setFileName(app.argvCopy, i); // i-th default filename
        app.myImg[i].readFile();
    }
    cerr << "error message" << endl;

    runAnimTimer(1);                // start our animation loop.
    camChoice = 0;                  // default:
	glutMainLoop();	                // enter GLUT's event-handler; NEVER EXITS.
}


void myReshape( int width, int height )
//------------------------------------------------------------------------------
{
// Called by GLUT on initial creation of the window, using the width,height
// arguments given in glutInitWindowSize(); see main() fcn. ALSO called by GLUT
// whenever you re-size the display window to a different width, height.
    nu_display_width    = width;    // save the current display size
    nu_display_height   = height;   // in these global vars.

//-------------------- glViewport() usage:
//
//   ?What coordinates for glVertex() calls will actually show up on-screen?
//
// BACKGROUND:
// OpenGL transforms glVertex() three times before they reach the screen, first
// --by the GL_MODELVIEW matrix (used to build 3D shapes and place your camera),
// --then the GL_PROJECTION matrix (used to make a 2D image from 3D vertices;
//      results fed into 'CVV'), and
// --then by the 'Viewport' matrix (used to fit the 2D image onto the screen).
//  !CHAOS!: ALL THREE matrices change a glVertex call's on-screen location,
//          and the windowing system may change display window size & location.
//
//      What goes where? How do we start?
//
// Fortunately, we can always rely on the CVV ('canonical view volume') as a
// our reference, our anchor for all the coordinate systems we chain together
// with these 3 matrices.
//
// The 'Canonical View Volume' is a simple axis-centered cube with corners at
// (+/-1, +/-1, +/-1), defined at the output of the GL_PROJECTION matrix.
// OpenGL 'clips' to this volume; it simply discards any part of any drawing
// primitive that comes out of the GL_PROJECTION matrix with values outside
// this CVV cube.  Any openGL drawing you see already passed through this CVV.
//
// Initially, GL_MODELVIEW and GL_PROJECTION both hold 4x4 indentity matrices,
// and (thanks to GLUT) the viewport fills your display window with the CVV.
// If your glVertex() calls specify (x,y,z) values within (+/-1, +/-1, +/-1)
// (inside the CVV) you can make on-screen pictures with them.  If you mess up,
// you can always return to this state by re-initializing the 3 matrices:
//          glMatrixMode(GL_MODELVIEW); // select and
//          glLoadIdentity();           // set to identity matrix.
//          glMatrixMode(GL_PROJECTION);// select and
//          glLoadIdentity();           // set to identity matrix.
//          glViewport(0,0,screenWidth,screenHeight); // fill screen with CVV.
//
// The glViewport() command specifies the mapping from CVV to display-window
// pixels, but its method is rather odd -- remember its arguments as:
//
//          glViewport(int xLL, int yLL, int CVVwidth, int CVVheight);
//
// The (xLL,yLL) arguments specify the display window pixel address for the
// CVV's (-1,-1) (lower-left) corner; any vertex that leaves the
// GL_PROJECTION matrix with value (-1,-1) will appear at (xLL,yLL) on-screen.
// The 'width' and 'height' specify how many display pixels the entire CVV would
// cover on-screen, and we see the portion that fits within the display window.
//
// The CVV is always square, but our display windows are not; remember that
// contents of the always-square CVV don't look their best if squashed or
// stretched to fill an entire non-square display window. Also, you're not
// required to fill every pixel of the display window with openGL drawings,
// and/or you don't have to show the entire CVV in your display window.  You'll
// get the best looking results if your glViewport() call keeps width=height.
// Try one of these (comment out all but one):
//-------------------------------
//  glViewport(0,0, width, width);	// viewport may extend past window TOP
//-------------------------------
//  glViewport(0,0,height,height);	// viewport may extent past window RIGHT
//-------------------------------
    // Draw CVV in the display window's largest, centered, inscribed square:
    if(width > height)
    {	// WIDE window:  height sets viewport size, and
        glViewport((width-height)/2, 0, height, height);

    }	// move LL corner rightwards to keep image centered in window.
    else
    {	// TALL window:  width sets viewport size, and
        glViewport(0,(height-width)/2, width, width);
        // move LL corner upwards to keep image centered in window
    }
//--------------------------------
// or invent your own; Perhaps make square CVV with a window fitted inside it?
// Or could you draw TWO viewports on-screen, side-by-side in the same window?

//==============================================================================
// SPECIFY OUR CAMERA in GL_PROJECTION:
    camChoice = 1;                  // re-set the camera choice to
    doCamChoice();                  // default: 3D orthographic
	glutPostRedisplay();			// request redraw--we changed window size!
}

void myDisplay( void )
//------------------------------------------------------------------------------
// Registered as our GLUT display callback function.
// GLUT calls us whenever we need to re-draw the screen.
// OPENGL HINTS:
{
    //images and bitmaps

    if (pictures == 1)
        app.imgMax = 3;
    else
        app.imgMax = 0;

    // Clear the screen (to change screen-clearing color, try glClearColor() )
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                                // clear the color and depth buffers. Now draw:

 	glMatrixMode(GL_MODELVIEW_MATRIX);     // select the modeling matrix stack
	glLoadIdentity();               // set to 'do nothing'.
    // Now we're in 'camera' coordinates.

//== VIEWING ==============================================================
    // Now that reshape() set up a camera matrix in 'GL_PROJECTION', let's call
    // the default coord. system in GL_MODELVIEW the 'camera' coord. system.
    // Its x,y axes line up with the display window, and we're looking down
    // the -z axis of the camera.
    //
    // Perhaps we should rename the GL_MODELVIEW matrix as 'GL_VIEW_MODEL', as
    // we make openGL transform calls for the 'VIEW' transformation first.
    // ------------------------------------------------------------
    // The 'VIEW' transform positions and aims the camera:
    // it convertices from camera drawing axes to world drawing axes.
    // ------------------------------------------------------------
    // In openGL we apply the 'VIEW' transform to the GL_MODELVIEW matrix first,
    // before anything else; before we draw world-space axes, before we draw any
    // features or models, before any traversal of a tree of transforms.
    // Previously, the 'world' drawing axes defined the root of our tree of
    // transforms, but now the 'view' transform demotes it to a sibling; now the
    // camera drawing axes become the root, and the 'view' transform convertices
    // camera drawing axes to world drawing axes.
    //
    // The 'view' transform at first seems quite odd and backwards. It begins in
    // the camera axes, in 'eyeball' drawing axes (GL_MODELVIEW at identity),
    // and changes them into world coordinates.  We start with identical
    // world and camera coord. systems, and then we
    //              ............................................
    //              "move the world out & away from our camera."
    //              ............................................
    // As right-handed 'eyeball' coordinates put our eye at the origin and draws
    // a picture looking down the -z axis, the 'view' transform moves the world
    // coord. system away from our face, in the -z direction:

    // VIEW METHOD 1-------------------------
    if(viewChoice==1)
     {
         glTranslated(0,0,-5.0);         // move world 4 units on camera's -z axis.
     }
    //---------------------------------------

    // We can then further improve our 'view' transformation with glTranslate(),
    // glRotate() and glScale() call to modify the position the world drawing
    // axes in front of your camera.  While gratifying, world-moving gets
    // awkward and counter-intuitive for most picture-making tasks.
    // Surely there is a better way?
    //
    // The 'VIEW' transformation seems strange and unnatural to us because we're
    // more familiar with the idea of measuring eye- and camera-positions using
    // world-drawing axes.  The openGL 'view' transform does the opposite; it
    // measures the position of world-drawing axes in eye- or camera axes.
    // We can use ordinary and familiar  openGL transforms (glRotate, glScale,
    // glTranslate) to build the 'VIEW' transform if we're willing to describe
    // world-axis positions measured in camera-axis coordinates, but if we
    // want to describe camera positions in world-axis coordinates, we have to
    // construct and INVERSE matrix.
    //
    // SIMPLE EXAMPLE:
    // All previously-explored openGL matrix-making functions (glTranslate(),
    // glRotate(), glScale(), etc) created new drawing axes by measurements
    // taken on the **OLD** drawing axes.  (e.g. glTranslate(2,3,4) makes new
    // drawing axes whose origin is located at 2,3,4 in the OLD drawing axes;
    // If we measured this same transformation using the NEW drawing axes, we'd
    // describe its inverse; the OLD drawing axes' origin is at (-2,-3,-4) in
    // the NEW drawing axes.

    // In class and in the lecture notes we learned how to construct a 'view'
    // transformation from a camera pose specified in the world coord. system:
    //   --VRP: camera 'View Reference Point(VRP)' or 'center of projection'
    //   --LookAt: a target point pierced by aiming camera's -z axis,
    //   --VUP: View UP Vector; 3D vector that sets camera image +y direction.
    // The gluLookAt() fcn computes an openGL view transform using these values;

	// Aim the camera: choose only one:
  // VIEW METHOD 2------------------------

    // or make your own...
    gluLookAt( 4.0, 0.0, 0.0,     //place camera at VRP location 0,0,4 and
               0.0, 0.0, 0.0,       // LookAt the origin point with cam, and
               0.0, 0.0, 1.0 );     // define the 'up' direction as world +y.

    glTranslated(0,0,-1.5);
    glViewport(0, 0, min(nu_display_width/2,nu_display_height/2), min(nu_display_width/2,nu_display_height/2));

    /*if (helpButton)
        printInstructions();
    else
        askForHelp();*/

    drawScene();
    glLoadIdentity();
    gluLookAt( 0.0, 0.0, 4.0,     //place camera at VRP location 0,0,4 and
               0.0, 0.0, 0.0,       // LookAt the origin point with cam, and
               0.0, 1.0, 0.0 );     // define the 'up' direction as world +y.

    glViewport(0, min(nu_display_width/2,nu_display_height/2), min(nu_display_width/2,nu_display_height/2), min(nu_display_width/2,nu_display_height/2));

    drawScene();
    glLoadIdentity();
    gluLookAt( 0.0, 6.5, 0.0,     //place camera at VRP location 0,0,4 and
               0.0, 0.0, 0.0,       // LookAt the origin point with cam, and
               0.0, 0.0, 1.0 );     // define the 'up' direction as world +y.
    glTranslated(0,0,-1.5);

    glViewport(min(nu_display_width/2,nu_display_height/2), 0, min(nu_display_width/2,nu_display_height/2), min(nu_display_width/2,nu_display_height/2));
    drawScene();

    glViewport(min(nu_display_width/2,nu_display_height/2), min(nu_display_width/2,nu_display_height/2),
               min(nu_display_width/2,nu_display_height/2), min(nu_display_width/2,nu_display_height/2));

    glLoadIdentity();
    gluLookAt(Xcamera, 4.0 + Ycamera, Zcamera,
                  Xcamera + 4 * sin(xtheta), Ycamera + 4.0 - 4 * cos(xtheta), 4 * sin(ytheta),
                  0.0, 0.0,1.0);

    glRotated(45.0 + xtheta, 1.0, 0.0, 0.0); // spin the world on x axis, then
    glRotated(45.0 + ytheta, 0.0, 1.0, 0.0); // on y axis.
    glTranslated(0,0,-1.0);

    drawScene();


	//===============DRAWING DONE.
	glFlush();	                // do any and all pending openGL rendering.
	glutSwapBuffers();			// For double-buffering: show what we drew.
}


void drawScene() {
//== MODELING =========================================================

////////////////////////////////////////////////////////////////////////////////////////////////

    aRotation = aRotation + aRotationationIncrement + 2;
//== MODELING =========================================================
//    glTranslated(0.0,0.0,2.0);

    //------------First, draw with NO modeling transformations: (world dwg axes)
    glColor3d(0.7, 0.7, 0.7);       // draw a gray ground-plane grid.
    drawPlane(50.0, 0.5);

    glScaled(0.3,0.3,0.3);
    cube1.draw();

    c.drawWireframe();
    c.drawPoints(1.3);

    glTranslated(0,0,7);
    drawAxes();
    glTranslated(0,0,-7);

    glPushMatrix();
        glTranslated(0,0,2);
        glRotated(rotationNumber,0,0,1);
        sp2.draw();

        glPushMatrix();
            glTranslated(0,1,0);
            glRotated(45-rotationNumber*2,0,0,1);
            glTranslated(0,0,1);
            glScaled(0.4, 0.4, 0.4);
        glPopMatrix();

        glPushMatrix();
            glTranslated(0,-1,0);
            glRotated(45-rotationNumber*2,0,0,1);
            glTranslated(0,0,1);
            glScaled(0.4, 0.4, 0.4);
            glPopMatrix();

        glPushMatrix();
            glTranslated(1,0,0);
            glRotated(45-rotationNumber*2,0,0,1);
            glTranslated(0,0,1);
            glScaled(0.4, 0.4, 0.4);
        glPopMatrix();

        glPushMatrix();
            glTranslated(-1,0,0);
            glRotated(45-rotationNumber*2,0,0,1);
            glTranslated(0,0,1);
            glScaled(0.4, 0.4, 0.4);
            glScaled(0.6, 0.6, 0.6);
        glPopMatrix();

        glTranslated(0,0,1);
        glRotated(135,0,0,1);
        tp3.draw();

        glPushMatrix();
            glTranslated(0,0,3);
            glTranslated(0,-1,0);
            glRotated(Htangle,1,0,0);
            glRotated(rotationNumber*2,0,0,1);
            drawAxes();
            tp3.draw();
        glPopMatrix();

        glTranslated(4.0, 3.0, 0.3);
        glRotated(95.0, 0.0, 1.0, 0.0);
        tp2.draw();

        glPushMatrix();
            glTranslated(0,0,3);
            glTranslated(0,1,0);
            glRotated(-Htangle,1,0,0);
            glRotated(-rotationNumber*2,1,0,0);
            drawAxes();
            tp3.draw();
        glPopMatrix();

        glTranslated(-4.0, -3.0, 0.3);
        glRotated(95.0, 0.0, 1.0, 0.0);
        tp2.draw();

        glPushMatrix();
            glTranslated(0,0,3);
            glTranslated(1,0,0);
            glRotated(Htangle,0,1,0);
            glRotated(rotationNumber*2,1,0,1);
            drawAxes();
            tp3.draw();
        glPopMatrix();

        glPushMatrix();
            glTranslated(0,0,3);
            glTranslated(-1,0,0);
            glRotated(-Htangle,0,1,0);
            glRotated(-rotationNumber*2,1,0,1);
            drawAxes();
            tp3.draw();
        glPopMatrix();

    glPopMatrix();

    glPushMatrix();
        glRotated(aRotation/10,0,0,1);
        glRotated(90, 1, 0, 0);
        glTranslated(6,0,1);
        glScaled(0.5,0.5,0.5);
        glutSolidCube(3.0);
        sp1.draw();
    glPopMatrix();

    glPushMatrix();
        glRotated(aRotation/10,0,0,1);
        glRotated(90, 1, 0, 0);
        glTranslated(0,4,1);
        glScaled(0.5,0.5,0.5);
        glutSolidCube(3.0);
        sp1.draw();
        glPopMatrix();

    glPushMatrix();
        glRotated(aRotation/10,0,1,0);
        glRotated(90, 1, 0, 0);
        glTranslated(-8,0,1);
        glScaled(0.5,0.5,0.5);
        glutSolidCube(3.0);
        sp1.draw();
    glPopMatrix();

    glPushMatrix();
        glRotated(aRotation/10,1,0,0);
        glRotated(90, 1, 0, 0);
        glTranslated(0,-5,1);
        glScaled(0.5,0.5,0.5);
        glutSolidCube(3.0);
        sp1.draw();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-5-sin(aRotation/100)/2,-5-sin(aRotation/100)/2,0);
        glScaled(0.5,0.5,0.5);
        cube2.draw();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-5-sin(aRotation/100)/2,5+sin(aRotation/100)/2,0);
        glScaled(0.5,0.5,0.5);
        cube2.draw();
    glPopMatrix();

    glPushMatrix();
        glTranslated(5+sin(aRotation/100)/2,-5-sin(aRotation/100)/2,0);
        glScaled(0.5,0.5,0.5);
        cube2.draw();
    glPopMatrix();

    tp2.draw();
    glTranslated(1.2, 0.8, 0.0);
    tp3.draw();

    glPushMatrix();
        glTranslated(5+sin(aRotation/100)/2,5+sin(aRotation/100)/2,0);
        glScaled(0.5,0.5,0.5);
        cube2.draw();
    glPopMatrix();
    //------------draw the cube WITH transformations:
	glPushMatrix();                 // save current matrix,
        glRotatef(170,1,-2,1);      // rotate 170 degrees around (1,2,1) axis:

                                    // uncomment one drawing method
   glPopMatrix();                  // discard the rotation matrix we made.

    //FOR LOOP FROM STARTER CODE
    //ADD BITMAPS/PICTURES CODE
    for(int i = 0; i < app.imgMax; i++)
    {
        glTranslated(0,0,-1);
        if (i == 0) {
        glRasterPos3d((app.imgXpos[i]+((double)rotationNumber)/100),
                      app.imgYpos[i],
                      app.imgZpos[i]);
        }
        else if (i == 1) {
        glRasterPos3d((app.imgXpos[i]+((double)rotationNumber)/100),
                      app.imgYpos[i]+((double)rotationNumber)/100,
                      app.imgZpos[i]);
        }
        else  {
        glRasterPos3d(app.imgXpos[i],
                      app.imgYpos[i]+((double)rotationNumber)/100,
                      app.imgZpos[i]);
        }
        app.myImg[i].drawPixelsGL();  // draw onscreen images from files
                                        // at current glRasterPos() position.
    }
}


void myKeyboard(unsigned char key, int xw, int yw)
//------------------------------------------------------------------------------
// GLUT CALLBACK: Don't call this function in your program--GLUT does it every
// time user presses an alphanumeric key on the keyboard.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left)

{
int xpos,ypos;  // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)

	switch(key)
	{
        case 'J':
        case 'j':
            Xcamera = Xcamera + 0.2*cos(xtheta);
            Ycamera = Ycamera + 0.2*sin(xtheta);
            break;
        case 'L':
        case 'l':
            Xcamera = Xcamera - 0.2*cos(xtheta);
            Ycamera = Ycamera - 0.2*sin(xtheta);
            break;
        case 'I':
        case 'i':
            Ycamera = Ycamera - 0.2*cos(xtheta);
            Xcamera = Xcamera + 0.2*sin(xtheta);
            break;
        case 'K':
        case 'k':
            Ycamera = Ycamera + 0.2*cos(xtheta);
            Xcamera = Xcamera - 0.2*sin(xtheta);
            break;
        case 'U':
        case 'u':
            Zcamera = Zcamera + 0.15;
            break;
		case 'D':
		case 'd':
            Zcamera = Zcamera - 0.15;
            break;
        case 'H':
        case 'h':
            cout << "\nHELP MENU FOR WACKY ROCKET"
            << "\nDrag the mouse in the top right quadrant to adjust how you look at the viewport"
            << "\nUse the J, L, I, K buttons to move (Left, Right, Up, Down) respectively in that viewport"
            << "\nUse U and D to move up and down respectively in the viewport"
            << "\nUse the left and right arrow keys to rotate the rocket and broken wing"
            << "\nUse the up and down arrow keys to move the wing components"
            << "\nUse Z to move between perspective views"
            << "\nUse B to get the pictures on screen"
            << "\nQ, ENTER or SPACE BAR will quit the program" << endl << endl;
            break;
        case 'r':
        case 'R':       // reset mouse rotations
            xtheta = ytheta = 0;
            break;
        case 'B':
        case 'b':                       // advance to next viewing choice:
            pictures = pictures ^ 1;
            break;
        case 'z':
        case 'Z':                       // advance to next camera choice:
            camChoice = (camChoice % 2) + 1;
            doCamChoice();
            break;
		case ' ':		// User pressed the spacebar.
		case 27:		// User pressed the 'Esc'  key...
		case 'Q':		// User pressed the 'Q' key...
		case 'q':
			exit(0);    // Done! quit the program.
			break;
        default:
            cout << "I don't respond to the %c key.\n" << key << endl;
            break;
	}
    glColor3d(1.0, 1.0, 1.0);// And white-colored text on-screen:
    drawText2D(helv18,-0.5, 0.0,"Ordinary Key.");
    //===============DRAWING DONE.
    glFlush();	            // do any and all pending openGL rendering.
    glutSwapBuffers();	    // For double-buffering: show what we drew.
}

void myKeySpecial(int key, int xw, int yw)
//------------------------------------------------------------------------------
// Called by GLUT when users press 'arrow' keys, 'Fcn' keys, or any other
// non-alphanumeric key.  Search glut.h to find other #define literals for
// special keys, such as GLUT_KEY_F1, GLUT_KEY_UP, etc.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left!)/
{
int xpos,ypos;      // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)
	switch(key)
	{
		case GLUT_KEY_UP:		// left arrow key
            Htangle -= HtangleIncrement;
            if (Htangle < 25)
                Htangle = 25;
            cout << "left-arrow key.\n";
			break;
		case GLUT_KEY_DOWN:	// right arrow key
            Htangle += HtangleIncrement;
            if (Htangle > 145)
                Htangle = 145;
            cout << "right-arrow key.\n";
			break;
		case GLUT_KEY_LEFT:		// dn arrow key
            rotationNumber -= rotationIncrement;
            rotationNumber %= 640 * 2;
            cout << "dn-arrow key.\n";
			break;
		case GLUT_KEY_RIGHT:		// up arrow key
            rotationNumber += rotationIncrement;
            rotationNumber %= 640 * 2;
            cout << "up-arrow key.\n";
			break;
		// SEARCH glut.h for more arrow key #define statements.
		default:
			cout << "Special key; integer code value"<< (int)key << "\n)";
			break;
	}

    //===============DRAWING DONE.
    glFlush();	        // do any and all pending openGL rendering.
    glutSwapBuffers();	// For double-buffering: show what we drew.
}

void myMouseClik(int buttonID, int upDown, int xw, int yw)
//-----------------------------------------------------------------------------
// Called by GLUT on mouse button click or un-click
// When user clicks a mouse button,  buttonID== 0 for left mouse button,
//                              		    (== 1 for middle mouse button?)
//	                                 		 == 2 for right mouse button;
//	                        upDown  == 0 if mouse button was pressed down,
//	                                == 1 if mouse button released.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left!)
{
int xpos,ypos;  // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)

    xclik = xpos;   // save most-recent click in global var
    yclik = ypos;

    cout << "click; buttonID=" << buttonID <<", upDown=" << upDown;
    cout << ", at xpos,ypos=(" << xpos <<"," << ypos << ")\n";
}

void myMouseMove(int xw, int yw)
//-----------------------------------------------------------------------------
// Called by GLUT when user moves mouse while pressing any mouse button.
//	xw,yw == mouse position in window-system pixels (origin at UPPER left)
{
int xpos,ypos;  // mouse position in coords with origin at lower left.

    xpos = xw;
    ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)

    ytheta += 0.3*(xpos-xclik)/50;     // change rot. angle by drag distance
    xtheta -= 0.3*(ypos-yclik)/50;     // (drag in X direction rotates on Y axis)
    xclik = xpos;                   // (drag in Y direction rotates on X axis)
    yclik = ypos;                   // update current mouse position.
    cout << ".";
}

void runAnimTimer(int isOn)
//-----------------------------------------------------------------------------
// Call runAnimTimer(0) to STOP animation;
//      runAnimTimer(1) to START or CONTINUE it.
{
    nu_Anim_isOn = isOn;   // Update global var to the most-recent state;
    if(isOn == 1)          // Start or continue animation?
    {   //YES. ANIMATE!   register a GLUT timer callback:
        glutTimerFunc(NU_PAUSE, myTimer, 1);
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
// Don't call this function yourself; use runAnimTimer() instead!
// runAnimTimer(1) causes GLUT to wait NU_PAUSE milliseconds (longer if busy)
// and then call this function with value==1.
// TRICKY: We allow users to choose very long NU_PAUSE times to see what happens
// in animation.  During GLUT's wait time, user events (keyboard, mouse, or
// window cover/uncover) may make runAnimTimer(0) calls intended to STOP
// animation.  GLUT can't cancel a previously-issued GLUT timer callback, so
// in the TimerCallback() function we check global variable nu_Anim_isON.
{
                                // GLUT finished waiting.
    if(value != nu_Anim_isOn)
        cout << "\n(Anim cancelled during GLUT wait)\n";
    if(nu_Anim_isOn==1)         // Go on with animation?
    {                           // YES.
        glutPostRedisplay();    // tell GLUT to re-draw the screen, and
        runAnimTimer(1);        // continue with the next time interval.
    }
                                // NO. Do nothing. No further animation.
}

void myHidden (int isVisible)
//------------------------------------------------------------------------------
// Called by GLUT when our on-screen window gets covered or un-covered. When
// called, it turns off animation and re-display when our display window is
// covered or stowed (not visible). SEE ALSO: runAnimTimer() and myTimer()
{
    if(isVisible == GLUT_VISIBLE)  runAnimTimer(1); // Visible! YES! Animate!
    else runAnimTimer(0);                           // No. Stop animating.
}

// Read the current size and position of the graphics display window
//------------------------------------------------------------------------------
int getDisplayHeight(void) { return nu_display_height; }
int getDisplayWidth(void)  { return nu_display_width; }
int getDisplayXpos(void)   { return nu_display_xpos; }
int getDisplayYpos(void)   { return nu_display_ypos; }


void drawText2D(void * pFont, double x0, double y0, const char* pString)
//------------------------------------------------------------------------------
//  Using the current color and specified font, write the text in 'pString' to
// the display window.  Place the lower left corner of the first line of text
//  at screen position x0,y0 in the current coordinate system.
//      Example:  DrawText2D(helv18, 0.3,0.4 ,"Hello!");
//
// Available fonts:
// helv10 (HELVETICA size 10)   rom10 (TIMES_ROMAN size 10)
// helv12 (HELVETICA size 12)   rom24 (TIMES_ROMAN size 24)
// helv18 (HELVETICA size 18)
{
int i, imax;					// character counters

	glRasterPos2d(x0, y0);		// set text's lower-left corner position
	imax = 1023;				// limit the number of chars we print.
	for(i=0; pString[i] != '\0' && i<imax; i++)	// for each char,
	{
		glutBitmapCharacter(pFont, pString[i]);
	}
}

void drawAxes(void)
//------------------------------------------------------------------------------
// Draw small +x,+y,+z axes in red, green, blue:
{
	glColor3d(1.0, 0.0, 0.0);	// red
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +x-axis
		glVertex3d(0.4,0.0,0.0);
	glEnd();
	glColor3d(0.0, 1.0, 0.0);	// green
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +y-axis
		glVertex3d(0.0,0.4,0.0);
	glEnd();
	glColor3d(0.0, 0.5, 1.0);	// bright blue
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +z-axis
		glVertex3d(0.0,0.0,0.4);
	glEnd();
}

void drawPlane(GLdouble siz, GLdouble xygap)
//------------------------------------------------------------------------------
// draw a grid of lines in the z=0 plane to cover an area spanning +/-siz,
// with grid spacing xygap between lines parallel to x and y axes.
{
int ij, ijmax;

    if(xygap <= 0.0)
    {
        xygap = 1.0;
        cout <<"\n\n!!! drawPlane() defaulted to xygap=1.0!!!\n\n" << endl;
    }
    ijmax = (int)(2.0*siz / xygap);    // how many lines to draw.

    // Draw lines parallel to x axis;
    glBegin(GL_LINES);
        for(ij = 0; ij <= ijmax; ij++)
        {
            glVertex3d(-siz, -siz + ij*xygap, 0.0);
            glVertex3d( siz, -siz + ij*xygap, 0.0);
        }
    glEnd();
    // Draw lines parallel to y axis;
    glBegin(GL_LINES);
        for(ij = 0; ij <= ijmax; ij++)
        {
            glVertex3d(-siz + ij*xygap, -siz, 0.0);
            glVertex3d(-siz + ij*xygap,  siz, 0.0);
        }
    glEnd();
}

void doCamChoice(void)
//------------------------------------------------------------------------------
// Switch to the openGL camera selected by global variable 'camChoice'.
// Called by myReshape() and myKeyboard() callbacks.
{
    glMatrixMode( GL_PROJECTION );	// select camera-setting matrix stack
	glLoadIdentity();	            // clear it: identity matrix.
    switch(camChoice)
    {
        case 0:         // Nothing--show CVV; orthographic (+/-1,+/-1,+/-1).
             cout <<"Cam 0: nothing--CVV orthographic +/-1, +/-1"<< endl;
           break;
        case 1:         // -----3D Orthographic projection
            glOrtho(-2.0, 2.0, -2.0, 2.0, 1.0, 1000.0);
            cout <<"Cam 1: 3D orthographic +/-2,+/-2"<< endl;
            break;
        case 2:         // -----3D Perspective projection method 1:
            gluPerspective( 70.0, 1.0,      // horiz. FOV(degrees), aspect ratio
                            1.0, 1000.0 );    // zNear, zFar clipping planes.
           cout <<"Cam 2: 3D gluPerspective, 70-degree, 1:1 aspect"<< endl;

            break;
        case 3:     	// -----3D Perspective projection method 2:
        	glFrustum(-0.5, 1.0,            // left, right,
                      -1.0, 0.5,            // bottom, top
                      1.0, 1000.0);           // znear, zFar
            cout <<"Cam 3: 3D glFrustum -1/+3, -3/+1"<< endl;

            break;
        default:
            cout << "unknown camera. Defaulted to 0: ident (CVV) cam"<< endl;
            break;
    }
    glMatrixMode(GL_MODELVIEW);        //return to modelview coords
}

//================================
//
//      Ccube class members
//
//================================

Ccube::Ccube()				// default constructor,
//------------------------------------------------------------------------------
{
}

Ccube::~Ccube()				// default destructor
//------------------------------------------------------------------------------
{
}

void Ccube::makecube( void )
//------------------------------------------------------------------------------
{
	// assign face colorsArray
	facecolor[0][0] = 1.0; facecolor[0][1] = 0.0; facecolor[0][2] = 0.0; //RED
	facecolor[1][0] = 0.0; facecolor[1][1] = 1.0; facecolor[1][2] = 0.0; //GREEN
	facecolor[2][0] = 0.0; facecolor[2][1] = 0.0; facecolor[2][2] = 1.0; //BLUE
	facecolor[3][0] = 0.0; facecolor[3][1] = 1.0; facecolor[3][2] = 1.0; //CYAN
	facecolor[4][0] = 1.0; facecolor[4][1] = 0.0; facecolor[4][2] = 1.0; //MAGENTA
	facecolor[5][0] = 1.0; facecolor[5][1] = 1.0; facecolor[5][2] = 0.0; //YELLOW

	// assign vertices
	vertex[0][0] = -1; vertex[0][1] = -1; vertex[0][2] = -1; // lower left back
	vertex[1][0] = -1; vertex[1][1] = -1; vertex[1][2] = 1;  // lower left front
	vertex[2][0] = 1;  vertex[2][1] = -1; vertex[2][2] = 1;  // lower right front
	vertex[3][0] = 1;  vertex[3][1] = -1; vertex[3][2] = -1; // lower right back
	vertex[4][0] = -1; vertex[4][1] = 1;  vertex[4][2] = -1; // upper left back
	vertex[5][0] = -1; vertex[5][1] = 1;  vertex[5][2] = 1;  // upper left front
	vertex[6][0] = 1;  vertex[6][1] = 1;  vertex[6][2] = 1;  // upper right front
	vertex[7][0] = 1;  vertex[7][1] = 1;  vertex[7][2] = -1; // upper right back

	// assign faces
	face[0][0] = 0; face[0][1] = 1; face[0][2] = 2; face[0][3] = 3; // base
	face[1][0] = 4; face[1][1] = 5; face[1][2] = 6; face[1][3] = 7; // top
	face[2][0] = 0; face[2][1] = 1; face[2][2] = 5; face[2][3] = 4; // left
	face[3][0] = 2; face[3][1] = 3; face[3][2] = 7; face[3][3] = 6; // right
	face[4][0] = 0; face[4][1] = 4; face[4][2] = 7; face[4][3] = 3; // back
	face[5][0] = 1; face[5][1] = 2; face[5][2] = 6; face[5][3] = 5; // front
}

void Ccube::drawSolid( void )
//------------------------------------------------------------------------------
// Use openGL calls to draw the cube as a solid object:
{
	int i, j;

	glBegin( GL_QUADS );	// start rendering the quads (faces of cube)
	// (? can you make a version that uses triangle-drawing GL_TRIANGLES?)
	for ( i = 0; i < NUM_CUBE_FACES; i++ ) { // for each face
		glColor3fv( facecolor[i] );	// set the face color
		for ( j = 0; j < NUM_VERTICES_PER_FACE; j++ )
		{
			glVertex3fv( vertex[face[i][j]] );	// make quad
		}
	}
	glEnd();
}

void Ccube::drawWireframe( void )
//------------------------------------------------------------------------------
// Use openGL calls to draw the cube as a wire-frame object:
{
int i, j;

	for ( i = 0; i < NUM_CUBE_FACES; i++ ) {  // for each face
		glColor3fv( facecolor[i] );	          // set the face color
	    glBegin( GL_LINE_LOOP);	// draw a closed loop of line segments:
                                // one for each FACE.
		    for ( j = 0; j < NUM_VERTICES_PER_FACE; j++ ) // draw line segments:
		    {
			    glVertex3fv( vertex[face[i][j]] );	// i-th point on the j-th face
		    }
		glEnd();
	}
}


void Ccube::drawPoints( int diam )
//------------------------------------------------------------------------------
{
GLfloat oldSize;
int i,j;

   glGetFloatv(GL_POINT_SIZE, &oldSize); // save current point size;
    glPointSize((GLfloat)diam);        // set OUR desired point size;
    glBegin( GL_POINTS);	           // Draw individual points at each vertex.
	for ( i = 0; i < NUM_CUBE_FACES; i++ ) { // for each face
		glColor3fv( facecolor[i] );	         // set the face color
		for ( j = 0; j < NUM_VERTICES_PER_FACE; j++ )
		{
			glVertex3fv( vertex[face[i][j]] );	     // each vertex of each face
		}
	}          // NOTE!  this is REDUNDANT! every vertex is part of 3 faces,
	           // so we're drawing each vertex 3 times....
	glEnd();
	glPointSize(oldSize);              // restore original size of openGL points

}

void printInstructions()
{
    glColor3d(1.0, 1.0, 1.0);
    drawText2D(helv12, -0.5, -0.9, "Click and drag mouse in top right viewport to rotate view");
    drawText2D(helv12, -0.5, -1.1, "Press Z to change camera views.");
    drawText2D(helv12, -0.5, -1.3, "Press R to change the movable viewport into default camera view.");
    drawText2D(helv12, -0.5, -1.5, "Press LEFT and RIGHT arrow keys to make things go wacky");
    drawText2D(helv12, -0.5, -1.7, "(Press the 'Space bar', ESC, 'Q' or 'q' to quit!)");
}

void askForHelp()
{
    glColor3d(1.0, 1.0, 1.0);
    drawText2D(helv18,-1.5,-1.7, "Press 'H' for instructions on how to run the program.");
}


//HELP FROM KHALID AZIZ
triangularPrism::triangularPrism(int height) {

vertices[0] = 1; vertices[1] = 0; vertices[2] = height;
vertices[3] = -1; vertices[4] = 0; vertices[5] = height;
vertices[6] = 0; vertices[7] = 1; vertices[8] = height;
vertices[9] = 0; vertices[10] = -1; vertices[11] = height;
vertices[12] = 0; vertices[13] = 0; vertices[14] = 0;


colorsArray[0] = 1; colorsArray[1] = 1; colorsArray[2] = 1;
colorsArray[3] = 0; colorsArray[4] = 0; colorsArray[5] = 1;
colorsArray[6] = 1; colorsArray[7] = 1; colorsArray[8] = 1;
colorsArray[9] = 0; colorsArray[10] = 0; colorsArray[11] = 1;
colorsArray[12] = 0; colorsArray[13] = 1; colorsArray[14] = 0;

indexes[0] = 0; indexes[1] = 2; indexes[2] = 1;
indexes[3] = 3; indexes[4] = 0; indexes[5] = 4;
indexes[6] = 2; indexes[7] = 1; indexes[8] = 4;
indexes[9] = 3; indexes[10] = 0;

}

void triangularPrism::draw() {

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // activate and specify pointer to vertex array
    glVertexPointer(3, GL_INT, 0, &vertices);
    glColorPointer(3, GL_FLOAT, 0, &colorsArray);

    // draw a cube
    glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_INT, &indexes);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

cube::cube(int height) {


vertices[0] = 0; vertices[1] = -1; vertices[2] = 0;
vertices[3] = 1; vertices[4] = 0; vertices[5] = 0;
vertices[6] = 0; vertices[7] = -1; vertices[8] = height;
vertices[9] = 1; vertices[10] = 0; vertices[11] = height;
vertices[12] = -1; vertices[13] = 0; vertices[14] = 0;
vertices[15] = 0; vertices[16] = 1; vertices[17] = 0;
vertices[18] = -1; vertices[19] = 0; vertices[20] = height;
vertices[21] = 0; vertices[22] = 1; vertices[23] = height;

colorsArray[0] = 1; colorsArray[1] = 1; colorsArray[2] = 1;
colorsArray[3] = 1; colorsArray[4] = 0; colorsArray[5] = 1;
colorsArray[6] = 1; colorsArray[7] = 1; colorsArray[8] = 0;
colorsArray[9] = 0; colorsArray[10] = 1; colorsArray[11] = 1;
colorsArray[12] = 0; colorsArray[13] = 0; colorsArray[14] = 0;
colorsArray[15] = 0; colorsArray[16] = 0; colorsArray[17] = 1;
colorsArray[18] = 0; colorsArray[19] = 0; colorsArray[20] = 1;
colorsArray[21] = 1; colorsArray[22] = 0; colorsArray[23] = 0;

indexes[0] = 0; indexes[0] = 1; indexes[0] = 2;
indexes[0] = 3; indexes[0] = 6; indexes[0] = 7;
indexes[0] = 4; indexes[0] = 5; indexes[0] = 6;
indexes[0] = 2; indexes[0] = 4; indexes[0] = 0;
indexes[0] = 5; indexes[0] = 1; indexes[0] = 7;
indexes[0] = 3;
}

void cube::draw() {

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);


    // activate and specify pointer to vertex array
    glVertexPointer(3, GL_INT, 0, &vertices);
    glColorPointer(3, GL_FLOAT, 0, &colorsArray);

    // draw a cube
    glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_INT, &indexes);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


squarePrism::squarePrism(int height) {

vertices[0] = 1; vertices[1] = 0; vertices[2] = 0;
vertices[3] = 0; vertices[4] = 1; vertices[5] = 0;
vertices[6] = -1; vertices[7] = 0; vertices[8] = 0;
vertices[9] = 0; vertices[10] = -1; vertices[11] = 0;
vertices[12] = 0; vertices[13] = 0; vertices[14] = height;
vertices[15] = 0; vertices[16] = 0; vertices[17] = -height;

colorsArray[0] = 1; colorsArray[1] = 1; colorsArray[2] = 1;
colorsArray[3] = 1; colorsArray[4] = 0; colorsArray[5] = 1;
colorsArray[6] = 0; colorsArray[7] = 0; colorsArray[8] = 1;
colorsArray[9] = 1; colorsArray[10] = 1; colorsArray[11] = 0;
colorsArray[12] = 1; colorsArray[13] = 1; colorsArray[14] = 0;
colorsArray[15] = 0; colorsArray[16] = 0; colorsArray[17] = 0;

indexes[0] = 5; indexes[1] = 3; indexes[2] = 0;
indexes[3] = 4; indexes[4] = 1; indexes[5] = 0;
indexes[6] = 5; indexes[7] = 2; indexes[8] = 1;
indexes[9] = 4; indexes[10] = 3; indexes[11] = 2;
indexes[12] = 4;
}

void squarePrism::draw() {

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);


    // activate and specify pointer to vertex array
    glVertexPointer(3, GL_INT, 0, &vertices);
    glColorPointer(3, GL_FLOAT, 0, &colorsArray);

    // draw a cube
    glDrawElements(GL_TRIANGLE_STRIP, 13, GL_UNSIGNED_INT, &indexes);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

///////FROM PROFESSOR TUMBLIN'S STARTER CODE
CTheApp::CTheApp(void)
//------------------------------------------------------------------------------
// default constructor
{

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
