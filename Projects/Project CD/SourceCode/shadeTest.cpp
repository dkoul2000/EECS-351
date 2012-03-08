//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
//
//	OpenGL/ GLUT 'starter' code for cameras, lighting, and shading.
//
//	OVERVIEW:
//		--Demonstrates openGL's built-in Phong lighting and (Gouraud) shading
//		--Demonstrates how to attach lights to different coordinate systems
//		ALSO:
//		--Limited demonstration of how to make user-positionable camera.
//
//	OPERATION:
//
//	  Draws 'world-space' axes as 3 colored lines (without shading):
//							+x=RED, +y=GREEN, +z = BLUE,
//			draws red and green teapots in world-space.
//
//	  Draws 'model-space' axes as 3 colored lines (without shading):
//							+x = CYAN, +y = MAGENTA, +z = YELLOW
//
//	  Draws 3 teapots, each with a different (boring) openGL material;
//			red,green teapots fixed to 'world' coord. system, while
//			blue teapot is in user-adjustable 'model' coord. system.
//	  Shading: Light 0 attached to world coord system;
//			   Light 1 attached to 'model' coord system (left mouse drag...)
//				as if it were at the center of the blue teapot.
//				If you move model-space coord system (where we draw blue teapot)
//				the light1 direction changes, re-lighting the red,green teapots.
//
//		--MOUSE left-click/drag rotates the camera around the world-space origin
//				-- ARROW keys translate the camera along its x and z axes.
//
//		--MOUSE right-click/drag adjusts rotation of a 'model' coord system
//				-- < and > keys adjusts model coord system's z translation.
//
//		-- 'r' key to reset camera position to initial values.
//		-- 'R' key RESET the model coord system to initial values;
//      -- 'm' or 'M' key to change Phong material in stuff[0].
// 		-- 's' or 'S' key to save OpenGL display window as BMP file
//				(default filename set by NU_SAVEFILE -- see shadeTexture.h)
//      -- 'z/Z' to lower/raise the pyramid tip (vertex-array object)
//		-- 'Q' key to quit.
//
//
//  for CS 351, Northwestern University, Jack Tumblin, jet@cs.northwestern.edu
//
//	10/29/2006 --J. Tumblin:  created from bits of 'Xform08robot': 3 lit,shaded
//					teapots...
//  11/03/2008 --J. Tumblin: better controls for camera & model coord. systems,
//					better comments to explain it;  multiple lights to show how
//					to attach lights to cam, world, or model coord. systems.
//					Updated 'idle()' callback explanations, updated CTransRot.
//  02/15/2010 --J. Tumblin: port to CodeBlocks, clean up comments, simplify.
//  07/05/2010 --J. Tumblin: create CMatl, CLight classes; includes my_vecmat.
//==============================================================================

#include "shadeTest.h"	// fcn prototypes, #defines,
#include "nu_progShader.h"
#include <iostream>

//===================
//
// GLOBAL VARIABLES (bad idea!)
//
//====================
CTransRot setModel;			// Mouse/Keyboard settings for model coord system.
CTransRot setCam;			// Mouse/Keyboard settings for camera coord system.

GLUquadricObj *pQuad0;		//ptr to the openGL quadric object(s) we'll draw
CMatl  stuff[3];            // Three material-describing objects.
CLight lamps[2];            // Two light source objects.

// One vertex-array-defined object: a simple pyramid with adjustable peak height
GLdouble *pyrVerts=NULL, *pyrNorms=NULL;
                                // For dynamically allocated arrays
                                // of vertices, surface normals, and colors.
GLdouble pyrHeight;        // height of pyramid (vertex-array object): z/Z keys

CProgGLSL *p_myGLSL;        //introducing a GLSL variable

//3D vertex array objects (from Project B)
squarePrism sp1 = squarePrism(1);
squarePrism sp2 = squarePrism(1);


int main( int argc, char *argv[] )
//------------------------------------------------------------------------------
{
	my_glutSetup(&argc, argv);		// GLUT calls that define windows, register
									// callbacks & any non-default OpenGL state.
	glutMainLoop();
	// Then give program control to GLUT.  This is an infinite loop, and from
	// within it GLUT will call the 'callback' functions below as needed.
	return 0;							// orderly exit.
}

void my_glutSetup(int *argc, char **argv)
//------------------------------------------------------------------------------
// A handy place to put all the GLUT library initial settings; note that we
// 'registered' all the function names for the callbacks we want GLUT to use.
{

	glutInit(argc, argv);				// GLUT's own internal initializations.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
								// double-buffered display, RGB (not RGBA)
                                // color model, use Z-buffering (depth buffer)
	glutInitWindowSize(NU_WIDTH, NU_HEIGHT);	// set display-window size
	glutInitWindowPosition(NU_XPOS, NU_XPOS);	// and position,
	glutCreateWindow(NU_TITLE);					// then create it.

    // And finally (must be AFTER the glutCreateWindow() call):
	glEnable( GL_DEPTH_TEST );			// enable hidden surface removal
// STUDENTS: what happens if you disable GL_DEPTH_TEST?
//	And what happens if display() callback clears the color buffer,
// 	but not the depth buffer? (e.g. what happens if you change
//  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); to
//  glClear( GL_COLOR_BUFFER_BIT)'; ?)

	// Register GLUT Callback function names. (these names aren't very creative)
	glutDisplayFunc(display);			// 'display'  callback:  display();
	glutReshapeFunc(reshape);			// 'reshape'  callback:  reshape();
	glutKeyboardFunc(keyboard);			// 'keyboard' callback:  keyboard();
	glutSpecialFunc(keySpecial);		// 'special'keys callback: keyspecial()
	glutMouseFunc(mouseClik);			// callbacks for mouse click, move
	glutMotionFunc(mouseMove);

//	glutIdleFunc(idle);					// 'idle'	  callback:  idle();
// CAREFUL! WE DON'T NEED IDLE()!  instead, we just call glutPostRedisplay()
// whenever we want the screen re-drawn.
//-----------------------------------
// A handy place to put all the OpenGL initial settings-- remember, you only
// have to change things if you don't like openGL's default settings.

	glClearColor(0.2, 0.2, 0.2, 0.0);	// Display-screen-clearing color;
										// acts as 'background color'

	pQuad0 = gluNewQuadric();	        // create a quadric object

    //***Create our materials.              // make pre-defined materials:
    stuff[0].createMatl(MATL_RED_PLASTIC);
    stuff[0].isFaceted = false;             // smooth-shaded,
    stuff[0].isTwoSided = false;            // shade front faces only.

    stuff[1].createMatl(MATL_GRN_PLASTIC);
    stuff[1].isFaceted = true;              // make a faceted green material
    stuff[1].isTwoSided = false;            // shade front faces only.

    stuff[2].createMatl(MATL_BLU_PLASTIC);
    stuff[2].isFaceted = false;
    stuff[2].isTwoSided = false;

    //***Create our light sources.          // make pre-defined light sources:
    lamps[0].createLamp(LAMP_POINT_KEY,     GL_LIGHT0);
    lamps[1].createLamp(LAMP_POINT_L_FILL,  GL_LIGHT1);
    //lamps[2].createLamp(LAMP_SPOT,  GL_LIGHT2);

    //***Create our pyramid object.
    pyrHeight = 0.0;       // initial height of pyramid tip.
    makePyramid();          // allocate memory if needed; compute vertices,normals

        //==============Create GLSL programmable shaders============================
    // Always AFTER 'glutCreateWindow()' because some GLSL commands rely on the
    // openGL 'rendering context' (all the state variables that tie openGL to
    // your particular OS and graphics card). The glutCreateWindow() call
    // forces creation of that 'rendering context' we need.

    #if !defined(__APPLE__)
    glewInit();                                 // if we use GLEW (Apple won't).
                                                // we must start its library;
    #endif
    // Create one GLSL-program object that will hold our programmable shaders;
    p_myGLSL = new CProgGLSL(argv[0],   "PassThroughVertexShader.vs",
                                        "PassThroughFragmentShader.fs");
    //cout << p_myGLSL;
    p_myGLSL->loadShaders();    // read in the shader files' contents
    p_myGLSL->compileProgram(); // compile and link the program for the GPU,
    p_myGLSL->useProgram();     // tell openGL/GPU to use it!


	glutMainLoop();	                // enter GLUT's event-handler; NEVER EXITS.

	delete p_myGLSL;                // orderly exit; delete the object we made.
}

void reshape(int w, int h)
//------------------------------------------------------------------------------
// GLUT 'reshape' Callback. Called when openGL display window gets created, and
// when user resizes the window from its current shape to one with width w and
// height h.
// We usually initialize (or re-initialize)openGL's 'GL_PROJECTION' matrix here.
{
	// set viewport to stretch the CVV to fit exactly in the display window
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	// Select projection matrix
	glMatrixMode(GL_PROJECTION);		// Select the Projection matrix,

//***CHOOSE A CAMERA  -----------------------------------------------
	glLoadIdentity();			// (Clear out any previous camera settings)
	gluPerspective(				// Set camera's internal parameters:
		20.0,					// vertical (y-axis) field-of-view in degrees,
		(double)w/(double)h,	// display image aspect ratio (width/height),
		NU_ZNEAR,NU_ZFAR);		// near,far clipping planes for camera depth.
//-----------------END camera setting.

	glutPostRedisplay();				// Re-draw the window (because we
                                        // changed the window size)
}

void display(void)
//------------------------------------------------------------------------------
// GLUT 'display' Callback.  GLUT calls this fcn when it needs you to redraw
// the dislay window's contents.  Your program should never call 'display()',
// because it will confuse GLUT--instead, call glutPostRedisplay() if you need
// to trigger a redrawing of the screen.
{
	// Clear the frame-buffer and the Z-buffer; ready for a new drawing.
	// (Set both bits using bit-wise OR)
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
									// clear the color and depth buffers

// =============================================================================
// START CAMERA POSITIONING CODE HERE:
// =============================================================================

	glMatrixMode(GL_MODELVIEW);		// select the model/view matrix, clear it:
	glLoadIdentity();				// (now cam = world = model coord. system)
	// By definition, we're in the 'cam' coord. system; we're at the 'root' of
	// a scene tree (tree of transformations).
	//
	//-------------------We ***COULD*** define an openGL light source here,
	// 					and it would cast light on the scene as if it were
	//					attached to our camera.  Our camera center-of-projection
	//					is at 0,0,0 in cam coord system, and we're looking down
	//					the -z axis.  Want a light in front of the camera?
	//					try positioning it at (0,0,-4)...(NOTE: openGL doesn't
	//					render the light itself, only its effect on surfaces).

	//-------------------Now create our 'world' coordinate system;
	//	--Make an exact copy of the current (cam) coord system:call it 'world':
	//	--transform that new 'world' coord system to put the 'world' coords
	// 		at the right place in the current 'cam' coordinate system.
	//	We **COULD** translate in -z direction to 'push away the world coord
	//  axes, then make a new copy of those axes, and rotate them, spinning the
	// world around that point in front of our camera:
	//***CAREFUL! uncomment ONLY ONE of these THREE camera-positioning methods!
	//
	/*	//BEGIN 1 of 3-------------------------------
		BE SURE to comment out the gluLookAt() below))
		glTranslated(0.0, 0.0,-10.0);	// move new origin in -z direction.
		glRotated(20, 1.0, 0.0, 0.0); 	// 20deg. rotation about that new
										// origin's y axis; now call that
										// the 'world' coord system.
	*/
        //END 1 of 3------------------------------------
	// 	or do it with mouse control like this:
		//BEGIN 2 of 3-----------------------------------
		glTranslated(0.0, 0.0, -9.0);		// push back the world along -z axis
											// (world origin in front of camera
											// which looks down cam's -z axis)
		setCam.apply_TR_Matrix();	// further translate the coord system by
                                    // mouse/keyboard control, then rotate it
									// (also from mouse/keyboard control).
		// TRANSLATION:	-- left/rt ARROW KEYS moves it in +/- x direction.
		//				-- up/dn ARROW KEYS move world origin in +/-y direction;
		//              -- +/- keyboard KEYS move world origin in +/-z direction
		// ROTATION:    -- LEFT MOUSE button drag to rotate about x,y axes.
		//END 2 of 3------------------------------------
	// Or do it with the classic 'glLookAt()' command:
		//BEGIN 3 of 3-------------------------------------
/*		gluLookAt(-5.0, 2.0, 8.66,	// VRP: eyepoint x,y,z position in world space.
			   0.0, 0.0, 0.0,	// 'look-at' point--we're looking at origin.
								// (VPN = look-at-point - VRP)
			   0.0, 1.0, 0.0);	// VUP: view 'up' vector; set 'y' as up...
*/	// END 3 of 3------------------------------------
	// CAREFUL!! gluLookAt() is a bit strange...
	// Usually, we make a NEW coord system (world) from the current
	// one (cam) by transformations defined/measured in the *current* one (cam).
	// With gluLookAt(), we're making the 'world' coord system using points
	// and vectors DEFINED IN WORLD coords! -- we're choosing a VRP, a look-at
	// point, and a VUP vector in world space, and using them to make a matrix
	// that changes our 'cam' coord. system into the 'world' coord system.
	//==========================================================================
	//  END CAMERA-POSITIONING CODE.
	//	(Now we can draw things in the 'world-space' coord. system:
	//==========================================================================

    //drawAxes(0);                // draw r,g,b axes
    //drawPyramid();              // Draw our adjustable pyramid in world space.

	// CREATE LIGHT 0:----------------------------------------------------------
	// IF IN -- 'cam' coordinate system: a "headlamp" attached to camera!
	//		 -- 'world' coord system: a "ceiling lamp" fixed overhead,
	//		 -- 'model' coord system: a lamp attached to a robot arm...

    lamps[0].I_pos.row[0] = 0.0f;// position our first lamp (already created in
    lamps[0].I_pos.row[1] = 5.0f;// myGlutSetup() fcn as LAMP_WHITE_KEY), and
    lamps[0].I_pos.row[2] = 3.0f;
    lamps[0].I_pos.row[3] = 1.0f;
    lamps[0].applyLamp();        // use it for lighting.

	// Set materials and shading for the first teapot:------------------------
    stuff[0].applyMatl();       // set openGL to use stuff[0] material params.
    stuff[0].showName();        // on-screen display names the material


	glPushMatrix();					// save 'world' coord. system;
        glTranslated(1.8, 0.0, 0.0);	// move to a starting pt away from origin,
        glutSolidTeapot(0.6);			// draw 1st teapot using material A.
                                            // (and whatever lights are enabled)
	glPopMatrix();					// return to 'world' coord system;
	//--------------------------------------
	glPushMatrix();					// save 'world' coord system;
        glTranslated(0.7, -0.7, 0.5);	// translate to the 'hinge point' we'll use
                                        // for this model;
        setModel.apply_RT_Matrix();		// LEFT_MOUSE drag rotates in on x,y axes,
                                        // <, > keys translate along z axis.
        glTranslated(-1.4, 1.4, -0.5);	// teapot offset AWAY from pivot point, so
                                        // we don't pivot around teapot's center...
        //================================================
        // NOW we can draw in the 'model' coordinate system:
        //==================================================
        // Draw axes in model-space coordinate system:
        drawAxes(1);            // draw cyan,magenta,yellow axes.
    //CREATE LIGHT 1------------------------------------------------------------
    // A second light source, fixed at origin in 'model' coordinates:
        lamps[1].I_pos.row[0] = 0.0f;   // set position of lamp 1; at origin
        lamps[1].I_pos.row[1] = 0.0f;
        lamps[1].I_pos.row[2] = 0.0f;
        lamps[1].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is
                                    // infinitely far away. w=1.0 for local lights.
        lamps[1].applyLamp();       // turn it on.
    //END light source 1------------------------------------------------------
        stuff[1].applyMatl();           // Setup openGL to use the 2nd material,
        glTranslated(-1.2, -0.75, 0.0);
        glutSolidTeapot(0.6);			// use it to draw 2nd, blue teapot.
                                        // (and whatever lighting is enabled)
        glPopMatrix();					// return to 'world' coord system.

        glPushMatrix();					// save 'world' coord. system, then
        glTranslated(0.0,-1.2, 0.0);	// translate to 3rd location,
        stuff[2].applyMatl();           // Set material we'll use for 3rd teapot:
        glutSolidTeapot(0.6);			// draw 3rd teapot using that material
                                        // and whatever lighting is enabled.

        lamps[2].I_pos.row[0] = 2.0f;   // set position of lamp 1; at origin
        lamps[2].I_pos.row[1] = 2.0f;
        lamps[2].I_pos.row[2] = 2.0f;
        lamps[2].I_pos.row[3] = 1.0f; // IMPORTANT! zero-valued 'w' means lamp is
                                    // infinitely far away. w=1.0 for local lights.
        lamps[2].applyLamp();       // turn it on.

        stuff[2].applyMatl();
        glTranslated(0.0,2.8,0.0);
        glutSolidTeapot(0.6);
        glColor3d(1.0, 1.0, 0.0);

	glPopMatrix();					// return to 'world' coord. system.

    //drawing my own 3D objects, a triangular prism and a square prism together
    glPushMatrix();
        glTranslated(0,0,0);
        glRotated(0,0,0,0);
        sp1.draw();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0,0.75,0);
        sp2.draw();
    glPopMatrix();


    // print instructions
    drawText2D(helv18, -0.5, -0.85, "'H' key: print HELP in console");

	// =========================================================================
	// END DRAWING CODE HERE
	// =========================================================================

	glFlush();
	glutSwapBuffers();			// Double-buffering: show the newly-drawn image.
}

void keyboard(unsigned char key, int x, int y)
//------------------------------------------------------------------------------
// GLUT 'keyboard' Callback.  User pressed an alphanumeric keyboard key.
// (x,y, give current mouse position).
// ('special' keys such as return, function keys, arrow keys? keyboardspecial)
{
int curMatl;
int junk;                   // to stop compiler warnings

    junk = x; junk = y;     // stops warnings of 'unused parameters' x,y
	switch(key) {
		case 27: // Esc
		case 'Q':
		case 'q':
		case ' ':
			exit(0);		// Quit application
			break;
        case 'H':
        case 'h':
            cout << "\nHELP MENU FOR PROJECT C/D (PROGRAMMABLE SHADERS)"
            << "\n------------------------------------------------"
            << "\nDrag the mouse to adjust the movable 3D camera"
            << "\nUse the arrow keys to move the camera/lighting around"
            << "\nUse R (capital for model, lowercase for camera) to reset"
            << "\nUse M to change the color of one of the teapots (shading mix)"
            << "\nUse O and P to move the Z-coordinates of the model"
            << "\nUse + and - to zoom in and out of the picture"
            << "\nQ, ENTER or SPACE BAR will quit the program" << endl << endl;
            break;
		case 'r':
			setCam.reset();	// reset camera coord system,
			break;
		case 'R':
			setModel.reset();	// reset model coord system.
			break;
        case 'm':
        case 'M':           // Cycle through all materials for stuff[0];
            curMatl = stuff[0].matlType; // get ID# for current material
            curMatl += 1;                // go to next material, but stay within
            if(curMatl > MATL_MAX) curMatl = MATL_RED_PLASTIC; // defined ones.
            stuff[0].createMatl(curMatl);   // change the material.
            break;
		case 'O':			// -Z Translation for model coord system
			setModel.z_pos -= 0.3;
			break;
		case 'P':			// +Z Translation for model coord system
			setModel.z_pos += 0.3;
			 break;
        case '+':           // PhotoShop-like move in/out with +/- keys
        case '=':
            setCam.z_pos += 0.1;
            break;
        case '-':
            setCam.z_pos -= 0.1;
			break;
		default:
			printf("unknown key %c:  Try arrow keys, r, R, s, S, <, >, or q",key);
			break;
	}
	// We might have changed something. Force a re-display
	glutPostRedisplay();
}

void keySpecial(int key, int x, int y)
//------------------------------------------------------------------------------
// GLUT 'special' Callback.  User pressed an non-alphanumeric keyboard key, such
// as function keys, arrow keys, etc. (x,y gives current mouse position)
{

	switch(key)
	{
		case GLUT_KEY_UP:		// up arrow key
			setCam.y_pos += 0.1;		// Move the camera coord system....
			break;
		case GLUT_KEY_DOWN:		// dn arrow key
			setCam.y_pos -= 0.1;
			break;
		case GLUT_KEY_LEFT:		// left arrow key
			setCam.x_pos += 0.1;
			break;
		case GLUT_KEY_RIGHT:	// right arrow key
			setCam.x_pos -= 0.1;
			break;
		default:
			break;
	}

	// We might have changed something. Force a re-display
	glutPostRedisplay();
}

void mouseClik(int buttonID,int upDown,int xpos,int ypos)
//------------------------------------------------------------------------------
// GLUT 'mouse' Callback.  User caused a click/unclick event with the mouse:
//     buttonID== 0 for left mouse button,
//			  (== 1 for middle mouse button?)
//			   == 2 for right mouse button;
//		upDown == 0 if mouse button was pressed down,
//			   == 1 if mouse button released.
//		xpos,ypos == position of mouse cursor, in pixel units within the window.
// *CAREFUL!* Microsoft puts origin at UPPER LEFT corner of the window.
{
	if(buttonID==0)				// if left mouse button,
	{
		if(upDown==0)			// on mouse press,
		{
			setCam.isDragging = 1;	// ready to record cam coord system changes.
			setCam.m_x = xpos;		// Dragging begins here.
			setCam.m_y = ypos;
		}
		else setCam.isDragging = 0;
	}
	else if(buttonID==2)		// if right mouse button,
	{
		if(upDown==0)
		{
			setModel.isDragging = 1;// ready to record model coord system changes.
			setModel.m_x = xpos;	// Dragging begins here.
			setModel.m_y = ypos;
		}
		else setModel.isDragging = 0;
	}
	else						// something else.
	{
		setCam.isDragging  = 0;	// default; DON'T change cam,
		setModel.isDragging = 0;	//					or  model coord systems.
	}
}

void mouseMove(int xpos,int ypos)
//------------------------------------------------------------------------------
// GLUT 'move' Callback.  User moved the mouse while pressing 1 or more of the
// mouse buttons.  xpos,ypos is the MS-Windows position of the mouse cursor in
// pixel units within the window.
// CAREFUL! MSoft puts origin at UPPER LEFT corner pixel of the window!
{
#define JT_INCR 1.0					// Degrees rotation per pixel of mouse move

	if(setModel.isDragging==1)			// if we're dragging the left mouse,
	{								// increment the x,y rotation amounts.
		setModel.x_rot += JT_INCR*(xpos - setModel.m_x);
		setModel.y_rot += JT_INCR*(ypos - setModel.m_y);
		setModel.m_x = xpos;		// and update current mouse position.
		setModel.m_y = ypos;
	}
	if(setCam.isDragging==1)		// if we're dragging theright mouse,
	{								// increment the x,y rotation amounts.
		setCam.x_rot += JT_INCR*(xpos - setCam.m_x);
		setCam.y_rot += JT_INCR*(ypos - setCam.m_y);
		setCam.m_x = xpos;
		setCam.m_y = ypos;
	}

	// We might have changed something. Force a re-display
	glutPostRedisplay();

#undef JT_INCR
}

/*
void idle(void)
//------------------------------------------------------------------------------
// GLUT 'idle' Callback. Called when OS has nothing else to do; a 'clock tick'.
// Use 'idle' *ONLY IF* your program does anything that needs lengthy computing
// and/or continual updates even when users are not pressing keys:
// put code to do the updates here.
// HINT: break up lengthy computations into small pieces; do a little bit, then
// return, to keep your program responsive and interactive even while it does
// work in 'idle()'.   IF your 'idle()' function runs for several seconds before
// it returns, your program will be unresponsive during that time.
//
// If you need to redraw the screen after your update, don't forget to call
// glutPostRedisplay() too.
//
//			*** AVOID THIS COMMON MISTAKE: ***
// 'idle()' gets called VERY OFTEN.  If you register 'idle()' and leave the idle
// function empty, GLUT will waste most/all CPU time, eating it all to make on
// useless calls to idle().
//	WORSE: if idle() contains only glutPostRedisplay(), you will force GLUT to
// redraw the screen as often as possible--even if the contents of the screen
// has not changed.  Instead, write your program so that it re-draws the screen
// ONLY when there has been a change to the screen's contents.  If your program
// only changes the screen in response to user inputs such as keyboard strikes,
// mouse moves, clicks, or drags etc., then your program doesn't need the idle()
// callback at all! Instead, call glutPostRedisplay() at the end of each of the
// GLUT callbacks that change the screen contents.
// Then you'll update the screen only when there is something new to show on it.
{

}
*/

void drawAxes(int colr)
//------------------------------------------------------------------------------
// When 'colr' = 0 draws red,grn,blu coordinate system axes;
// Otherwise draws       cyan,magenta,yellow axes
{
	glDisable(GL_LIGHTING);//-----------------Draw axes:
		glBegin(GL_LINES);			// start drawing lines:
            if(colr) glColor3f( 0.0, 1.0, 1.0); // cyan or
            else     glColor3f( 1.0, 0.0, 0.0);	// Red X axis
			glVertex3f( 0.0, 0.0, 0.0);
			glVertex3f( 1.0, 0.0, 0.0);

            if(colr) glColor3f( 1.0, 0.0, 1.0); // Magenta or
            else     glColor3f( 0.0, 1.0, 0.0);	// Green Y axis
			glVertex3f( 0.0, 0.0, 0.0);
			glVertex3f( 0.0, 1.0, 0.0);

            if(colr) glColor3f( 1.0, 1.0, 0.0); // Yellow or
            else     glColor3f( 0.0, 0.0, 1.0); // Blue Z axis
			glVertex3f( 0.0, 0.0, 0.0);
			glVertex3f( 0.0, 0.0, 1.0);
		glEnd();					// end drawing lines
	glEnable(GL_LIGHTING);// ------------DONE drawing axes.
}

void drawText3D(void *pFont, double x0, double y0, const char* pString)
//-----------------------------------------------------------------------------
//  Using the specified font, write the text in 'pString' to the display window
// at location x0,y0 in the current 3D coord system using the current color.
// CAREFUL! This is trickier to use than drawText2D()!
// CAREFUL! It ASSUMES that lighting is NOT enabled--if it is, you usually won't
// see any on-screen text.
// CAREFUL! Some or all of your text might be hiding inside a 3D object! e.g. if
// you put text at the origin of the current coord system (x0,y0,z) = (0,0,0)
// enable openGL's depth-testing, AND draw a teapot at that origin, then some or
// all of your text may hide inside!
//      Usage example:
//              glDisable(GL_LIGHTING);     // turn OFF lighting temporarily
//              glColor3d(1.0,1.0,0.0);     // bright yellow color
//              drawText3D(helv18, 0.3,0.4 ,"Hello!");  // print text in 3D;
//              glEnable(GL_LIGHTING);      // re-enable openGL lighting.
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

void drawText2D(void *pFont, double x0, double y0, const char* pString)
//------------------------------------------------------------------------------
// same as drawText3D(), but puts white text on top of the displayed image.
// Draws text at front of the CVV, so be sure x0,y0 fit within +/-1.0.
//      Example:  drawText2D(helv18, 0.3,0.4 ,"the 'M' key changes materials!");
//
// Available fonts:
// helv10 (HELVETICA size 10)   rom10 (TIMES_ROMAN size 10)
// helv12 (HELVETICA size 12)   rom24 (TIMES_ROMAN size 24)
// helv18 (HELVETICA size 18)
{
GLboolean isLit;
GLint curMatMode;

    glGetIntegerv(GL_MATRIX_MODE, &curMatMode);
                                    // save the currently-selected matrix-mode.
    glMatrixMode(GL_PROJECTION);    // Then temporarily turn off openGL camera,
    glPushMatrix();                 // save it.
        glLoadIdentity();           // put in a 'do-nothing' matrix
        glMatrixMode(GL_MODELVIEW); // temporarily turn off all model xforms,
            glPushMatrix();         // save them
                glLoadIdentity();       // put in a 'do-nothing' matrix,
                glGetBooleanv(GL_LIGHTING,&isLit); // save the lighting state;
                if(isLit == GL_TRUE)
                {
                    glDisable(GL_LIGHTING);  // temp. disable;
                    glColor3d(1.0,1.0,1.0);     // Set text color to 'white'
                    drawText3D(pFont, x0, y0, pString); // print it!
                    glEnable(GL_LIGHTING);  // re-enable.
                }
                else
                {   // lighting is OK; don't bother it, just print text.
                    glColor3d(1.0, 1.0, 1.0);   // Set text color to white
                    drawText3D(pFont, x0, y0, pString); // print it!
                }
            glPopMatrix();              // restore previous model xforms,
        glMatrixMode(GL_PROJECTION);    // go back to camera matrix mode,
    glPopMatrix();                      // restore previous openGL camera.
    glMatrixMode(curMatMode);           // restore previous matrix mode. Done!
}

void makePyramid(void)
//------------------------------------------------------------------------------
// create (global) arrays of verts,norms,colors at *pyrVerts, *pyrNorms, *pyrColr
{

int i;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);

    // STEP 2: Create the arrays, fill with data.
    // We did this already, but used the cheap & sleazy way by creating global
    // variables at top of file.  VERY BAD IDEA!!  Instead, make your own
    // 'shape' class that holds a complete description of one shape--I call
    // mine 'CGeom' (Class, geometry), and store vertices in dynamically alloc'd
    // in the class.  Your class should include members for 'resize()', for
    // making various standard shapes (triangle, box, pentagon, hexagon... and
    // later we'll expand it to 3D (tetrahedron,cube, arm, leg, sphere, torus..)

    // Dynamic allocation for 12 vertices (4 triangles) ........................
    // array of 12 vertices: each vertex holds 3 coordinate values: (x,y,z)

    if(pyrVerts==NULL)      // allocate memory if we haven't already done so.
    {
        pyrVerts = (GLdouble *)malloc(12* (3*sizeof(GLdouble)));
        if(pyrVerts==NULL) cout << "\n !Failed to malloc pyrVerts!\n" << endl;
    }
    // then initialize all the vertex locations.
    i=0; // vertex counter.
    // Right triangle:  X coord;                Y coord;                Z coord.
    pyrVerts[   3*i] = 0.0; pyrVerts [1 + 3*i] = 0.0; pyrVerts[2 + 3*i] = pyrHeight;
    i++;    // go on to next vertex;
    pyrVerts[   3*i] = 1.0; pyrVerts [1 + 3*i] =-1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    pyrVerts[   3*i] = 1.0; pyrVerts [1 + 3*i] = 1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    // Top triangle:  X coord;                Y coord;                Z coord.
    pyrVerts[   3*i] = 0.0; pyrVerts [1 + 3*i] = 0.0; pyrVerts[2 + 3*i] = pyrHeight;
    i++;    // go on to next vertex;
    pyrVerts[   3*i] = 1.0; pyrVerts [1 + 3*i] = 1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    pyrVerts[   3*i] =-1.0; pyrVerts [1 + 3*i] = 1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    // Left triangle:  X coord;                Y coord;                Z coord.
    pyrVerts[   3*i] = 0.0; pyrVerts [1 + 3*i] = 0.0; pyrVerts[2 + 3*i] = pyrHeight;
    i++;    // go on to next vertex;
    pyrVerts[   3*i] =-1.0; pyrVerts [1 + 3*i] = 1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    pyrVerts[   3*i] =-1.0; pyrVerts [1 + 3*i] =-1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    // Bottom triangle:  X coord;              Y coord;                Z coord.
    pyrVerts[   3*i] = 0.0; pyrVerts [1 + 3*i] = 0.0; pyrVerts[2 + 3*i] = pyrHeight;
    i++;    // go on to next vertex;
    pyrVerts[   3*i] =-1.0; pyrVerts [1 + 3*i] =-1.0; pyrVerts[2 + 3*i] = 0.0;
    i++;
    pyrVerts[   3*i] = 1.0; pyrVerts [1 + 3*i] =-1.0; pyrVerts[2 + 3*i] = 0.0;

    // Dynamic allocation for 12 vertices (4 triangles).........................
    // array of 12 vertices: each vertex holds 3 coordinate values: (x,y,z)
    if(pyrNorms==NULL)      // allocate memory if we haven't already done so.
    {
        pyrNorms = (GLdouble *)malloc(12* (3*sizeof(GLdouble)));
        if(pyrNorms==NULL) cout << "\n !Failed to malloc pyrNorms!\n" << endl;
    }
    // initialize the normal vectors for each vertex AS IF PyrHeight == z=1.0
    // (so that they pyramid sides are sloped 45 degrees)
    //**** YOU SHOULD COMPUTE THESE WITH CROSS_PRODUCTS ****
    i=0; // vertex counter.
    // Right triangle:  X coord;                Y coord;                Z coord.
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] = 0.0; pyrNorms[2  + 3*i] = 1.0;
    i++;    // go on to next vertex;
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] = 0.0; pyrNorms[2 + 3*i] = 1.0;
    i++;
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] = 0.0; pyrNorms[2 + 3*i] = 1.0;
    i++;
    // Top triangle:  X coord;                Y coord;                Z coord.
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] = 0.707; pyrNorms[2 + 3*i] = 0.707;
    i++;    // go on to next vertex;
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] = 0.707; pyrNorms[2 + 3*i] = 0.707;
    i++;
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] = 0.707; pyrNorms[2 + 3*i] = 0.707;
    i++;
    // Left triangle:  X coord;                Y coord;                Z coord.
    pyrNorms[   3*i] =-0.707; pyrNorms [1 + 3*i] = 0.0; pyrNorms[2 + 3*i] = 0.707;
    i++;    // go on to next vertex;
    pyrNorms[   3*i] =-0.707; pyrNorms [1 + 3*i] = 0.0; pyrNorms[2 + 3*i] = 0.707;
    i++;
    pyrNorms[   3*i] =-0.707; pyrNorms [1 + 3*i] = 0.0; pyrNorms[2 + 3*i] = 0.707;
    i++;
    // Bottom triangle:  X coord;              Y coord;                Z coord.
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] =-0.707; pyrNorms[2 + 3*i] = 0.707;
    i++;    // go on to next vertex;
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] =-0.707; pyrNorms[2 + 3*i] = 0.707;
    i++;
    pyrNorms[   3*i] = 0.0; pyrNorms [1 + 3*i] =-0.707; pyrNorms[2 + 3*i] = 0.707;

    /*
    // Dyn. allocation for three colors.........................................
    pColrs = (GLdouble *)malloc(3* (3*sizeof(GLdouble)));  // 3 colors
    if(pColrs ==NULL) cout << "\n !Failed to malloc pColrs!\n" << endl;

    i=0;    // color counter. // each color has 3 parts: 0 <= (R,G,B) <= 1.0
    //             Red value,           Green value,            Blue value.
    pColrs[    3*i] = 0.0; pColrs[1 + 3*i] = 1.0; pColrs[2 + 3*i] = 1.0; // Cyan
    i++;    // color number;    // set bright Magenta pen color;
    pColrs[    3*i] = 1.0; pColrs[1 + 3*i] = 0.0; pColrs[2 + 3*i] = 1.0; // Magenta
    i++;    // color number;    // set bright Yellow pen color;
    pColrs[    3*i] = 1.0; pColrs[1 + 3*i] = 1.0; pColrs[2 + 3*i] = 0.0; // Yellow
    */

    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
    glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pyrVerts);  // --*pointer-- address of array's element [0].
    glNormalPointer(GL_DOUBLE,  // --type-- openGL data type for each coord.,
                    0,          // --stride--byte offset between consecutive
                                // normals; nonzero for interleaved vertices,
                                // colors, etc.
                    pyrNorms);  // --*pointer-- address of array's element [0].

/*    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pyrColrs);   // --*pointer--address of array's element [0].
*/
    // (NOTE:---------------------
    // If you wish, you can merge several arrays into one 'interleaved'
    // array.  For each vertex, the interleaved array holds SEVERAL attributes
    // instead of just one.  For example, we could replace the pVerts and pColrs
    // arrays with just one interleaved array named 'pShape'. For each vertex,
    // we would store x,y,z,r,g,b values in sequence.  To 'bind' these two
    // interleaved attributes (vertex location, color), we need to use the
    // 'stride' value properly.  For example, you could do this:
    //      glDouble *pVertStart, *pColrStart;
    //      pVertStart = &(pShape[0]);    // point to the first vertex location;
    //      pColrStart = &(pShape[3]);    // point to the first color location.
    //      glVertexPointer(3,GL_DOUBLE,  // # of coords, data type for them,
    //               6*sizeof(GLdouble),  // stride:# of bytes from start of 1st
    //               pVertStart);         // vertex to start of next vertex.
    //      glColorPointer(3,GL_DOUBLE,   // same idea binds the color array.
    //               6*sizeof(GLdouble),  // stride:# of bytes from start of 1st
    //               pColrStart);         // color to the start of next color.
    // END OF NOTE----------------------
    // STEP 4: Tell OpenGL to draw pictures using these 'bound' array(s);
        //  *** SEE: drawTriangleBETTER().

}

void drawPyramid(void)          // used by 'drawPyramid' to draw a 3D shape.
//------------------------------------------------------------------------------
// draw the shape defined by *pyrVerts, *pyrNorms, (*pyrColr)
// before drawing, we made ARRAY(S) of vertices, colors, and other attributes
// by calling makePyramid();
// drawPyramid() function asks OpenGL to draw pictures using the contents of
// those arrays.  It transfers array contents from 'client' (your
// machine's RAM) to the 'server' (your graphics card's memory).
{
        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLES,      //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     12);                //--count-- and draw this many elements.
    // This call uses array(s) contents sequentially, from 'first' onwards,
    // and stops after using 'count' array elements.
    // But what if you don't want to use array elements in sequence?
    // What if you want to use elements 0,2,1,2,0, and end at 1?
    // Easy! just create and bind an 'index' array to hold that sequence.
    // Then use the glDrawElements() command instead of glDrawArrays().
}

//==============================================================================
//
//  CTransRot function bodies:
//
//==============================================================================

CTransRot::~CTransRot(void)
//------------------------------------------------------------------------------
// Default destructor
{
}

CTransRot::CTransRot(void)
//------------------------------------------------------------------------------
// Default constructor
{
	reset();						// set all values to zero.
}

void CTransRot::reset(void)
//------------------------------------------------------------------------------
// Set all values to zero.
{
	x_pos = 0.0; y_pos = 0.0; z_pos = 0.0;
	x_rot = 0.0; y_rot = 0.0; z_rot = 0.0;
}

void CTransRot::apply_TR_Matrix(void)
//------------------------------------------------------------------------------
// Apply translations, then rotations to current coordinate axes.
{
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
								// d suffix to specify 'doubles' as arguments.
	glRotated(z_rot, 0.0, 0.0, 1.0);	// rotate new coord system axes about
	glRotated(x_rot, 0.0, 1.0, 0.0);	// the current z axis, then x, then y.
	glRotated(y_rot, 1.0, 0.0, 0.0);
}

void CTransRot::apply_RT_Matrix(void)
//------------------------------------------------------------------------------
// Apply rotations, then translations to current coordinate axes.
{
	glRotated(z_rot, 0.0, 0.0, 1.0);	// rotate new coord system axes about
	glRotated(x_rot, 0.0, 1.0, 0.0);	// the current z axis, then x, then y.
	glRotated(y_rot, 1.0, 0.0, 0.0);
								// d suffix to specify 'doubles' as arguments.
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
}

//3D VERTEX ARRAY OBJECTS - GOTTEN FROM MY PROJECT B CODE
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
