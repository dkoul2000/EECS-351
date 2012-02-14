//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/* (JT: I prefer to keep line lengths <=80 chars for easy printing/display).

            CByteImg.h
            CByteImg Class holds an image with 1 byte per 'field' or
            color component, such as grayscale (1 field), RGB, or RGBA,
            and is suitable for use with OpenGL. Includes C++ -based file
            I/O for platform-indep PPM and BMP files.

            NOTE: image stored at *pImg starts every scanline on 32-bit
            boundaries, and thus may include up to 3 'padding' bytes at the
            end of each scanline.  This ensures fastest-possible memory
            transfers on Intel i-86 architecture machines (Windows, Macs, etc)
            and for openGL transfers, use glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    ***CONFUSED? LOST?***
            Go to these files and start reading: glutPak.cpp and .h

    2011.09.27 -- J. Tumblin Created

*=============================================================================*/
// Guard Block (prevents cyclic includes)
#ifndef _CBYTEIMG_H
#define _CBYTEIMG_H

/*============================================================================*/
// multi-platform includes for the OpenGL/GLUT/freeGLUT libraries:
#if defined(_WIN32)
#include <GL/glew.h>        // OpenGL 'extensions wrangler' for easy access.
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

#include <stdint.h>         // C integer size definitions (uint32_t, etc)
                            //(C++ version not released/ready yet: 08/2011)
#include <cstdlib>          // for exit() function
#include <cstdio>           // C++ for fscanf(), sprintf(), etc.
#include <iostream>         // C++ streams support;
#include <iomanip>          // C++ for cin,cout formatting (setw(), etc)
#include <fstream>          // C++ file I/O streams
#include <string>           // C++ string-editing class;nice for filenames,etc.
#include <cmath>            // C++ math: for M_PI and trig functions
#include <cstring>          // ?!? fix student reported 'memcpy()' link error.
#include <string.h>


struct myBITMAPFILEHEADER
//------------------------------------------------------------------------------
// Only used within CByteImg::readBMPfile() and writeBMPfile().
// MS Windows 'BMP' (bitmap' file format begins with TWO header structs, stored
// in 'little-endian' form; integers with LSB first.
// This first one describes the file itself (bitmap*FILE*header)
// (The 'my' prefix prevents name collisions with Windows OS declarations).
// (uses C++ size-specifying types for portability, not 'unsigned short' etc.)
{
    uint16_t    bfType;          // 2 ASCII chars: 'BM', or 19778
	uint32_t	bfSize;         // 16 bits
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
};

struct myBITMAPINFOHEADER
//-----------------------------------------------------------------------------
// Only used within CByteImg::readFile() and writeFile().
// This second struct holds 'info' for the DIB ('device independent bitmap')
// data held in the BMP file; thus the 'bitmap*INFO*header name.
// (the 'my' prefix prevents name collisions with Windows OS declarations)
// (uses C++ size-specifying types for portability, not 'unsigned short' etc.)
{
	uint32_t biSize;                // size in bytes of this *info* header
	int32_t  biWidth;               // SIGNED w,h of image in pixels (unpadded)
	int32_t  biHeight;              // Height>0, width>0: lower-left origin
                                    // Height<0, width>0: upper-left origin.
	uint16_t biPlanes;              // number of color planes (MUST be 1)
	uint16_t biBitCount;            // number of bits per pixel
	uint32_t biCompression;         // type of data-compression (default: 0)
	uint32_t biSizeImage;           // total bytes for image (INCLUDES padding)
    int32_t  biXPelsPerMeter;       // display resolution (DPI)
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;             // number of colors in palette (if used)
	uint32_t biClrImportant;        // number of 'important' palette colors
};

class CByteImg
//==============================================================================
// CByteImg class defines an object that holds one image as a 2D array of
// pixels, each made of one or more adjacent 8-bit bytes, suitably arranged for
// use by openGL.  You can create these images using test-pattern generators
// (makeChecker(), makeColorRamps(), make Random() ), copy any desired
// rectangular set of pixels and set of pixel fields to another (you may need
// an intermediate CByteImg object to hold them (e.g. RGB->RGBA), and read/write
// any CByteImg object to a file using BMP or PPM format.  Its file I/O is
// limited: it reads/writes only two uncompressed BMP formats (1-byte grayscale
// 3-byte/pixel RGB, or 4-byte/pixel RGBA), and two uncompressed PPM formats
// (P3 'text-only' or P6 'raw bytes').
//
// The CByteImg coord. system (x,y) counts discrete integer pixel locations, has
// its origin (0,0) at the lowermost, leftmost pixel(bottom left corner) and
// increases upwards and rightwards. To access the pixel at (x,y) as a vector of
// bytes, use:          GLbyte *pPixelv;   // declare ptr-to-pixel-vector
//                      pPixelv = pImg + pixSiz*x + lineSiz*y;
// To access element 'e' in pixel vector pPixelv, use pPixelv[e]; for example,
//  e==0,1,2,3 to access R,G,B,A values, and pPixelv[2] holds B (blue) value.
//
// For a more thorough image file I/0 library, see FreeImage.sourceforge.net.
//
// This code DOES NOT depend on your machine's integer-storage format; it reads
// and writes files byte-by-byte to prevent different results on 'little-endian'
// (e.g. Intel/Windows) & 'big-endian' (e.g. Apple/PowerPC) machines. Note the
// BMP file format includes 16 and 32-bit signed and unsigned integers stored in
// files in little-endian format.
{
public:
int32_t xSiz, ySiz;         // image width,height measured in pixels.
int pixSiz;                 // number of bytes per pixel. Users can choose any
                            // pixSiz, but the most common uses are:
                            //  pixSiz == 1: grayscale 8-bit image (BMP or PPM)
                            //  pixSiz == 3: RGB,  8 bits per color (BMP or PPM)
                            //  pixSiz == 4: RGBA, 8 bits per color (BMP or PPM)
int32_t lineSiz;            // Number of bytes used to store one scanline; this
                            // is *NOT* always xSiz*pixSiz, because each
                            // scanline must begin on a 32-bit (DWORD)in memory,
                            // and thus scanlines often have 1 to 3 unused bytes
                            // at the end of each scanline.
int padding;                // number of 'extra' bytes added to the end of each
                            // scanline to fill it to 32-bit (DWORD) boundary.
GLbyte *pImg;               // Points to a dynamically-allocated array of bytes
// (is GLbyte undefined? )  // that holds 'ySiz' scanlines of length 'lineSiz'
// (use uint8_t instead, )  // bytes.  Each scanline holds 'xSiz'' pixels made
// (from <stdint.h>      )  // of 'pixSiz' adjacent bytes and arranged in RGB
                            // or RGBA order ('A', opacity value, has default
                            // value of 255).
                            // Every scanline begins on a 32-bit (DWORD)boundary
                            // in memory, and thus includes 0,1,2, or 3 extra
                            // unused bytes as 'padding' at the end of each
                            // scanline.  The Windows BMP image format REQUIRES
                            // this memory alignment; it ensures fastest access
                            // on Intel i-86 processors. By default, openGL also
                            // expects image buffers aligned to 32-bit bounds,
                            // just as you'll need it for use with CByteImg.
                            // You can FORCE openGL to 32-bit alignment using:
                            // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
                            // glPixelStorei(GL_PACK_ALIGNMENT, 4);
// File Specifiers:
fstream inFile;             // a C++ file-streaming object: our link to the OS.
string nameOfFile;          // File-name for this image INCLUDING its 3-letter
                            // file-extension; DOES NOT include the path.
string pathOfFile;          // full directory path; DOES NOT include filename.
string extnOfFile;          // 3-letter file extension (e.g. "BMP", "PPM")
                            // that selects our file format to read/write file.
string fullBuf;             // Absolute filename:  path+filename with extension.
// The askFileName() function fills 'fullBuf' and it's 3 sub-strings:
                            // e.g. for the file "C:\Img\Src\myImage.BmP"
                            //   it sets nameOfFile to "myImag.BmP",
                            //   it sets pathOfFile to "C:\Img\Src\"
                            //   it sets extnOfFile to "bmp" (all lowercase)

bool isTopDownFile;         // ==true if this image file stores the top-most
                            // scanline first (origin at upper left).
bool isBGRfile;             // ==true if this image file stores pixel colors
                            // in BGR or BGRA order instead of RGB or RGBA.
bool isTextFile;            // ==true for PPM format 'P3' files that store all
                            // pixel values as sequential ASCII-text integers.
                            // ==false for PPM format 'P6' files that store all
                            // pixel values as sequential bytes or byte-pairs.
uint8_t byteBuf[16];    // BYTES to/from file (immune to big/little-endian woes)
                        // ?!? why use uint8_t instead of char? C/C++ WIERDNESS!
                        // C/C++ typecast conversions from char to UNSIGNED int
                        // presumes 8-bit char values are SIGNED (!??!) and this
                        // can mess up byte packing/unpacking:
                        // char charBuf[16];
                        // uint16_t uns16;
                        //      infile.read(charBuf,2); // read 2 bytes
                        //      // suppose charBuf[0] is 0x90 (decimal 128)
                        //      uns16 = (uint16_t)charBuf[0];
                        //      cout << "uns16" << endl;
                        //      RESULT: 65408, or 0xff80 DESPITE 'unsigned' cast
                        // HOW TO FIX?  use either:
                        // unsigned char charBuf[16];  **OR**
                        // uint8_t charBuf[16];.
// For BMP files:
struct myBITMAPFILEHEADER fileHdr;      // two local struct objects that
struct myBITMAPINFOHEADER infoHdr;      // describe file contents.
int32_t tallTmp;                        // image height, positive-only.

public:
 CByteImg(void);            // default constructor, destructor
~CByteImg(void);

void askFileName(char *myArgv[]);
                            // Uses C++ stdin/stdout stream to ask users for
                            // the filename to read/write, then fills the string
                            // buffers nameOfFile, pathOfFile, and extnOfFile.
                            // (Be sure to set myArgv = argv; in main() fcn.).
void setFileName(char *myArgv[], int fileNum);
                            // Just like 'askFileName(), except it uses the
                            // default filename number 'fileNum' from its onw
                            // internal list of names, and doesnt' pester you
                            // with questions. Uses C++ stdin/stdout stream
                            // operators throughout, and sets proper values of
                            // buffers nameOfFile, pathOfFile, and extnOfFile.
                            // (Be sure to set myArgv = argv; in main() fcn.).
int readFile(void);         // Discard our current contents and replace it with
                            // the image in file described by the strings
                            // 'nameOfFile' and 'pathOfFile'.  Use the filename
                            // extension to select the file format (BMP,PPM,etc)
                            // Error? keep current contents, return 1; else 0).
int writeFile(void);        // Write our current image to the file described by
                            // the strings 'nameOfFile' and 'pathOfFile'. Note
                            // this call DESTROYS any previous file contents!
                            // Use the filename extension to select the file
                            // format (e.g. BMP,PPM, etc).
                            // Error? return 1; else return 0.

void drawPixelsGL(void);    // draw our current image on-screen in openGL at
                            // its current raster position (glRasterPos).

inline uint32_t pixIndex(int x, int y){return(x*pixSiz + y*lineSiz);};
                            // find pImg array index at the start of pixel x,y.
int sizer(int xmax, int ymax, int pixmax);
                            // Discard object's current image contents, and
                            // create (but do not init) a 'padded-scanline'
                            // buffer that holds ymax scanlines of xmax pixels
                            // made of pixmax bytes each.
                            // Returns 1 on error; 0 otherwise.
                            // To discard all memory safely, call sizer(0,0,0)

int copy(CByteImg *pSrc);   // Make a copy of the CByteImg object at pSrc.
// 'CopyBlock...()' functions let you crop/shift/padd any part of any image.
int copyOutBlock(CByteImg *pDest, int x0Src, int y0Src, int f0Src);
                            // Fill the entire pDest object with a rectangular
                            // block of our own pixel-field values beginning
                            // at (x0Src,y0Src,f0Src). (The 'field' is the
                            // byte# within a pixel:f0Src =0,1,2 for R,G,B, etc)
                            //  Return 0 on success or 1 on error (zero-sized
                            // pDest, or if pDest's size added to x0Src,y0Src,
                            // f0Src extends beyond our image limits (the
                            // function always fills as much of pDest as
                            // possible, and ignores the rest.
int copyInBlock(CByteImg *pSrc, int x0Dest, int y0Dest, int f0Dest);
                            // Copy the entire contents of pSrc image to
                            // replace a rectangular block of pixel-field
                            // values in our own image, starting at location
                            // (x0Dest,y0Dest,f0Dest).  Recall that a 'field'
                            // is the byte# within the pixel, so that
                            // f0Dest=0,1,2... for R,G,B,...etc).
                            // Return 0 on success, or 1 on error (zero-sized
                            // pSrc image, or pSrc image too large to fit in
                            // into our image at and above x0Dest,y0Dest,f0Dest.
// Make test images:
int makeConstant(GLbyte val);   // Set all bytes to 'val' in allocated memory.
                            // Return 1 if image is empty, else return 0.
int makeChecker(int blockSiz);  // Make a black-and-white checkerboard in the
                            // memory already allocated at pImg with uniform
                            // blocks (blockSize x blockSize). Returns 1 on
                            // error (e.g. empty buffer at pImg) else return 0.
int makeColorRamps(void);   // Make a 3-color set of differently-oriented
                            // RGB color ramps as test signal.  Returns 1 on
                            // error (e.g. empty buffer at pImg) else return 0.
int makeRandom(int blockSiz);   // Make randomly-colored squares to fill the
                            // image(blockSize x blockSize) in the memory
                            // already allocated at pImg.  Returns 1 on
                            // error (e.g. empty buffer at pImg) else retn 0.

//-----------------------------------------------------------------------------
//      Support Functions: you shouldn't ever need to call these yourself.
private:
int readTXTfile(void);      // read file using format rules for .TXT extension
int readPPMfile(void);      // read file using format rules for .PPM extension
int readBMPfile(void);      // read file using format rules for .BMP extension

int writeTXTfile(void);     // write file using format rules for .TXT extension
int writePPMfile(void);     // write file using format rules for .PPM extension
int writeBMPfile(void);     // write file using format rules for .BMP extension

void printBMPreport(void);  // print all the current BMP header information
void reportGLerr(const char *pIdent);   // print (and clear) the most-recent
                            // openGL error. (COPIED from CTheApp class)
void printImgContents(int xQuit, int yQuit);    // print all pixel values;
                            // Prints only the 1st 'xQuit' columns, 'yQuit' rows
                            // or prints ALL columns,rows if xQuit,yQuit == 0.

// In typical short-sighted Microsoft fashion, the BMP file format stores multi-
// byte integer values in little-endian form: it stores the least-significant
// byte first, most-significant last.  Many common machines (Apple, Sun, etc)
// use 'Big-endian' integer formats, so any portable 'BMP' file-reading code
// must read sets of bytes from the file, and then construct integers in the
// machine's native format before use.  Similarly, BMP file-writing code must
// convert machine-native integers to sets of little-endian byte sequences to
// read from or write to the file.
// The functions below pack/unpack bytes in little-endian order.  All accept
// only 16- or 32-bit UNSIGNED integers only to avoid any arithmetic sign
// complications when converting to little-endian bytes.  To read 16(32)-bit
// SIGNED values from a file (e.g. biWidth, biHeight), read 2(4) bytes from
// the file, call the right 'unpack' function to make an unsigned integer,
// then use a typecast to assign the returned value to a signed variable; e.g.
//    uint32_t unsigned32;
//    int32_t signed32;
//    uint8_t byteBuf[1024];            // BYTE array -- DON'T use 'char'
//                                      // or 'unsigned char'; not portable!
//          infile.read(byteBuf,4);     // read 4 bytes
//          unsigned32 = lilEndUnpack32( byteBuf);  // unpack,
//          signed32 = (int32_t)unsigned32;         // copy bits to signed var.

uint32_t lilEndUnpack32(const uint8_t *pSrc);
                            // construct an 32-bit unsigned integer from
                            // adjacent bytes of 'little-endian' file data
uint16_t lilEndUnpack16(const uint8_t *pSrc);
                            // construct a 16-bit unsigned integer from
                            // adjacent bytes of 'little-endian' file data
void lilEndPack32(uint8_t *pDest, const uint32_t src32);
                            // Fill byte-buffer pDest with bytes of the src32
                            // integer, (re)arranged into little-endian order.
void lilEndPack16(uint8_t *pDest, const uint16_t src16);
                            // Fill byte-buffer pDest with bytes of the src16
                            // integer, (re)arranged into little-endian order.

};

#endif // _CBYTEIMG_H
