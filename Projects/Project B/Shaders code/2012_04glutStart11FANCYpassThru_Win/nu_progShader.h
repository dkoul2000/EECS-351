#ifndef _NU_PROGSHADER_H_      // Guard-block to prevent cyclic #include
#define _NU_PROGSHADER_H_

//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (JT: I prefer to keep line lengths <=80 chars for easy printing/display).

/*
    CProgGLSL Class object  -- hold one complete GLSL program
    CShader   Class object  -- hold source-code of one GLSL shader
                                for use in one GLSL program

 As the OpenGL programming specification grew past version 2.0, it deprecated
 and replaced fixed-pipeline shading with programmable shading to enable far
 better rendering speeds and far more advanced picture-making capabilities.

 However, writing your first successful openGL program with programmable shaders
 is much more difficult than writing your first successful fixed-pipeline code:
 "There is no free lunch":
 --Most fixed-pipeline programs compiled and ran entirely on the CPU.  You could
 easily trace, single-step, and debug programs full of straightforward openGL
 functions such as glBegin(), glVertex(), and glMatrixMode() because these fcns
 used 'device drivers' that safely hid all the messy communications needed with
 the GPU/graphics card.
 --Programmable-shader programs abandon that model; your CPU uses openGL calls
 to transfer blocks of data to and from the GPU/graphics card memory, to request
 data transfers within the GPU, to 'compile' the C-like GLSL shader code into
 GPU-executable programs, and to run those executables as needed by different
 parts of the GPU's processing tasks.
 --Programmable-shader programs also require use of openGL's 'extension'
 mechanisms.  Unless you use a library such as GLEW (openGL Extension Wrangler)
 or Apple's iOS system, you cannot simply call these functions by name.

 The 'nu_progShader' library attempts to lower the 'entry barriers' with two
 shader-handling classes:
 --CShader object holds the source-code for one shader of any type (e.g. vertex
    shader, fragment shader, geometry shader, etc).  It can 1) read the shader
    from a plain-text file; 2) print the shader it read from the file (for
    inspection & debugging), and 3) hold its openGL-supplied 'shaderID'.
    for inspection,it from a plain-text
    file in your project directory that holds the GLSL code for the shader.
 --CprogGLSL object holds all the CShader objects used by your program; it can
    check to make sure your GPU/Graphics card supports programmable shading;
    load your programmable shaders from files, create openGL 'program' and
    'shader' objects on the GPU necessary for using programmable shaders, load
    the source-code, compile it for the GPU, transfer it into the GPU's
    'program' and 'shader' objects for use in rendering.  At the end of the
    program or when you want to change GPU programs, use the CProgGLSL object to
    'unload' the program and delete it from the GPU memory.

NOTE: The GPU usually can't do anything else while you load/unload/compile the
 GLSL programs, and once loaded and ready, the GPU uses it continuously. Limit
 GLSL program-object changes to the start and end of the program, or when you
 won't mind a few moments of waiting while the GPU rearranges itself for your
 newly loaded GLSL program (don't waste time in your main loop re-reading,
 recompiling, reloading etc.)
 */

 //------------------------------------------------------------------------------
// multi-platform includes for the GLUT/OpenGL libraries:
#if defined(_WIN32)
#include <GL\glew.h>    // Open-source GL extension wrangler: for GLSL shaders.
#include <GL\freeglut.h>// open-source GLUT functions for windows, keybd, mouse
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
#include <GL/glew.h>        // OpenGL 'extensions wrangler' for GLSL, 'mesa' use
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
//------------------------------------------------------------------------------
#include <string>
using namespace std;


/*==============================================================================
    CShader Class: GLSL source-code objects for OpenGL's GLSL shading language
          including:
            --vertex shaders,
            --fragment shaders,
            --and (eventually) geometry shaders.

 Each object of this type holds source code and misc descriptors for one
 complete 'shader' written in openGL's GLSL (openGL Shading Language); either
 a 'vertex shader' or a 'fragment shader'.
 Requires openGL 2.0 or better; on Windows we require open-source 'glew' library.
 Each object stores the shader's source as a single string ASCII text, held in
 a dynamically-allocated buffer at pointer pText.
 It offers member functions for
      text file read/write,
      report openGL version-number and extension-checking,

    2011.02.26  -- J. Tumblin. Created
    2012.01.22  -- P. Olczak; modified for use with CProgGLSL class.
    2012.02.29  -- J. Tumblin; revised naming, comments, multiplatform linking
================================================================================
*/
#include <iostream>
#include <fstream>
#include <string>


//===================shaderType===================
// Code to identify shader contents--useful as debugging tool.
#define INVALID_SHADER      0       // default for 'shaderType' member var.
//---VERTEX SHADERS-------
#define V_SHADER_BARE     100       // simplest possible vertex shader:
#define V_SHADER_FILENAME 101       // src code read in from 'fileName' member
// ADD YOUR OWN VERTEX SHADERS!

//---FRAGMENT SHADERS-----
#define F_SHADER_BARE     200       // simplest possible fragment shader:
#define F_SHADER_FILENAME 201       // src code read in from 'fileName' member
// ADD YOUR OWN FRAGMENT SHADERS!

//---GEOMETRY SHADERS-----
#define G_SHADER_BARE     300

using namespace std;

class CShader
{
public:
    int shaderType;                 // see #define statements above.
    string fileName;                // shader source-code filename to read/write
    string pathName;                // path to 'thisPrograms.exe' (this program)
                                    // (We need this for codeBlocks on Apple;
                                    // fstream's 'current working dir' is *NOT*
                                    // not your CodeBlocks project directory,
                                    // but the filesystem root (C:\)).
    string srcCode;                 // C++ string obj. holds shader source code
                                    // as the the char pointer srcCode.c_Str

    GLuint shaderObjID;             // GLSL identifies each shader object (held
                                    // within a program object) by a unique
                                    // 'shader ID'. Calling glCreateShader()
                                    // creates and empty shader object, and
                                    // returns its ID#.
    const GLchar *src;              // The NULL-terminated text string that
                                    // holds this shader's GLSL source code.
public:
    CShader(void);                // default constructor,destructor.
    ~CShader(void);

    void setPath(const char *pPath);// save pathName to executable's directory;
                                    // for Apple Mac OS X: setPath(argv[0]);
    void vertBareShader(void);      // Make bare 'pass-thru' vertex shader
    void fragBareShader(void);      // Make bare 'pass-thru' fragment shader
    void print(void);               // print shader's src code in console window
    void readShaderFile(const char *fName); // read the named shader file

};

/*==============================================================================

  CProgGLSL Class: Complete GLSL program, with create/load/compile/unload.


  2012.01.20  Created --paul olczak
  2012.01.30  fix missing GLEW library includes; fix CProgGLSL constructor
              filename errors, short/meaningful names,comments--Jack Tumblin

================================================================================
*/


class CProgGLSL
//------------------------------------------------------------------------------
// Holds a complete GLSL 'program' consisting of vertex shaders and fragment
// shaders.  The CProgGLSL object loads/compiles/unloads all shaders as needed
// into the GPU graphics hardware. Each shader's source code is stored in a
// 'CShader' object that read it from a plain-text file.
{
private:
    CShader   vSh,fSh;    // the vertex and fragment shader objects

    GLuint  programID;  // GLSL keeps shaders (& other objects too)
                        // in a 'Program Object' container; when you
                        // call glCreateProgram(), it makes an empty Program
                        // Object on the GPU and supplies the ID# to select it.
public:
    CProgGLSL(void);  // default constructor,destructor.
    ~CProgGLSL(void);

    CProgGLSL(const char *myArgv0,
                string vertexShFileName, string fragmentShFileName);
                // shader-path-and-filename-setting constructor;
                // use this to create a program object.

#if !defined(__APPLE__)
/* 'hardwareCheck()' member function is not available (and not necessary) on
 Apple MacOS, because we don't use the GLEW library this function requires.
 Apple Xcode supplies its own method (perhaps in the Xcode 'OpenGL' framework?)
 for linking with OpenGL's extended functions such as glCreateShaderObject().

 Windows and Linux systems need GLEW to simplify access to openGL extensions.
 Link your program to the GLEW library and after you've established your openGL
 rendering context (after your glutCreateWindow() command), call glewInit().
 After this, you can 'call-by-name' the openGL extended functions, just like any
 other OpenGL function.

 GLEW performs the messy process of accessing OpenGL's extended functions. Each
 time you call an extended function, GLEW checks availability on your hardware;
 if it exists, GLEW then requests a function pointer to it, and calls the
 functions with the arguments you supplied in your call-by-name usage.
 */
    int hardwareCheck(void);        // returns FALSE if this machine's openGL
                                    // implementation DOES NOT support GLSL;
#endif

    void setVertexShFileName(string name);
    void setFragmentShFileName(string name);

    // Shaders' default path: the directory that holds your executable (.exe)
    // (e.g. the path found in argv[0]). YOU PROBABLY DON'T NEED THESE FCNS!
    void setVertexShPath(string path);
    void setFragmentShPath(string path);

    void loadShaders(void);    // read shaders src-code from plain-text files

    void compileProgram(void); // convert source code into GPU executables

    void useProgram(void);     // load program into GPU, make it an active part
                                // of the current openGL rendering method/state.
};

#endif  // _NU_PROGSHADER_H_
