//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/*==============================================================================

        ARRAYTriStrip.cpp   This program demonstrates how to replace sensible
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

void drawTriStripDEPRECATED(int halfCount); // !!!BAD!!! Vertex-by-vertex dwg.
void makeTriStripBETTER(int halfCount);
void drawTriStripBETTER(int halfCount);

//Global variables; a spectacularly bad idea!
// (but callbacks can force their use: use sparingly or not at all).
//--------------------
GLdouble *pTriVerts,                //For GL_TRIANGLE dyn. alloc'd arrays
         *pTriColrs;                // for 3 vertices and 3 colors

GLdouble *pStripVerts,              // For GL_TRIANGLE_STRIP dyn. alloc'd arrays
         *pStripColrs,              // for 2*(halfCount+1) vertices,their colors
         *pLineColrs;               // and outline colors (line-strip)

#define JT_STRIP_HALFSIZE   5       // number of triangle-pairs in the tri-strip

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

                                    // OpenGL setup:
                                    // Create arrays of vertices & colors..
    makeTriangleBETTER();           //  ... used by drawTriangleBETTER()
    makeTriStripBETTER(JT_STRIP_HALFSIZE);  //  ... used by drawTriangleSTRIP()

	glutMainLoop();			        // start (endless) message-handling loop.
	return 0;                       // normal exit.
}

void myDisplay(void)				// (Function body)
//------------------------------------------------------------------------------
// GLUT callback function; this function gets called whenever the OS needs us to
// re-draw our picture in the on-screen display (window).

{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                                             // clear the old image.
    //drawTriangleDEPRECATED(); // Pick this one or...
    drawTriangleBETTER();       // this one. They both work...

    //drawTriStripDEPRECATED(JT_STRIP_HALFSIZE);   // Pick this one or...
    drawTriStripBETTER(JT_STRIP_HALFSIZE);       // this one. They both work...

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
    // STEP 1: ENABLE ARRAYS in openGL(in makeTriangleBETTER())
    // STEP 2: CREATE, FILL ARRAYS  (in makeTriangleBETTER())

    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
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
                   pTriColrs); // --*pointer--address of array's element [0].


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
// The 'drawTriangleBETTER()' function then makes OpenGL *use* these arrays
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
    // Done here the cheap & sleazy way by creating global variables at top
    // of this file.  VERY BAD IDEA!!  Instead, make your own
    // 'shape' class that holds a complete description of one shape--I call
    // mine 'CGeom' (Class, geometry), and store vertices in dynamically alloc'd
    // in the class.  Your class should include members that allow you to use
    // various openGL drawing primitives (e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP
    // for making various standard shapes (triangle, box, pentagon, hexagon...
    // and expandable to 3D (tetrahedron,cube, arm, leg, sphere, torus..)

    // Dynamic allocation for 3 triangle vertices.............................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    pTriVerts = (GLdouble *)malloc(3* (3*sizeof(GLdouble)));
    if(pTriVerts==NULL) cout << "\n !Failed to malloc pTriVert!\n" << endl;
    i=0; // vertex counter.
    //              X coord;                Y coord;                Z coord.
    pTriVerts[3*i] = -1.0; pTriVerts[1 +3*i] = -1.0; pTriVerts[2 +3*i] = 0.0;
    i++;    // go on to next vertex;
    pTriVerts[3*i] =  1.0; pTriVerts[1 +3*i] = -1.0; pTriVerts[2 +3*i] = 0.0;
    i++;
    pTriVerts[3*i] =  0.0; pTriVerts[1 +3*i] =  1.0; pTriVerts[2 +3*i] = 0.0;
    // Dyn. allocation for three colors.........................................
    pTriColrs = (GLdouble *)malloc(3* (3*sizeof(GLdouble)));  // 3 colors
    if(pTriColrs ==NULL) cout << "\n !Failed to malloc pTriColrs!\n" << endl;

    i=0;    // color counter. // each color has 3 parts: 0 <= (R,G,B) <= 1.0
    //             Red value,           Green value,            Blue value.
    pTriColrs[   3*i] = 0.0;
    pTriColrs[1 +3*i] = 1.0;
    pTriColrs[2 +3*i] = 1.0; // Cyan
    i++;    // color number;    // set bright Magenta pen color;
    pTriColrs[    3*i] = 1.0;
    pTriColrs[1 + 3*i] = 0.0;
    pTriColrs[2 + 3*i] = 1.0; // Magenta
    i++;    // color number;    // set bright Yellow pen color;
    pTriColrs[    3*i] = 1.0;
    pTriColrs[1 + 3*i] = 1.0;
    pTriColrs[2 + 3*i] = 0.0; // Yellow

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

    //  *** SEE: drawTriangleBETTER() for these next 2 steps:
    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
    // STEP 4: Tell OpenGL to draw pictures using these 'bound' array(s);
}

void drawTriStripDEPRECATED(int halfCount)
//------------------------------------------------------------------------------
// Simple example of the ***OLD*** way to draw a GL_TRIANGLE_STRIP primitive;
// creates 2*halfCount triangles by generating a 'stair-step' vertex sequence.
//
// WARNING:  Don't use this method -- deprecated!  don't build a bad habit!
{
int i, imax;

    imax = halfCount+1;         // simplifies our 'for' loop...
    glBegin(GL_TRIANGLE_STRIP);
    for(i=0; i< imax; i++)
    {
        // A 'stairstep' of triangles from (-1,-1) to (+1,+1) across CVV:
		glColor3d(  1.0,  0.0, 0.0);   // bright red pen color
        glVertex3d(-1.0 + (2.0*i)/imax,         // vertex along diagonal
                   -1.0 + (2.0*i)/imax, 0.0);

		glColor3d(  0.0, 0.0,  1.0);	// bright blue pen color
        glVertex3d(-1.0 + (2.0*(i+1))/imax,
                   -1.0 + (2.0*i)/imax, 0.0);   // incremented x; same y.
    }
    glEnd();

    // Draw the sequence of vertices as a line loop;
    glColor3d(  0.0,  1.0, 0.0);   // bright green line color
    glBegin(GL_LINE_STRIP);
    for(i=0; i<imax; i++)
    {   // Same 'stairstep' of triangles from (-1,-1) to (+1,+1) across CVV:
        glVertex3d(-1.0 + (2.0*i)/imax,         // vertex along diagonal
                   -1.0 + (2.0*i)/imax, 0.0);
        glVertex3d(-1.0 + (2.0*(i+1))/imax,
                   -1.0 + (2.0*i)/imax, 0.0);   // incremented x; same y.
    }
    glEnd();
}

void makeTriStripBETTER(int halfCount)
//------------------------------------------------------------------------------
// Here, we make ARRAY(S) of vertices, colors and other attributes that describe
// the triangle strip we want to draw.  In OpenGL, we can make arrays that hold:
// vertices, colors, normals, texture coordinates, fog coords, edge flags, and
// indices (an array that holds the sequence we want for those other attributes)
// The 'drawTriangleBETTER()' function then makes OpenGL *use* these arrays
// draw pictures.
{
int i, imax;

    // STEP 1: Enable Arrays
    //========================
    // openGL is one gigantic state machine; unless you say otherwise, it uses
    // default states for everything.  Set it to the non-default state where it
    // will accept ARRAYS of vertices and colors to specify drawings.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // STEP 2: Create the arrays, fill with data.
    // Done here the cheap & sleazy way by creating global variables at top
    // of this file.  VERY BAD IDEA!!  Instead, make your own
    // 'shape' class that holds a complete description of one shape--I call
    // mine 'CGeom' (Class, geometry), and store vertices in dynamically alloc'd
    // in the class.  Your class should include members that allow you to use
    // various openGL drawing primitives (e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP
    // for making various standard shapes (triangle, box, pentagon, hexagon...
    // and expandable to 3D (tetrahedron,cube, arm, leg, sphere, torus..)

    // Dyn. alloc. for 2*(1+halfCount) tri-strip vertices........................
    // array of 3 vertices: each vertex holds 3 coordinate values: (x,y,z)
    imax = halfCount+1;         // simplifies for() loops...
    pStripVerts = (GLdouble *)malloc(2*imax * (3*sizeof(GLdouble)));
    if(pStripVerts==NULL) cout << "\n !Failed to malloc pStripVert!\n" << endl;
    // Fill the vertex array with 'stair-step' pairs of vertex positions; start
    // at -1,-1 and stretch up to +1,+1, but skip the top step to avoid drawing
    // triangles we can't see on-screen.
    for(i=0; i<imax; i++)   // for each horizontal step tread, make 2 vertices;
    {
        pStripVerts[6*i  ] = -1.0 + (2.0*i)/imax;   // x  // vertex on diagonal
        pStripVerts[6*i+1] = -1.0 + (2.0*i)/imax;   // y
        pStripVerts[6*i+2] = 0.0;                   // z

        pStripVerts[6*i+3] = -1.0 + (2.0*(i+1))/imax;   // x shifted left 1 step
        pStripVerts[6*i+4] = -1.0 + (2.0*i)/imax;   // y same as previous vert
        pStripVerts[6*i+5] = 0.0;                   // z
    }
    // Dyn. alloc. for 2*(1+halfCount) tri-strip colors.........................
    pStripColrs = (GLdouble *)malloc(2*imax * (3*sizeof(GLdouble)));  // 3 colors
    if(pStripColrs==NULL) cout << "\n !Failed to malloc pStripColrs!\n" << endl;
    for(i=0; i<imax; i++)   // for each pair of vertices in tri strip,
    {
        pStripColrs[6*i   ] = 1.0;  // red  // red on diagonal
        pStripColrs[6*i +1] = 0.0;  // grn
        pStripColrs[6*i +2] = 0.0;  // blu

        pStripColrs[6*i +3] = 0.0;  // red  // blue on rightward edges
        pStripColrs[6*i +4] = 0.0;  // grn
        pStripColrs[6*i +5] = 1.0;  // blu
    }
    // Dyn. alloc. for 2*(1+halfCount) tri-strip line colors....................
    pLineColrs = (GLdouble *)malloc(2*imax * (3*sizeof(GLdouble)));  // 3 colors
    if(pLineColrs ==NULL) cout << "\n !Failed to malloc pLineColrs!\n" << endl;
    for(i=0; i<imax; i++)   // for each pair of vertices in tri strip,
    {
        pLineColrs[6*i   ] = 0.0;  // red  // black on diagonal
        pLineColrs[6*i +1] = 0.0;  // grn
        pLineColrs[6*i +2] = 0.0;  // blu

        pLineColrs[6*i +3] = 1.0;  // red  // white on rightward edges
        pLineColrs[6*i +4] = 1.0;  // grn
        pLineColrs[6*i +5] = 1.0;  // blu
    }

//  *** SEE: drawTriangleBETTER() for these next 2 steps:
    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
    // STEP 4: Tell OpenGL to draw pictures using these 'bound' array(s);
}
void drawTriStripBETTER(int halfCount)
//------------------------------------------------------------------------------
// The NON-DEPRECATED, BETTER way to draw a simple shape in openGL:
// before drawing, we made ARRAY(S) of vertices, colors, and other attributes
// by calling makeTriangleBETTER().
// drawTriangleBETTER() function asks OpenGL to draw pictures using the
// contents of those arrays.  It transfers array contents from 'client' (your
// machine's RAM) to the 'server' (your graphics card's memory).
{
int imax;

    // ALREADY DONE IN makeTriStriBETTER():
    // STEP 1: ENABLE ARRAYS in openGL
    // STEP 2: CREATE, FILL ARRAYS with triangle strip arrays.

    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
    glVertexPointer(3,          // --size-- # of coords per vertex (2,3,or 4)
                    GL_DOUBLE,  // --type-- openGL data type for each coord.
                    0,          // --stride-- byte offset between consecutive
                                // vertices; nonzero for interleaved vertices,
                                // color, normals, etc.
                    pStripVerts);// --*pointer-- address of array's element [0].
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pStripColrs); // --*pointer--address of array's element [0].

    imax = halfCount +1;        // for simpler for() loops;

    // STEP 4: Draw Triangle Strip..............................................
        glDrawArrays(                   // Using the currently bound arrays,
                     GL_TRIANGLE_STRIP, //--mode-- use this drawing primitive,
                     0,                 //--first-- start at this array element,
                     2*imax);           //--count-- and draw this many elements.
    // This call uses array(s) contents sequentially, from 'first' onwards,
    // and stops after using 'count' array elements.
    // But what if you don't want to use array elements in sequence?
    // What if you want to use elements 0,2,1,2,0, and end at 1?
    // Easy! just create and bind an 'index' array to hold that sequence.
    // Then use the glDrawElements() command instead of glDrawArrays().

    // Draw Green/yellow outlines of the triangle:..............................
    // STEP 3: 'bind' arrays; tell openGL where to find the data arrays(s) in
    // memory, and specify the how it was organized there:
    // CHANGE the color pointer only:
    glColorPointer(3,           // --size-- # of color coordinate (3 or 4)
                                // ( why 4? for RGBA; A for 'alpha'==opacity)
                   GL_DOUBLE,   // --type-- openGL data type for each coord.
                   0,           // --stride-- byte offset between consecutive...
                   pLineColrs); // --*pointer--address of array's element [0].
    // STEP 4: draw, using the currently bound arrays:
    glDrawArrays(                   // Using the currently bound arrays,
                    GL_LINE_STRIP,  //--mode-- use this drawing primitive,
                    0,              //--first-- start at this array element,
                    2*imax);        //--count-- and draw this many elements.

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

