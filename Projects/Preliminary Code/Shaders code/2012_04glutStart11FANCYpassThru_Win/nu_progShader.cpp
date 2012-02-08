//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// (JT: I prefer to keep line lengths <=80 chars for easy printing/display).
/*
        CShader class members
            Each CShader object holds source-code for one programmable shader,
            written in openGL's C-like 'GLSL' language.  It reads shaders from
            ordinary text files, and takes care of all the pesky file-system
            directory and read/write bookkeeping

        CProgGLSL class members
            Each CProgGLSL object holds a complete GLSL program made of two
            shader objects with GLSL source code and openGL IDs.  It loads
            shader source code from files, compiles, loads the program and
            its shaders into the GPU, and directs the GPU to render with it.

---Revisions
2012.01.20  paul olczak.
    Changed shader file reader to append newline character at end of each line;
    permits us to use GLSL's compiler directives, and keeps formatting readable.
*/

#include "nu_progShader.h"

CShader::CShader(void)
//------------------------------------------------------------------------------
// default constructor.
{
    shaderType = INVALID_SHADER;    // forces you to initialize.
    srcCode.erase();                // discard any existing source code.
    fileName = "[No specified file]";
    pathName.erase();               // EMPTY but valid string.
}

CShader::~CShader(void)
//------------------------------------------------------------------------------
// default destructor.
{
    shaderType = INVALID_SHADER;
    srcCode.erase();
}

void CShader::setPath(const char *pPath)
//------------------------------------------------------------------------------
// find the complete path to executable's directory;
// for Apple Mac OS X: setPath(argv[0]); otherwise, readShaderFile() will
// require the entire path to your CodeBlocks directory "/Users/<myName>/...
{
size_t found;

#if defined(__APPLE__)
    pathName.erase();
    pathName = pPath;
    //cout << "\n\n Given path:" << pathName << "\n\n" << endl;
    // Does the pathName end with our executable's filename? (progShade)?
    // SEE: www.cplusplus.com/reference/string/string/find_last_of
    found = pathName.find_last_of("/");   // Linux,Apple path? use "/"
                                          //Windows path? use "/\\"
    if(found != string::npos)
    {
        pathName = pathName.substr(0,found);    // discard executable filename
        pathName += "/";                // replace the slash we removed...
    }
    cout << "\n\n Trimmed path:" << pathName << "\n\n" << endl;
#endif
#if defined(_WIN32)         // Codeblocks/Windows argv[0] gives us INCOMPLETE
                            // path (skips Debug/Bin/ directories?!?!) like this
                    // C:\\Teach\\CS351-1\\2012_glutStart10FANCY/glutStart.exe
    pathName.erase();
    pathName = pPath;
    //cout << "\n\n Given path:" << pathName << "\n\n" << endl;
    // Does the pathName end with our executable's filename? (progShade)?
    // SEE: www.cplusplus.com/reference/string/string/find_last_of
    found = pathName.find_last_of("/\\");   // Linux,Apple path? use "/"
                                            //Windows path? use "/\\"
    if(found != string::npos)
    {
        pathName = pathName.substr(0,found);// discard executable filename
        pathName += "\\";                    // replace the slash we removed...
    }
    cout << "\n\n Trimmed path:" << pathName << "\n\n" << endl;
#endif
}

void CShader::vertBareShader(void)
//------------------------------------------------------------------------------
// Discard current shader, replace with the simplest-possible vertex shader: the
// 'pass-thru' shader (pg 457 "Interactive Computer Graphics",Ed Angel,5th ed.)
{
    srcCode.erase();
    srcCode += "const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);\n";
    srcCode += "\n";
    srcCode += "void main()\n";
    srcCode += "{\n";
    srcCode += "    gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n";
    srcCode += "    gl_FrontColor = red;\n";
    srcCode += "}\n";
}

void CShader::fragBareShader(void)
//------------------------------------------------------------------------------
// Discard current shader, replace with the simplest-possible frag. shader: the
// 'pass-thru' shader (pg 4560 "Interactive Computer Graphics",Ed Angel,5th ed.)
{
    srcCode.erase();            // discard any existing source code, then
    srcCode += "void main()\n";
    srcCode += "{\n";
    srcCode += "    gl_FragColor = gl_Color; \n";
    srcCode += "}\n";
}

void CShader::readShaderFile(const char *fName)
//------------------------------------------------------------------------------
// Discard current shader, replace with source code read from the named file.
{
fstream inFile ;            // a C++ file-buffer object
string lineBuf;             // a C++ string-buffer for 1 line of file's text;
string nameBuf;             // holds pathName + fileName;

    fileName = fName;      // remember our path-free filename
    nameBuf = pathName + fileName;      // absolute file name (req'd for Apple)
	inFile.open (nameBuf.c_str(), ifstream::in);
	if(! inFile)
	{                          // can't open file? Complain
		cout << "\n\n!!------Could not open shader file: --------\n \t\t";
		cout << fName;
		cout << "\n  Full path: " << nameBuf << endl;
		cout <<   "\n--------------------------------------------\n Bye."<<endl;
		exit(EXIT_FAILURE);
	}
	srcCode.erase();                // discard any existing source code, and
	while (inFile.eof() != true)
	{
		getline(inFile,lineBuf);    // read one line from file into lineBuf
		lineBuf += '\n';            // append 'newline' to it,
		srcCode += lineBuf;	        // append the line to our source code;
	}
	inFile.close(); // close the stream.
}

void CShader::print(void)
//------------------------------------------------------------------------------
// Print shader's source-code in the text console window.
{
    cout << "\n-------Shader Listing--[file: ";
                                cout << fileName << "]------------" << endl;
    if(srcCode.length() <= 0)
         cout << " EMPTY:  NO SHADER SOURCE CODE!";
    else cout << srcCode << endl;
    cout << "\n---------------END shader END----------------\n" << endl;
}


/*==============================================================================
================================================================================
================================================================================
================================================================================

    CProgGLSL member functions


 2012.01.20 CProgGLSL class created -- paul olczak
 2012.01.30 Fix path errors on windows; add missing GLEW libraries; ANSI-std
              indenting; ANSI-std formal params (void); add lines to mark
              function starts,tutorial comments, replace generic identifiers

================================================================================
================================================================================
================================================================================
================================================================================
*/

CProgGLSL::CProgGLSL(void)
//------------------------------------------------------------------------------
// Default constructor
{
    programID = glCreateProgram();  // Ask openGL to reserve int. 'ID number' or
                                    //'name' to identify a new 'shader program'
                                    // we will load into the GPU.
                                    // (An openGL 'shader program' can contain
                                    // a vertex shader, fragment shader, & more)
}

CProgGLSL::~CProgGLSL(void)
//------------------------------------------------------------------------------
// Default destructor.
{

}


CProgGLSL::CProgGLSL(const char* myArgv0, string vertexShFileName,
                                            string fragmentShFileName)
//------------------------------------------------------------------------------
// Shader-filename-and-path-setting constructor;
// Set the 'myArgv0' pointer to the the value of argv[0] (the formal param of
// of your main() function). We use this string(exe-filename-with-full-path)
// to find the full path to your shader files.
//
// For example, suppose you keep your source-code and shader files in the
// directory 'myProj02', and build it with the CodeBlocks IDE.  CodeBlocks will
// put the executable file here: 'myProj02/Debug/bin/myProj.exe', and when
// we run the program, main() gives us that string as 'argv[0]'.
{
#if !defined(__APPLE__)
bool isOK;
    isOK = hardwareCheck();         // returns FALSE if this machine's openGL
                                    // implementation DOES NOT support GLSL;
    if(isOK==false)
    {
        cout << "\n!?!? your computer's openGL implementation\n";
        cout << "\t DOES NOT SUPPORT programmable shading (GLSL!).\nBye.\n";
        exit(1);
    }
#endif
    programID=glCreateProgram();    // Ask openGL to reserve an 'ID number' or
                                    //'name' to identify a new 'shader program'
                                    // we will load into the GPU.
                                    // (An openGL 'shader program' can contain
                                    // a vertex shader, fragment shader, & more)
    // MacOS requires the complete path+filename when we load in the
    vSh.setPath(myArgv0);            // vertex shader
    fSh.setPath(myArgv0);            // fragment shader

    vSh.fileName = vertexShFileName.c_str();
    fSh.fileName = fragmentShFileName.c_str();
}

#if !defined(__APPLE__)
 /* 'hardwareCheck()' member function is not available (and not necessary) on
 Apple MacOS, because it doesn't use the GLEW library that function requires.
 Apple Xcode supplies its own method (perhaps in the Xcode 'OpenGL' framework?)
 for linking with OpenGL's extended functions such as glCreateShaderObject().

 For Windows and Linux, after we link to the GLEW library, we call glewInit(),
 and then we can 'call-by-name' these extended functions, just like any other
 OpenGL function. GLEW performs the messy process of accessing OpenGL's extended
 functions. Each time you call an extended function, GLEW checks availability on
 your hardware; if it exists, GLEW then requests a function pointer to it, and
 calls the functions with the arguments you supplied in your call-by-name usage.
 */
int CProgGLSL::hardwareCheck(void)
//------------------------------------------------------------------------------
// Returns FALSE (0) if this machine's openGL implementation DOES NOT support
// GLSL (requires version 2.0 or later), otherwise returns TRUE (1).
{
    if (glewIsSupported("GL_VERSION_2_0"))
    {
        cout << "OpenGL 2.0 or better: READY for GLSL!" << endl;
        return 1;
    }
    else
    {
        cout << "OpenGL 2.0 not supported! NOT READY for GLSL!" << endl;
        return 0;
    }
}
#endif

void CProgGLSL::setVertexShPath(string path)
//------------------------------------------------------------------------------
// See the 'shader-file-loading constructor'
{
    vSh.setPath(path.c_str());
}

void CProgGLSL::setFragmentShPath(string path)
//------------------------------------------------------------------------------
{
    fSh.setPath(path.c_str());
}

void CProgGLSL::loadShaders(void)
//------------------------------------------------------------------------------
// this will load the shaders from their filenames & paths held in vSh,fSh.
{
    vSh.readShaderFile(vSh.fileName.c_str());
    vSh.print();    // print the code we got to std::cout. Not req'd for openGL,
                    // but it lets you check the code for correctness.

    // create a shader location in GPU for the vertex shader... and get its ID
    vSh.shaderObjID = glCreateShader(GL_VERTEX_SHADER);

    // Bind this shader to the GPU program ID# we created in the constructor;
    glAttachShader(programID, vSh.shaderObjID);

    // openGL doesn't understand the C++ 'string' class; instead you must send
    // it a pointer of type const char*.  Thus this:
    vSh.src = vSh.srcCode.c_str();

    // ****
    // We'll upload the shader code to the server (GL Card) using the following call.
    // ****

    // glShaderSource(<#GLuint shader#>, <#GLsizei count#>,
    //                      <#const GLchar **string#>, <#const GLint *length#>)
    // GLuint shader# is the shader ID number we just got.
    // count is how many progs are we sending (1 in this case)
    // the glchar** is the address of the const char* pointer-to-code just made
    // length is the length of the code,
    // but we can just tell it to go to the null terminator
    glShaderSource(vSh.shaderObjID, 1, &vSh.src, NULL);
    // steps on the fragment shader are exactly the same
    // except we specify that it's a fragment shader object:
    fSh.readShaderFile(fSh.fileName.c_str());
    fSh.print();            // show me the shader source code on-screen.
    fSh.shaderObjID = glCreateShader(GL_FRAGMENT_SHADER);
    glAttachShader(programID,fSh.shaderObjID);  // attach to the program
    fSh.src=fSh.srcCode.c_str();                // make pointer to source code,
    glShaderSource(fSh.shaderObjID, 1, &fSh.src, NULL); // upload the source.
}

void CProgGLSL::setVertexShFileName(string name)
//------------------------------------------------------------------------------
{
    vSh.fileName = name;
}

void CProgGLSL::setFragmentShFileName(string name)
//------------------------------------------------------------------------------
{
    fSh.fileName = name;
}

void CProgGLSL::compileProgram(void)
//------------------------------------------------------------------------------
{
    // we now need to compile, link and check for problems
GLint vertCompiled,fragCompiled;
    // compile the shaders
    glCompileShader(vSh.shaderObjID);
    glCompileShader(fSh.shaderObjID);

    // get the status of the compile
	glGetShaderiv(vSh.shaderObjID, GL_COMPILE_STATUS, &vertCompiled);
	glGetShaderiv(fSh.shaderObjID, GL_COMPILE_STATUS, &fragCompiled);

    if(!vertCompiled)
    {
        cout << "VERTEX shader [" << vSh.fileName;
        cout <<                     "] did not compile!" << endl;
        exit(EXIT_FAILURE);
    }
    if(!fragCompiled)
    {
        cout << "FRAGMENT shader [" << fSh.fileName;
        cout <<                     "] did not compile!" << endl;
        exit(EXIT_FAILURE);
    }
    // Now let's link the gl program
    glLinkProgram(programID);
    // and we'll by default say we want to use this new program
}

void CProgGLSL::useProgram(void)
//------------------------------------------------------------------------------
{
    // use the program associated with the shaders to render the next pixels.
    glUseProgram(programID);
}
