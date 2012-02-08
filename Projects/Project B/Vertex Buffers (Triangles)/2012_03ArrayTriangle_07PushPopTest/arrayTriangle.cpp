//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/*==============================================================================

        ARRAYTriangle.cpp   This program demonstrates how to replace sensible
                            but deprecated (and inefficient) calls to old-style
                            fixed-pipeline openGL (2.1 and earlier) with calls
                            suitable for programmable-pipeline openGL(>= 3.0).
                            ====================================================
                            See pp. 187-191 in your textbook; Edward Angel,
                            "Interactive Computer Graphics..." 5th Edition
                            ====================================================
                            Sadly, openGL 3.0 and beyond deprecates these very
                            intuitive, sensible, but wildly inefficient funcs:
                                    glBegin(),glEnd(),glVertex().
                            Instead, you can modify this program to learn about
                            these fast, non-deprecated commands:
                                    DrawArrays(),   MultiDrawArrays()
                                    DrawElements(), MultiDrawElements()
                                    DrawRangeElements()

                            Use them only with non-deprecated openGL primitives:
                            GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
                            GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN

                            And avoid these deprecated prims(openGL3.0 & beyond)
                                GL_QUADS, GL_QUAD_STRIP, GL_POLYGON

? What's 'deprecated'? What's not? See:
http://www.devklog.net/2008/08/23/forward-compatible-opengl-3-entry-points/

2011.12.13-J. Tumblin: created for use in EECS351-1 at Northwestern University
*=============================================================================*/

// multi-platform includes for the GLUT/OpenGL libraries:-----------------------
#if defined(_WIN32)
#include <GL/freeglut.h>// open-source GLUT functions for windows, keybd, mouse
// *OR* use:
// #include <GL/glut.h> // for 100% backwards-compatibility with Mark Kilgard's
                        // SGI-copyrighted GLUT library. Careful! this excludes
                        // freeglut's callbacks for mouse wheel, etc.
// freeglut.h already contains these other includes that were necessary for
// using Kilgard's original GLUT library:
// #include <GL/GL.h>      // OpenGL 3D graphics rendering functions
// #include <GL/Glu.h>     // OpenGL utilties functions
// #include <GL/glext.h>   // #defines for all OpenGL features on your machine.

#elif defined(__APPLE__)
#include <GLUT/glut.h>  // GLUT functions for windows, keybd, mouse
#include <OpenGL/GL.h>  // OpenGL 3D graphics rendering functions
#include <OpenGL/Glu.h> // OpenGL utilties functions
#include <OpenGL/GLext.h>   // #defines for all OpenGL features on your machine.
// UNIX, Linux of all flavors;
#else
#include <unistd.h>         // defines symbolic constants, POSIX versions, etc.
#include <GL/glew.h>        // OpenGL 'extensions wrangler' for 'mesa' lib usage
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
//==============================================================================
using namespace std;        // for cin, cout.
#include <iostream>         // for cin, cout.

void myDisplay(void);			// GLUT callback function for: window redraw.
void drawTriangleDEPRECATED(void);      // !!!BAD!!! Vertex-by-vertex drawing
void makeTriangleBETTER(void);          // Better: array-based drawing.
void drawTriangleBETTER(void);

//Global variables; a spectacularly bad idea!
// (but callbacks can force their use: use sparingly or not at all).
//--------------------
GLdouble *pVerts, *pColrs;      // For dynamically allocated arrays
                                // (or to point to static arrays)
/*
// For a gradual transition from glVertex() calls to attribute arrays, you can
// start with statically allocated arrays (not recommended--inflexible!) and
// point to them with pVerts, pColrs.  Once this works, replace them with
// dynamically-allocated arrays.
                                // Vertex positions:
GLdouble VertArry[] = {         // array of three x,y,z triplets.
        -1.0, -1.0, 0.0,        // vertex 0 values for (x,y,z)...
         1.0, -1.0, 0.0,        // vertex 1 ...
         0.0,  1.0, 0.0};       // vertex 2 ...

                                // Color values:
GLdouble ColrArry[] = {         //array of three r,g,b triplets.
         0.0,  1.0,  1.0,       // bright Cyan color
         1.0,  0.0,  1.0,	    // bright Magenta color
         1.0,  1.0,  0.0};	    // bright Yellow pen color.
*/

int main(int argc, char *argv[])
//=============================================================================
// Initialization--then start the event loop for callbacks (glutMainLoop).
{

    glutInit(&argc, argv);  // initialize GLUT/OpenGL, relay any cmd-line args;
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
									// double buffering, use full 32-bit color
									// (remember glSwap(); to show what we drew)

	glutCreateWindow("Northwestern University - EECS 351-1 Intro Graphics ");
									// Open a window in your operating system
	glutDisplayFunc(myDisplay);		// Register the fcn. GLUT calls for drawing
//	pVerts = VertArry;              // Used fixed array contents? Point to them!
//	pColrs = ColrArry;
    makeTriangleBETTER();           // OpenGL setup: arrays of vertices & colors
	glutMainLoop();			        // start (endless) message-handling loop.
	return 0;                       // normal exit.
}

void myDisplay(void)				// (Function body)
//------------------------------------------------------------------------------
// GLUT callback function; this function gets called whenever the OS needs us to
// re-draw our picture in the on-screen display (window).

{
int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                                             // clear the old image.
    glMatrixMode(GL_MODELVIEW); // Select the model-view matrix,
    glPushMatrix();             // save its current contents (identity matrix)
        glScaled(0.3, 0.3, 0.3);    // shrink drawing axes,
        glTranslated(-3.0,-1.0, 0.0);  // move drawing leftwards & downwards;
        glRotated(-30.0, 0.0, 0.0, 1.0);// rotate -30 degrees around z axis;
        for(i=0; i<20; i++)
        {
            if(i==7) glPushMatrix();   // SAVE the MIDPOINT drawing axes;
        //drawTriangleDEPRECATED(); // Draw vertex-by-vertex triangle, or...
            drawTriangleBETTER();       // draw array triangle. Then:

            glTranslated(0.4, 0.2, 0.0);    // shift a little along the X axis,
            glRotated(15.0 -3.0*i, 0.0, 0.0, 1.0); // spin by a shrinking angle

        }
        glPopMatrix();          // go back to 'midpoint' drawing axes, and
        for(i=0; i<10; i++)     // draw another arm:
        {
        //drawTriangleDEPRECATED(); // Draw vertex-by-vertex triangle, or...
            drawTriangleBETTER();       // draw array triangle. Then:

            glTranslated(-0.2, 0.2, 0.0);    // shift a little along the X axis,
            glRotated(-10.0 +3.5*i, 0.0, 0.0, 1.0); // spin by a growing angle
        }

    glPopMatrix();              // restore previously stored matrix.
// NOTE HOW EACH PUSH_MATRIX HAS A MATCHING POPMATRIX!
	glutSwapBuffers();	// double-buffering; SHOW what openGL just drew...
}

void drawTriangleDEPRECATED(void)
//------------------------------------------------------------------------------
// Simple example of the ***OLD*** way to draw a simple colored shape in openGL:
//   the glBegin() function's arg specifies which openGL drawing primitive used.
// EXERCISE: Make this an equilateral triangle.
// EXERCISE: Try other openGL drawing primitives.
// WARNING:  Don't use this method -- deprecated!  don't build a bad habit!
{
    glBegin(GL_TRIANGLES);		// draw a simple triangle
		glColor3d(  0.0,  1.0, 1.0);   // bright Cyan pen color
        glVertex3d(-1.0, -1.0, 0.0);

		glColor3d(  1.0, 0.0,  1.0);	// bright Magenta pen color
        glVertex3d( 1.0, -1.0, 0.0);

        glColor3d(  1.0,  1.0, 0.0);	// bright Yellow pen color
        glVertex3d( 0.0,  1.0, 0.0);
    glEnd();
}

void drawTriangleBETTER(void)
//------------------------------------------------------------------------------
// The NON-DEPRECATED, BETTER way to draw a simple shape in openGL:
// before drawing, we made ARRAY(S) of vertices, colors, and other attributes
// by calling makeTriangleBETTER().
// drawTriangleBETTER() function asks OpenGL to draw pictures using the
// contents of those arrays.  It transfers array contents from 'client' (your
// machine's RAM) to the 'server' (your graphics card's memory).
{
        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLES,      //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     3);                //--count-- and draw this many elements.
    // This call uses array(s) contents sequentially, from 'first' onwards,
    // and stops after using 'count' array elements.
    // But what if you don't want to use array elements in sequence?
    // What if you want to use elements 0,2,1,2,0, and end at 1?
    // Easy! just create and bind an 'index' array to hold that sequence.
    // Then use the glDrawElements() command instead of glDrawArrays().
}

void makeTriangleBETTER(void)
//------------------------------------------------------------------------------
// Here, we make ARRAY(S) of vertices, colors and other attributes that describe
// the triangle shape we want to draw.  In OpenGL, we can make arrays that hold:
// vertices, colors, normals, texture coordinates, fog coords, edge flags, and
// indices (an array that holds the sequence we want for those other attributes)
// The 'drawTriangleBETTER()' function then makes OpenGL *use* these arrays to
// draw pictures.
{
int i;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // STEP 2: Create the arrays, fill with data.
    // We did this already, but used the cheap & sleazy way by creating global
    // variables at top of file.  VERY BAD IDEA!!  Instead, make your own
    // 'shape' class that holds a complete description of one shape--I call
    // mine 'CGeom' (Class, geometry), and store vertices in dynamically alloc'd
    // in the class.  Your class should include members for 'resize()', for
    // making various standard shapes (triangle, box, pentagon, hexagon... and
    // later we'll expand it to 3D (tetrahedron,cube, arm, leg, sphere, torus..)

    // Dynamic allocation for 3 triangle vertices.............................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    pVerts = (GLdouble *)malloc(3* (3*sizeof(GLdouble)));
    if(pVerts==NULL) cout << "\n !Failed to malloc pVert!\n" << endl;
    i=0; // vertex counter.
    //              X coord;                Y coord;                Z coord.
    pVerts[   3*i] = -1.0; pVerts [1 + 3*i] = -1.0; pVerts[2 + 3*i] = 0.0;
    i++;    // go on to next vertex;
    pVerts[   3*i] =  1.0; pVerts [1 + 3*i] = -1.0; pVerts[2 + 3*i] = 0.0;
    i++;
    pVerts[   3*i] =  0.0; pVerts [1 + 3*i] =  1.0; pVerts[2 + 3*i] = 0.0;
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

    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
    glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pVerts);    // --*pointer-- address of array's element [0].
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pColrs);     // --*pointer--address of array's element [0].
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

// DISCUSSION:
//==============================================================================
// 1) WHY did we write 'makeTriangleBETTER()' and not 'makeTriangleBEST()?
// This function creates arrays inside your PC's main memory, in the
// 'client side' of the openGL client/server model.  Every time we call
// drawTriangleBETTER(), openGL COPIES the bound arrays' contents ('client')
// into the graphics card's memory (the 'server') for rendering.
// All that copying takes time; for much faster results, openGL 2.1 and beyond
// also provides 'buffer objects' -- server-side array storage, without needless
// copying. Drawing pictures from these 'buffer objects' yields the 'best',
// highest-speed rendering in OpenGL.
//
// 2)  This program uses just one OpenGL drawing primitive: GL_TRIANGLES,
// but OpenGL offers more; these 6 non-deprecated openGL drawing primitives:
//  GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
//  GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
//
// Exercise:  Modify/Extend this program to use ALL of them.
//
// 3) This program initially used fixed-size arrays as global variables--a
// horror to any respectable object-oriented programmer and very inflexible if
// we want to change a shape.
// INSTEAD, look at comments for 'makeTriangleBETTER()', STEP 2, and create
// your own openGL shape-description class.  It should use dynamic memory
// allocation, and use re-sizeable  arrays of vertices, colors, normals, or any
// of openGL's different kinds of array attributes, and include easy-to-use
// methods such as 'drawShape' that draws its current contents.  We'll expand
// this shape-description class as we learn more openGL.

