//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (JT: I prefer to keep line lengths <=80 chars for easy printing/display.
//
// EECS 351-1 'Starter Code' for Project B: Moving Views
//
#include "cubeDemoCPP.h"
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
static int nu_Anim_isOn = 1;        // ==1 to run animation, ==0 to pause.
                        // !DON'T MESS WITH nu_Anim_isOn; call runAnimTimer().
//========================
// Global vars for our application:
GLdouble xclik=0, yclik=0;      // mouse button down position, in pixels
GLdouble xtheta=0, ytheta=0;    // mouse-driven rotation angles in degrees.
int camChoice=1,                  // change using 'z' key (0,1,2, or 3)
    fixedCamChoice = 1;
int viewChoice=1;                 // change using 'v' key  (1,2, or 3)
double thetastep=.5,
        thetastep1=.2,
        thetastep2=.3,
        headtheta=0,
        bodytheta=0,
        bodytheta2=0,
        bodytheta3=0,
        thetastep3=.4,
        xmove=0,
        ymove=0,
        zmove=0;
bool inflated=0;
GLdouble *pTriVerts,                //For GL_TRIANGLE dyn. alloc'd arrays
         *pTriColrs,                // for 3 vertices and 3 colors
         *pCylVerts,
         *pCylColrs;

GLdouble *pStripVerts,              // For GL_TRIANGLE_STRIP dyn. alloc'd arrays
         *pStripColrs,              // for 2*(halfCount+1) vertices,their colors
         *pLineColrs;               // and outline colors (line-strip)

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
	glutCreateWindow( "Cassie's Project B" ); // set window title-bar name
	   cout<< "You can rotate the world with the mouse and move in and out using the arrows keys and 'b' or 'n'. Press 'i' to inflate the dancer and 'd' to deflate him. Enjoy! \n";
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
    makeCrown(.5,.25);
    makeCylinder(.5,.25);

	//================================
    runAnimTimer(1);                // start our animation loop.
    camChoice = 1;                  // default:
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


    //glViewport(0,0,width/2,height/2);

//--------------------------------
// or invent your own; Perhaps make square CVV with a window fitted inside it?
// Or could you draw TWO viewports on-screen, side-by-side in the same window?

//==============================================================================
// SPECIFY OUR CAMERA in GL_PROJECTION:
    //camChoice = 1;                  // re-set the camera choice to
    //doCamChoice();                  // default: 3D orthographic
	//glutPostRedisplay();			// request redraw--we changed window size!
}

void myDisplay( void )
//------------------------------------------------------------------------------
// Registered as our GLUT display callback function.
// GLUT calls us whenever we need to re-draw the screen.
{
    // Clear the screen (to change screen-clearing color, try glClearColor() )
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                                // clear the color and depth buffers. Now draw:

 	glMatrixMode(GL_MODELVIEW);     // select the modeling matrix stack
	glLoadIdentity();               // set to 'do nothing'.
    // Now we're in 'camera' coordinates.

    // VIEW METHOD 1-------------------------
    if(viewChoice==1)
     {
         glTranslated(0,0,-4.0);         // move world 4 units on camera's -z axis.
     }
    //---------------------------------------

  // VIEW METHOD 2------------------------
    if(viewChoice==2)
	{
        gluLookAt( 2.0, 2.0, 2.0,       //place camera at VRP location 2,2,2 and
                   0.0, 0.0, 0.0,       // LookAt the world-space origin pt., and
                   0.0, 1.0, 0.0 );	    // define the 'up' direction world +y.
	}
    // VIEW METHOD 3------------------------
    if(viewChoice==3)
    {
        gluLookAt( 0.0, 0.0, 4.0,     //place camera at VRP location 0,0,4 and
                   0.0, 0.0, 0.0,       // LookAt the origin point with cam, and
                   0.0, 1.0, 0.0 );     // define the 'up' direction as world +y.
    }
    //-------------------------------------
    // or make your own...

    // print on-screen text in these un-rotated world coordinates,

    // and then 'spin the world' around it's newly displaced origin point
    // to look at it from a different angle:
//== MODELING =========================================================
    drawAxes();
    if(inflated) {
        headtheta += thetastep;
        if( (headtheta >  15 && thetastep >0) ||
        (headtheta < -15 && thetastep <0)) thetastep = -thetastep;
        bodytheta += thetastep1;
        if( (bodytheta>30 && bodytheta >0) ||
        (bodytheta<-30 && bodytheta <0)) thetastep1 = -thetastep1;
        bodytheta2 += thetastep2;
        if( (bodytheta2>50 && bodytheta2 >0) ||
        (bodytheta2<-50 && bodytheta2 <0)) thetastep2 = -thetastep2;
        bodytheta3 += thetastep3;
        if( (bodytheta3>60 && bodytheta3 >0) ||
        (bodytheta3<-60 && bodytheta3 <0)) thetastep3 = -thetastep3;
    }

    //Bottom Left View
    glViewport(0,0,nu_display_width/2,nu_display_height/2);
        glPushMatrix();
        fixedCamChoice = 1;
        doCamChoice(fixedCamChoice);
        glColor3d(0.7, 0.7, 0.7);       // draw a gray ground-plane grid.
        drawGndPlane(50.0, 0.5);
        glColor3d(.3294,0,.6667);       //draw a purple crazy guy
        crazyguy();
        RandomObjects();
        glPopMatrix();

    //Bottom Right View
    glViewport(nu_display_width/2,0,nu_display_width/2,nu_display_height/2);
        glPushMatrix();
        fixedCamChoice = 1;
        glRotated(90,0,1,0); //View from side
        doCamChoice(fixedCamChoice);
        glColor3d(0.7, 0.7, 0.7);       // draw a gray ground-plane grid.
        drawGndPlane(50.0, 0.5);
        glColor3d(.3294,0,.6667);       //draw a purple crazy guy
        crazyguy();
        RandomObjects();
        glPopMatrix();

    //Top Left View
    glViewport(0,nu_display_height/2,nu_display_width/2,nu_display_height/2);
        glPushMatrix();
            fixedCamChoice = 1;
            glRotated(90,1,0,0); //View from Top
            doCamChoice(fixedCamChoice);
            glColor3d(0.7, 0.7, 0.7);       // draw a gray ground-plane grid.
            drawGndPlane(50.0, 0.5);
            glColor3d(.3294,0,.6667);       //draw a purple crazy guy
            crazyguy();
            RandomObjects();
            glutPostRedisplay();			// request redraw--we changed window size!
        glPopMatrix();

    //Top Right View - Moving
    glViewport(nu_display_width/2,nu_display_height/2,nu_display_width/2,nu_display_height/2);
        glRotated(45.0+xtheta, 1.0, 0.0, 0.0); // spin the world on x axis, then
        glRotated(45.0+ytheta, 0.0, 1.0, 0.0); // on y axis.
        glTranslated(xmove,ymove,zmove);
        doCamChoice(camChoice);
        glColor3d(0.7, 0.7, 0.7);       // draw a gray ground-plane grid.
        drawGndPlane(50.0, 0.5);
        glColor3d(.3294,0,.6667);       //draw a purple crazy guy
        crazyguy();
        RandomObjects();
        glutPostRedisplay();			// request redraw--we changed window size!

	//===============DRAWING DONE.
	glFlush();	                // do any and all pending openGL rendering.
	glutSwapBuffers();			// For double-buffering: show what we drew.
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
		case 'D':       // User pressed the 'A' key...
		case 'd':
            inflated =0;
            break;
        case 'r':
        case 'R':       // reset mouse rotations
            xtheta = ytheta = 0;
            break;
        case 'B':
        case 'b':
            zmove=zmove-.01;
            break;
        case 'N':
        case 'n':
            zmove=zmove+.01;
            break;
        case 'z':
        case 'Z':                       // Change to orthographic camera
            cout<< "Your were looking at cam " << camChoice <<"/n";
            if(camChoice==1)camChoice= 2;       // stay within 0,1,2,3
            else if(camChoice==2)camChoice= 1;
            doCamChoice(camChoice);
            cout<< "Your now looking at cam " << camChoice <<"/n";
            break;
        case 'I':
        case'i':
            inflated = 1;
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
		case GLUT_KEY_LEFT:		// left arrow key
            xmove=xmove-.01;
			break;
		case GLUT_KEY_RIGHT:	// right arrow key
            xmove=xmove+.01;;
			break;
		case GLUT_KEY_DOWN:		// dn arrow key
            ymove = ymove-.01;
			break;
		case GLUT_KEY_UP:		// up arrow key
            ymove = ymove+.01;
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

    ytheta += 0.3*(xpos-xclik);     // change rot. angle by drag distance
    xtheta -= 0.3*(ypos-yclik);     // (drag in X direction rotates on Y axis)
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
		glVertex3d(0.8,0.0,0.0);
	glEnd();
	glColor3d(0.0, 1.0, 0.0);	// green
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +y-axis
		glVertex3d(0.0,0.8,0.0);
	glEnd();
	glColor3d(0.0, 0.5, 1.0);	// bright blue
	glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);	// +z-axis
		glVertex3d(0.0,0.0,0.8);
	glEnd();
}

void drawGndPlane(GLdouble siz, GLdouble xygap)
//------------------------------------------------------------------------------
// draw a grid of lines in the z=0 plane to cover an area spanning +/-siz,
// with grid spacing xygap between lines parallel to x and y axes.
{
int ij, ijmax;

    if(xygap <= 0.0)
    {
        xygap = 1.0;
        cout <<"\n\n!!! drawGndPlane() defaulted to xygap=1.0!!!\n\n" << endl;
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

void doCamChoice(int camChoice)
//------------------------------------------------------------------------------
// Switch to the openGL camera selected by global variable 'camChoice'.
// Called by myReshape() and myKeyboard() callbacks.
{
    glMatrixMode( GL_PROJECTION );	// select camera-setting matrix stack
	glLoadIdentity();	            // clear it: identity matrix.
    switch(camChoice)
    {
        case 0:         // Nothing--show CVV; orthographic (+/-1,+/-1,+/-1).
           break;
        case 1:         // -----3D Orthographic projection
            if(nu_display_width>nu_display_height) {
            glOrtho(-3.0, 3.0,              // left, right
                    -2.0, 2.0,              // bottom, top
                    1.0, 1000.0);            // zNear, zFar
            }
            else if(nu_display_width<nu_display_height) {
            glOrtho(-2.0, 2.0,              // left, right
                    -3.0, 3.0,              // bottom, top
                    1.0, 1000.0);            // zNear, zFar
            }
            else if(nu_display_width==nu_display_height) {
            glOrtho(-2.0, 2.0,              // left, right
                    -2.0, 2.0,              // bottom, top
                    1.0, 1000.0);            // zNear, zFar
            }
            break;
        case 2:         // -----3D Perspective projection method 1:
            gluPerspective( 70.0, 1.0,      // horiz. FOV(degrees), aspect ratio
                            1.0, 1000.0 );    // zNear, zFar clipping planes.
            break;
        case 3:     	// -----3D Perspective projection method 2:
        	glFrustum(-0.5, 1.0,            // left, right,
                      -1.0, 0.5,            // bottom, top
                      1.0, 1000.0);           // znear, zFar
            break;
        default:
            cout << "unknown camera. Defaulted to 0: ident (CVV) cam"<< endl;
            break;
    }
    glMatrixMode(GL_MODELVIEW);        //return to modelview coords
}


void crazyguybody() {
glRotated(180,0,0,1);
glTranslated(0,2.2,0); //to bottom of cone
        glRotated(bodytheta,1,0,1*sin(bodytheta*.01));
        drawAxes();
glPushMatrix();
        totalCylinder(2,.25); //torso
        glTranslated(0,-2.0,0); //tip of cone inside head
        totalCone(.2,.25);
glPopMatrix();

glTranslated(0,-2.1,0); //tip of cone inside head
            glRotated(bodytheta2,1,0,0);
            drawAxes();
glPushMatrix();
        totalCylinder(.6,.25); //torso
        glTranslated(0,-.6,0); //tip of cone inside head
        totalCone(.2,.25);
glPopMatrix();


glPushMatrix();
        glTranslated(0,-.7,0);
            glRotated(bodytheta3,1,0,0);
                drawAxes();
        totalCylinder(.4,.25); //head
        glTranslated(0,-.4,0);
            glRotated(90,1,0,0);
        drawCrown(); //crown
glPopMatrix();
}

void crazyguyarms() {
//Arms
 glRotated(180,1,0,0);
    glPushMatrix();
        glTranslated(.2,.4,0);
        if(inflated) {
         glRotated(290,0,0,1);
        }
        else {
        glRotated(195,0,0,1);
        }
        glRotated(headtheta,0,0,1);
        totalCylinder(.4,.08);
        glTranslated(0,.4,0);
        glRotated(headtheta,0,0,1);
        totalCylinder(.4,.08);
        glTranslated(0,.4,0);
        glRotated(headtheta,0,0,1);
        totalCylinder(.4,.08);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-.2,.4,0);
        if(inflated) {
         glRotated(40,0,0,1);
        }
        else {
        glRotated(140,0,0,1);
        }
        glRotated(headtheta,0,0,1);
        totalCylinder(.4,.08);
        glTranslated(0,.4,0);
        glRotated(headtheta,0,0,1);
        totalCylinder(.4,.08);
        glTranslated(0,.4,0);
        glRotated(headtheta,0,0,1);
        totalCylinder(.4,.08);
    glPopMatrix();
}

void crazyguy(void) {
    glPushMatrix();
        crazyguybody();
        crazyguyarms();
    glPopMatrix();
}

void drawTire(void)
//------------------------------------------------------------------------------
// Simple example of the ***OLD*** way to draw a GL_TRIANGLE_STRIP primitive;
// creates 2*halfCount triangles by generating a 'stair-step' vertex sequence.
//
// WARNING:  Don't use this method -- deprecated!  don't build a bad habit!
{
int i, imax;
    glPushMatrix();
    glScaled(.5,.5,.5);
    imax = 64;         // simplifies our 'for' loop...
    glBegin(GL_TRIANGLE_STRIP);
    for(i=0; i< imax; i++)
    {
        // A 'stairstep' of triangles from (-1,-1) to (+1,+1) across CVV:
		glColor3d(.545,.525,.5098);   // gray
        glVertex3f(sin(i),cos(i),0);
        glColor3d(1,.843,0);
        glVertex3f(sin(i+1),cos(i+1),0);
		glColor3d( 183/255,183/255,183/255);	// black
        glVertex3f(sin(i),cos(i),1);   // incremented x; same y.
    }
    glEnd();
    glPopMatrix();
}

void TirePile(void) {
    glPushMatrix();
        glTranslated(1.2,-1.2,0);
            drawTire();
        glTranslated(1,0,0);
            drawTire();
        glTranslated(-.5,.8,0);
            drawTire();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1.2,-1.2,0);
                drawTire();
        glTranslated(0,1,0);
                drawTire();
        glTranslated(0,1,0);
                drawTire();
    glPopMatrix();
}


void drawCrown(void)
//------------------------------------------------------------------------------
{
    glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pTriVerts); // --*pointer-- address of array's element [0].
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pTriColrs); // --*pointer--address of array's element [0]. */


        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLE_STRIP,      //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     66);                //--count-- and draw this many elements.
}

void makeCrown (double height,double radius)
//------------------------------------------------------------------------------
{
int i;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // Dynamic allocation for 3 triangle vertices.............................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    pTriVerts = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));
    if(pTriVerts==NULL) cout << "\n !Failed to malloc pTriVert!\n" << endl;
    // vertex counter.
    //              X coord;                Y coord;                Z coord.
    for (i=0;i<66;i++) {
    pTriVerts[3*i] = radius*sin(i*.1); pTriVerts[2 +3*i] = 0; pTriVerts[1 +3*i] = radius*cos(i*.1);
    i++;    // go on to next vertex;
    pTriVerts[3*i] =  radius*sin(i*.1); pTriVerts[2 +3*i] = 0.0; pTriVerts[1 +3*i] = radius*cos(i*.1);
    i++;
    pTriVerts[3*i] = radius*(sin((i-2)*.1)+sin((i-1)*.1))/2; pTriVerts[2 +3*i] = height; pTriVerts[1 +3*i] = radius*(cos((i-2)*.1)+cos((i-1)*.1))/2;
    }
    // Dyn. allocation for three colors.........................................
    pTriColrs = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));  // 3 colors
    if(pTriColrs ==NULL) cout << "\n !Failed to malloc pTriColrs!\n" << endl;

    // color counter. // each color has 3 parts: 0 <= (R,G,B) <= 1.0
    for(i=0;i<66;i++) {
    //             Red value,           Green value,            Blue value.
    pTriColrs[   3*i] = 0.0;
    pTriColrs[1 +3*i] = 1.0;
    pTriColrs[2 +3*i] = 0.0; // Cyan
    i++;    // color number;    // set bright Magenta pen color;
    pTriColrs[    3*i] = 1.0;
    pTriColrs[1 + 3*i] = 0.0;
    pTriColrs[2 + 3*i] = 1.0; // Magenta
    i++;    // color number;    // set bright Yellow pen color;
    pTriColrs[    3*i] = 1.0;
    pTriColrs[1 + 3*i] = 1.0;
    pTriColrs[2 + 3*i] = 0.0; // Yellow
    }
}

void drawCylinder(void)
//------------------------------------------------------------------------------
{
    glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pCylVerts); // --*pointer-- address of array's element [0].
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pCylColrs); // --*pointer--address of array's element [0]. */


        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLE_STRIP,      //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     66);                //--count-- and draw this many elements.
}
void totalCylinder(double height,double radius) {
    glPushMatrix();
        glRotated(90,1,0,0);
        makeCircle(height,radius);
        drawCircle();
        makeCylinder(height,radius);
        drawCylinder();
    glRotated(180,1,0,0);
        glTranslated(0,0,-height);
        drawCylinder();
        makeCircle(height,radius);
        drawCircle();
    glPopMatrix();
}

void makeCylinder (double height,double radius)
//------------------------------------------------------------------------------
{
int i;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // Dynamic allocation for 3 triangle vertices.............................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    pCylVerts = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));
    if(pCylVerts==NULL) cout << "\n !Failed to malloc pTriVert!\n" << endl;
    // vertex counter.
    //              X coord;                Y coord;                Z coord.
    for (i=0;i<66;i++) {
    pCylVerts[3*i] = radius*sin(i*.1); pCylVerts[2 +3*i] = 0; pCylVerts[1 +3*i] = radius*cos(i*.1);
    i++;    // go on to next vertex;
    pCylVerts[3*i] =  radius*sin(i*.1); pCylVerts[2 +3*i] = 0.0; pCylVerts[1 +3*i] = radius*cos(i*.1);
    i++;
    pCylVerts[3*i] = radius*(sin((i-2)*.1)+sin((i-1)*.1))/2; pCylVerts[2 +3*i] = height; pCylVerts[1 +3*i] = radius*(cos((i-2)*.1)+cos((i-1)*.1))/2;
    }
    // Dyn. allocation for three colors.........................................
    pCylColrs = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));  // 3 colors
    if(pCylColrs ==NULL) cout << "\n !Failed to malloc pTriColrs!\n" << endl;

    // color counter. // each color has 3 parts: 0 <= (R,G,B) <= 1.0
    for(i=0;i<66;i++) {
    //             Red value,           Green value,            Blue value.
    pCylColrs[   3*i] = 0.0;
    pCylColrs[1 +3*i] = 1.0;
    pCylColrs[2 +3*i] = 1.0; // Cyan
    i++;    // color number;    // set bright Magenta pen color;
    pCylColrs[    3*i] = 1.0;
    pCylColrs[1 + 3*i] = 0.0;
    pCylColrs[2 + 3*i] = 1.0; // Magenta
    i++;    // color number;    // set bright Yellow pen color;
    pCylColrs[    3*i] = 1.0;
    pCylColrs[1 + 3*i] = 1.0;
    pCylColrs[2 + 3*i] = 0.0; // Yellow
    }
}

void drawCircle(void)
{
glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pCylVerts); // --*pointer-- address of array's element [0].
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pCylColrs); // --*pointer--address of array's element [0]. */


        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLE_STRIP,      //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     66);                //--count-- and draw this many elements.

}

void makeCircle (double height, double radius)
{
    int i;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // Dynamic allocation for 3 triangle vertices.............................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    pCylVerts = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));
    if(pCylVerts==NULL) cout << "\n !Failed to malloc pTriVert!\n" << endl;
    // vertex counter.
    //              X coord;                Y coord;                Z coord.
    for (i=0;i<66;i++) {
    pCylVerts[3*i] = radius*sin(i*.1); pCylVerts[2 +3*i] = 0; pCylVerts[1 +3*i] = radius*cos(i*.1);
    i++;    // go on to next vertex;
    pCylVerts[3*i] =  radius*sin(i*.1); pCylVerts[2 +3*i] = 0; pCylVerts[1 +3*i] = radius*cos(i*.1);
    i++;
    pCylVerts[3*i] = 0; pCylVerts[2 +3*i] = 0; pCylVerts[1 +3*i] = 0;
    }
    // Dyn. allocation for three colors.........................................
    pCylColrs = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));  // 3 colors
    if(pCylColrs ==NULL) cout << "\n !Failed to malloc pTriColrs!\n" << endl;

    // color counter. // each color has 3 parts: 0 <= (R,G,B) <= 1.0
    for(i=0;i<66;i++) {
    //             Red value,           Green value,            Blue value.
    pCylColrs[   3*i] = 0.0;
    pCylColrs[1 +3*i] = 1.0;
    pCylColrs[2 +3*i] = 1.0; // Cyan
    i++;    // color number;    // set bright Magenta pen color;
    pCylColrs[    3*i] = 1.0;
    pCylColrs[1 + 3*i] = 0.0;
    pCylColrs[2 + 3*i] = 1.0; // Magenta
    i++;    // color number;    // set bright Yellow pen color;
    pCylColrs[    3*i] = 1.0;
    pCylColrs[1 + 3*i] = 1.0;
    pCylColrs[2 + 3*i] = 0.0; // Yellow
    }

}

void makeCone(double height, double radius)
{
    int i;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // Dynamic allocation for 3 triangle vertices.............................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    pCylVerts = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));
    if(pCylVerts==NULL) cout << "\n !Failed to malloc pTriVert!\n" << endl;
    // vertex counter.
    //              X coord;                Y coord;                Z coord.
    for (i=0;i<66;i++) {
    pCylVerts[3*i] = radius*sin(i*.1); pCylVerts[2 +3*i] = 0; pCylVerts[1 +3*i] = radius*cos(i*.1);
    i++;    // go on to next vertex;
    pCylVerts[3*i] =  radius*sin(i*.1); pCylVerts[2 +3*i] = 0; pCylVerts[1 +3*i] = radius*cos(i*.1);
    i++;
    pCylVerts[3*i] = 0; pCylVerts[2 +3*i] = height; pCylVerts[1 +3*i] = 0;
    }
    // Dyn. allocation for three colors.........................................
    pCylColrs = (GLdouble *)malloc(66* (3*sizeof(GLdouble)));  // 3 colors
    if(pCylColrs ==NULL) cout << "\n !Failed to malloc pTriColrs!\n" << endl;

    // color counter. // each color has 3 parts: 0 <= (R,G,B) <= 1.0
    for(i=0;i<66;i++) {
    //             Red value,           Green value,            Blue value.
    pCylColrs[   3*i] = 0.0;
    pCylColrs[1 +3*i] = 1.0;
    pCylColrs[2 +3*i] = 1.0; // Cyan
    i++;    // color number;    // set bright Magenta pen color;
    pCylColrs[    3*i] = 1.0;
    pCylColrs[1 + 3*i] = 0.0;
    pCylColrs[2 + 3*i] = 1.0; // Magenta
    i++;    // color number;    // set bright Yellow pen color;
    pCylColrs[    3*i] = 1.0;
    pCylColrs[1 + 3*i] = 1.0;
    pCylColrs[2 + 3*i] = 0.0; // Yellow
    }
}

void drawCone()
{
 glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pCylVerts); // --*pointer-- address of array's element [0].
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pCylColrs); // --*pointer--address of array's element [0]. */


        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLE_STRIP,      //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     66);                //--count-- and draw this many elements.

}

void totalCone(double height, double radius) {
    glPushMatrix();
    glRotated(90,1,0,0);
    makeCone(height,radius);
    drawCone();
    makeCircle(height,radius);
    drawCircle();
    glPopMatrix();
}

void totalwierdthing(double height, double radius) {
    glPushMatrix();
    int i;
    glRotated(-80,1,1,0);
    for (i=0;i<6;i++){
        glRotated(60,1,0,1);
        drawCrown();
    }
    glPopMatrix();
}

void totalchristmastree(int elements,double height,double radius) {
    glPushMatrix();
    int i;
    glRotated(180,1,0,0);
    for(i=0;i<elements;i++) {
        totalCone(height,radius);
        glScaled(.8,.8,.8);
        glTranslated(0,-.8*height,0);
    }
    glPopMatrix();
}

void bottle (double height, double radius) {
    glPushMatrix();
        totalCylinder(height,radius);
        glRotated(180,0,0,1);
        totalCone(height/3,radius);
        glTranslated(0,-height/3+.1,0);
        totalCylinder(height/6,radius/4);
    glPopMatrix();
}

void stool (double height, double radius) {
    glPushMatrix();
        totalCylinder(height/5, radius);
        totalCylinder(height,radius/5);
    glPopMatrix();
}

void RandomObjects(void) {
   glPushMatrix();
   glTranslated(1,-1.3,0);
    totalwierdthing(.5,.5);
    glTranslated(0,1.5,0);
    totalchristmastree(3,.5,.5);
    glTranslated(-2,0,0);
    bottle(.5,.5);
    glTranslated(0,.8,0);
    stool(.5,.5);
    glPopMatrix();
}
