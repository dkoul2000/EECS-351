//============================
//============================
//
//      CByteImg Class Member Functions
//
//============================
//============================

#include "CByteImg.h"           // header file with class definition.

 CByteImg::CByteImg(void)
//------------------------------------------------------------------------------
// Default constructor.     Set benign values for all data members.
{
    extnOfFile = "";
    fullBuf = "";
    isTextFile = false;
    isTopDownFile = false;
    isBGRfile = false;
    lineSiz = 0;
    nameOfFile = "";
    pathOfFile = "";
    pImg = NULL;
    pixSiz = 0;
    xSiz = 0;
    ySiz = 0;
}

CByteImg::~CByteImg(void)
//------------------------------------------------------------------------------
// Default destructor
{
    sizer(0,0,0);       // discard current contents, if any
}

void CByteImg::askFileName(char *myArgv[])
//------------------------------------------------------------------------------
// Uses C++ stdin/stdout stream to ask users for the filename to read or write;
// uses 'arv[0]' to get directory of the executable and find HOME directory
// (presumes HOME/debug/bin), then properly fills the 'nameOfFile', pathOfFile',
// and 'extnOfFile(lower-case only)' buffers.
{
int i;
size_t found;               // character counter used to parse strings
string userBuf;              // string for user's typed input

//    cout << "What's our current pathname? argv[0] holds the" << endl;
    pathOfFile.assign(myArgv[0]);
    nameOfFile = "";            // clear all other buffers.
    extnOfFile = "";
    userBuf = "";
//    cout << "entire path + this program's executable filename:" << endl;
//    cout << "\t" << pathOfFile << endl;
    // SEE: www.cplusplus.com/reference/string/string/find_last_of
    found = pathOfFile.find_last_of("\\/"); // count chars; find the last slash
                    // or backslash. Linux,Apple uses "/"; Windows uses "\\";
                    // WHY double-backslash? in C, backslash is a switch
                    // to say 'next char is special' (\n, \t, \b, etc).
    if(found != string::npos)   // if the last 'slash' IS NOT the last char in
    {                           // in the pathOfFile string, then:
        pathOfFile = pathOfFile.substr(0,found + 1);
                                // discard all chars AFTER the last slash.
                                // (strips away the executable's filename).
    }
    cout << "Directory that holds this CodeBlocks program's executable is:";
    cout << endl << "\t" << pathOfFile << endl;
    cout << "Get HOME from 2 directories above (e.g. remove /debug/bin/)" << endl;
    for(i=0; i<2; i++)
    {   // remove bottom-most subdirectory from path string:
        pathOfFile = pathOfFile.substr(0,found);
                                            // eliminate the terminating slash;
        found = pathOfFile.find_last_of("\\/"); // find the right-most remaining
                                                // slash or back-slash in string
        pathOfFile = pathOfFile.substr(0,found+1);  // discard all chars past it.
    }
    cout << "This CByteImg object will look for file(s) in this directory:";
    cout << endl << "\t" << pathOfFile << endl;
//=========================================
// ASSIGN a default file name:  (CHOOSE JUST ONE)
//=========================================
    //nameOfFile.assign("testText.txt");
    //nameOfFile.assign("textImage.ppm");   // text-only P3-style PPM
    //nameOfFile.assign("feepP3.ppm");      // tiny,externally-supplied P3 PPM
    //nameOfFile.assign("snailP3.ppm");     // larger externally-supplied P3 PPM
    //nameOfFile.assign("stop_2_P6.ppm");   // 'raw' P6 PPM (binary pixel values
    //nameOfFile.assign("mcfaddin_1.ppm");    // 'raw' P6 PPM
    //nameOfFile.assign("RedGreenBlue24bit.bmp");   // (243x146 x 24bpp: RGB)
    nameOfFile.assign("RedGreenBlue32bit.bmp");   // (243x146 x 32bpp: RGBA)
    //nameOfFile.assign("prim1.bmp");       // (640x480 x 24bpp) synthetic BMP file
    //nameOfFile.assign("chewy.bmp");       // (169w x 232h x 24bpp) BMP photo
                                            //  (from 2011IVMSP talk)
//=========================================
    cout << "Default filename:" << nameOfFile << endl;
    cout << "Enter new filename (RETURN for default): ";
    // Tempted to write 'cin >> nameOfFile;'? DON'T DO IT! Instead, read:
    //      http://www.cplusplus.com/forum/articles/6046/
    // tl:dr -- it's not type-safe; it can 'hang', so use getline() instead.
    getline(cin, userBuf);
    //cout << "userBuf.length():" << userBuf.length() << endl;
    if(userBuf.length()==0)
    {
        cout << "OK: using default filename: " << nameOfFile << endl;
    }
    else
    {
        nameOfFile = userBuf;
        cout << "Thanks. New filename is : " << nameOfFile << endl;
    }
    cout << "Found this extension on the filename (converted to lowercase):";
    cout << endl;
    found = nameOfFile.find_last_of(".");   // count chars; find last period.
    if(found != string::npos)   // if the last period is NOT the last char in
    {                           // the filename string, then
        extnOfFile = nameOfFile.substr(found+1, string::npos);
    }                            // copy all chars AFTER the period at 'found',
	for(i = 0; extnOfFile[i] != '\0'; i++)
	{   // convert all chars to lowercase
		extnOfFile[i] = tolower(extnOfFile[i]);
	}
    cout << "\t" << extnOfFile << endl;
    fullBuf = pathOfFile + nameOfFile;   // assemble absolute filename:
    cout << "Absolute Filename: " << fullBuf << endl;
}

void CByteImg::setFileName(char *myArgv[], int fileNum)
//------------------------------------------------------------------------------
// Same as 'askFilename()' fcn, but without all the pesky user prompting.
// Just pass it the filename. Uses C++ stdin/stdout stream operators throughout,
// and sets proper values of buffers nameOfFile, pathOfFile, and extnOfFile.
// (Be sure to set myArgv = argv; in main() fcn.).
{
int i;
size_t found;               // character counter used to parse strings
string userBuf;              // string for user's typed input

//    cout << "What's our current pathname? argv[0] holds the" << endl;
    pathOfFile.assign(myArgv[0]);
    nameOfFile = "";            // clear all other buffers.
    extnOfFile = "";
    userBuf = "";
//    cout << "entire path + this program's executable filename:" << endl;
//    cout << "\t" << pathOfFile << endl;
    // SEE: www.cplusplus.com/reference/string/string/find_last_of
    found = pathOfFile.find_last_of("\\/"); // count chars; find the last slash
                    // or backslash. Linux,Apple uses "/"; Windows uses "\\";
                    // WHY double-backslash? in C, backslash is a switch
                    // to say 'next char is special' (\n, \t, \b, etc).
    if(found != string::npos)   // if the last 'slash' IS NOT the last char in
    {                           // in the pathOfFile string, then:
        pathOfFile = pathOfFile.substr(0,found + 1);
                                // discard all chars AFTER the last slash.
                                // (strips away the executable's filename).
    }
    cout << "Directory that holds this CodeBlocks program's executable is:";
    cout << endl << "\t" << pathOfFile << endl;
/*    cout << "Get HOME from 2 directories above (e.g. remove /debug/bin/)" << endl;
    for(i=0; i<2; i++)
    {   // remove bottom-most subdirectory from path string:
        pathOfFile = pathOfFile.substr(0,found);
                                            // eliminate the terminating slash;
        found = pathOfFile.find_last_of("\\/"); // find the right-most remaining
                                                // slash or back-slash in string
        pathOfFile = pathOfFile.substr(0,found+1);  // discard all chars past it.
    }
//    cout << "This CByteImg object will look for file(s) in this directory:";
//    cout << endl << "\t" << pathOfFile << endl;
//=========================================
// SELECT a file from this default file name:  (CHOOSE JUST ONE)
//=========================================
*/
    switch(fileNum)
    {

        case 0: nameOfFile.assign("snailP3.ppm");
                // larger externally-supplied P3 PPM
                break;
        case 1: nameOfFile.assign("RedGreenBlue32bit.bmp");
                // RGBA test-signal image shows byte order(243x146 x 32bpp: RGBA)
                break;
        case 2: nameOfFile.assign("prim1.bmp");
                // RGB text signal image
                break;
        case 3: nameOfFile.assign("stop_2_P6.ppm");
                // 'raw' P6 PPM (binary pixel values) ?!? green stopsign.
                break;
        case 4: nameOfFile.assign("chewy.bmp");
                // candid photo at IVMSP Keynote looks odd to me.
                break;
        case 5: nameOfFile.assign("mcfaddin_1.ppm");
                 // 'raw' P6 PPM (binary pixel values) Cornell McFaddin Hall
                // WEIRD colors
            break;
        case 6: nameOfFile.assign("coffeeSmall01.bmp");
                // coffeetable camera input image
            break;
        case 7: nameOfFile.assign("coffeeSmall02.bmp");
                // coffeetable camera input image
            break;
        case 8: nameOfFile.assign("RedGreenBlue24bit.bmp");
                // RGB test-signal image shows byte order (243x146 x 24bpp:RGB)
            break;
/*        case 0: nameOfFile.assign("testText.txt");
                // text-only file I/0 test
            break;
        case 1: nameOfFile.assign("textImage.ppm");
                // text-only P3-style PPM
            break;
        case 2: nameOfFile.assign("feepP3.ppm");
                // tiny,externally-supplied P3 PPM
            break;
*/

        default:
            cout << "CByteImg::setFileName() ?!!? unknown file-number! Bye!";
            cout << endl;
            return;
            break;
    }

//=========================================
    cout << "Using filename selection # "<< fileNum<<": " << nameOfFile << endl;
    cout << "Found this filename extension (converted to lowercase):";
    cout << endl;
    found = nameOfFile.find_last_of(".");   // count chars; find last period.
    if(found != string::npos)   // if the last period is NOT the last char in
    {                           // the filename string, then
        extnOfFile = nameOfFile.substr(found+1, string::npos);
    }                            // copy all chars AFTER the period at 'found',
	for(i = 0; extnOfFile[i] != '\0'; i++)
	{   // convert all chars to lowercase
		extnOfFile[i] = tolower(extnOfFile[i]);
	}
    cout << "\t" << extnOfFile << endl;
    fullBuf = pathOfFile + nameOfFile;   // assemble absolute filename:
    cout << "Absolute Filename: " << fullBuf << endl;
}


int CByteImg::readFile(void)
//------------------------------------------------------------------------------
// Discard our current contents and replace it with the image in file described
// by the strings 'nameOfFile' and 'pathOfFile'.  Use the filename extension to
// select the file format (BMP,PPM,etc)
//       File opening error? keep current pImg contents, return 1; else 0).
{
string srcCode;                         // Buffer for contents of .txt file
    if(extnOfFile.find("txt") != string::npos)      // if 'txt' found anywhere,
    {
        return (readTXTfile());
    }
    else if(extnOfFile.find("ppm") != string::npos) // if 'ppm' found anywhere,
    {
        return (readPPMfile());
    }
    else if(extnOfFile.find("bmp") != string::npos) // if 'bmp' found anywhere,
    {
        return (readBMPfile());
    }
    else
    {
        cout << "?!?! Unknown file extension:" << extnOfFile << endl;
        cout << "Could not read. Bye!"  << endl;
        return 1;
    }
    return 0;
}

int CByteImg::writeFile(void)
//-----------------------------------------------------------------------------
// Write our current image to the file described by the strings 'nameOfFile' and
// 'pathOfFile'.  Note this call DESTROYS any previous file contents!  Use the
// filename extension to select the file format (e.g. BMP,PPM, etc).
// Error? return 1; else return 0.
{
    cout << "CByteImg::writeFile() -- stub! no code yet." << endl;
    return 0;
}

void CByteImg::drawPixelsGL(void)
//------------------------------------------------------------------------------
// draw current 'myImg' CByteImg image on-screen in openGL at its current
// raster position (use openGL's glRasterPos() and glDrawPixels() functions).
// EXAMPLE: glRasterPos3d(0.1, 0.2, 0.0);// set 3D position for any raster-ops
                                        // in the current coordinate system.
                                        // (CAREFUL! this position gets xformed
                                        // by GL_MODELVIEW and GL_PROJECTION
                                        // before use!
// !SURPRISE! if glRasterPos() puts lower-left corner OUTSIDE the CVV, then
// the ENTIRE IMAGE vanishes!  THUS be sure you've defined the matrices
// GL_MODELVIEW, GL_PROJECTION, and the viewport to ensure that for ALL desired
// image-display locations the lower-left corner will stay within the CVV.
//      HINT: don't use buffer-to-buffer transfers to re-position images
//          on-screen. You'll avoid this 'lower-left' problem and gain much
//          greater control of your images by displaying them as a texture map
//          surface defined by a grid of vertices (put 1 at each image corner,
//          or make a grid with more points to permit you to 'warp' the image.)
{
    if(pImg==NULL || xSiz*ySiz*pixSiz==0)
    {
        cout << "CByteImg::drawPixelsGL holds no valid image. Bye!" << endl;
        return;              // no image? zero-sized? do nothing.
    }
    switch(pixSiz)
    {
    case 3:
        glDisable(GL_BLEND);            // No alpha for this image, so draw
                                        // it into buffer as opaque.
        glDrawPixels(xSiz,ySiz,         // image width, height in pixels
                     GL_RGB,            // Pixel size (GL_RGB or GL_RGBA)
                     GL_UNSIGNED_BYTE,  // data type for each pixel component,
                     pImg);             // pointer-to-pixel memory array.
        break;
    case 4:                             // has ALPHA channel, so enable blending
        glEnable (GL_BLEND);            // to mix this image with existing
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // buffer contents.
        glDrawPixels(xSiz,ySiz,         // image width, height in pixels
                     GL_RGBA,           // Pixel size (GL_RGB or GL_RGBA)
                     GL_UNSIGNED_BYTE,  // data type for each pixel component,
                     pImg);             // pointer-to-pixel memory array.
        break;

    default:
        cout << "CByteImg::drawPixelsGL puzzled by pixSiz:" << pixSiz;
        cout << " Bye!" << endl;
        break;
    }
}

int CByteImg::sizer(int xmax, int ymax, int pixmax)
//------------------------------------------------------------------------------
// Discard object's current image contents, and create (but do not initialize)
// a buffer that holds xmax,ymax pixels of pixmax bytes each.
// NOTE THAT the buffer ALWAYS start on a 32-bit (DWORD) boundary, and
// NOTE THAT each scanline also always starts on a 32-bit (DWORD) boundary.
// THUS we must append a few zero-valued bytes to the end of each scanline
// whose length in bytes (xSiz*pixSiz) is NOT a multiple of four. This padding
//  means you must ALWAYS use 'lineSiz' when computing pixel addresses
// (see CByteImg::pixAddr() because often lineSiz > xSiz*pixSiz.
// True, it's an ugly practice, but:
//      --it's REQUIRED for Windows BMP-format images,
//      --it ensures fastest memory access on Intel i-86 processors,
//      --it's OpenGL's default setting: glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
// SEE: http://www.opengl.org/resources/features/KilgardTechniques/oglpitfall/
// Returns 1 on error; 0 otherwise.
// **** To discard all memory safely, call sizer(0,0,0); ****
{
uint32_t scanlinemax;       // # of bytes/scanline for new image
uint32_t *pDword = NULL;    // 32-bit pointer to newly-allocated image buffer

    if(xmax==0 || ymax==0 || pixmax==0)
    {                       // if user asked for a zero-sized image, AND
        if(pImg != NULL)    // we currently hold a non-zero-sized image,
        {
            delete [] pImg; // DISCARD the existing buffer,
            pImg = NULL;    // point safely to the 'empty pointer' location
        }
        xSiz = ySiz = pixSiz = lineSiz = 0; // clear previously-held values
        return 0;
    }
    // Otherwise, try to make a new image buffer with properly computed padding:
    padding = (xmax * pixmax)%4;    // # of bytes in one scanline that extends
                                    // PAST the last DWORD boundary; this will
                                    // always be either 0,1,2 or 3.
    padding = (4 - padding)%4;      // # of bytes to add to extend scanline to
                                    // the next DWORD boundary: 0,3,2 or 1.
    scanlinemax = padding + xmax*pixmax;             // # bytes per scanline.
    if(scanlinemax%4 != 0)
    {
        cout << "\n?!?! CByteImg::sizer() PADDING FAILURE?!?\n" << endl;
    }
    pDword = new (nothrow) uint32_t [ymax*scanlinemax/4];
    if(pDword==NULL)
    {
        cout << "\n ?!CByteImg::sizer() can't allocate memory!?!?\n"<< endl;
        return 1;           // STOP! Complain, but keep our old bitmap, if any.
    }
    else
    {                       // discard/replace any existing contents of pImg:
        if(pImg != NULL) delete [] pImg;    // discard any existing image buffer
        pImg = (GLbyte *)pDword;    // and point to the new one we just created.
                                    // copy the buffer's size to CByteImg object
        pixSiz = pixmax;            // # bytes per pixel
        lineSiz = scanlinemax;      // # bytes needed to store 1 scanline
        xSiz = xmax;                // # pixels per scanline
        ySiz = ymax;                // # scanlines in the image
        cout << "CByteImg::sizer() made xSiz,ySiz,pixSiz,lineSiz: ";
        cout << xSiz << ", " << ySiz << ", " << pixSiz << ", "<< lineSiz <<endl;
        cout << "\t for filename: " << nameOfFile << endl;
    }
    return 0;           // no error.
}

int CByteImg::copy(CByteImg *pSrc)
//------------------------------------------------------------------------------
// Discard current image contents, and replace it with a copy of the image
// found at pSrc.  (Note: does NOT copy file descriptors, etc).
{
int j;
int32_t indx;

    if(pSrc->xSiz * pSrc->ySiz * pSrc->pixSiz ==0)
    {
        cout << "CByteImg::copyAddAlpha() given zero-sized source. Bye!";
        cout << endl;
        return 1;
    }
    sizer(pSrc->xSiz, pSrc->ySiz, pSrc->pixSiz);    // (sets linesiz too!)
    for(j=0; j<ySiz; j++)
    {
        indx = pixIndex(0,j);        // array index for scanline j;
        memcpy(pImg+indx, pSrc->pImg + indx, lineSiz);  // copy one scanline.
    }
    return 0;
}


int CByteImg::copyOutBlock(CByteImg *pDest, int x0Src, int y0Src, int f0Src)
//------------------------------------------------------------------------------
// Fill the ENTIRE pDest object with a rectangular block of pixel-field values
// copied from our own image. (The 'field' is the byte# within a pixel:
// f0Src =0,1,2 for R,G,B, etc)
// The block starts at our pixel (x0Src,y0Src) and field f0Src; we copy that
// value to pDest image at pixel (0,0) field 0, and continue copying as pixel
// x,y and field numbers grow to fill the entire pDest image.
// Return 0 on success, or 1 on error (zero-sized pDest, or if pDest size added
// to x0Src,y0Src,f0Src extends beyond our image limits (the function always
// fills as much of pDest as possible, and ignores the rest.
{
int i,j,k, imax, jmax, kmax;
int32_t srcIndx, destIndx;

    kmax = pDest->pixSiz;               // Copy 'kmax' fields per pixel,
    imax = pDest->xSiz;                 // and 'imax' pixels per scanline,
    jmax = pDest->ySiz;                 // and 'jmax' scanlines to pDest image,
                                        // reading them from our (src) image.
    if(imax * jmax * kmax ==0)
    {
        cout << "CByteImg::copyOutBlock() given zero-sized pDest image. Bye";
        cout << endl;
        return 1;
    }
    // but first, be sure the source image ACTUALLY HAS that many pixels/fields
    // to read past the chosen starting point (x0Src,y0Src,f0Src);
    if(pDest->pixSiz > (pixSiz - f0Src))// if our (source) image isn't DEEP
        kmax = pixSiz - f0Src;          // enough (past the f0Src field) to
        // fill the full depth of pDest image, set kmax to take what we have.
    if(pDest->xSiz > (xSiz-x0Src))      // if our (source) image isn't WIDE
        imax = xSiz - x0Src;            // enough...
    if(pDest->ySiz > (ySiz-y0Src))      // if our (source) image isn't TALL
        jmax = ySiz - y0Src;            // enough...
    for(j=0; j < jmax; j++)             // for each scanline of the copied block
    {
        for(i=0; i < imax; i++)         // for each pixel in those scanlines
        {
            srcIndx = pixIndex(x0Src+i, y0Src+j);   // array index, source image
            destIndx = pDest->pixIndex(i,j);        // array index, pDest image
            for(k=0; k < kmax; k++)
            {
                // Copy k-th field
                pDest->pImg[destIndx + k] = pImg[srcIndx + f0Src + k];
            }
        }
    }
    return 0;       // normal exit.
}

int CByteImg::copyInBlock(CByteImg *pSrc, int x0Dest, int y0Dest, int f0Dest)
//-----------------------------------------------------------------------------
// Copy the ENTIRE contents of pSrc image into a rectangular block of our own
// pixel-field values in our own image (a 'field' is the byte# within the
// pixel, so that f0Dest=0,1,2... for R,G,B,...etc).  The over-written block of
// values in our image starts at pixel (x0Dest,y0Dest) and field f0Dest, and
// extends outwards (increasing x,y,f) until we've copied ALL pixels from pSrc.
// Return 0 on success, or 1 on error: zero-sized pSrc image, or pSrc image was
// too large to fit in into our own image starting from  x0Dest,y0Dest,f0Dest.
{
int i,j,k, imax, jmax, kmax;
int32_t srcIndx, destIndx;

    kmax = pSrc->pixSiz;                // Copy 'kmax' fields per pixel,
    imax = pSrc->xSiz;                  // and 'imax' pixels per scanline,
    jmax = pSrc->ySiz;                  // and 'jmax' scanlines to pDest image,
                                        // reading them from our (src) image.
    if( imax * jmax * kmax ==0)
    {
        cout << "CByteImg::copyInBlock() given zero-sized pSrc image. Bye";
        cout << endl;
        return 1;
    }
    // but first, be sure our destination image can actually store that many
    // pixels/fields past the chosen starting point (x0Dest,y0Dest,f0Dest);
    if(pSrc->pixSiz > (pixSiz - f0Dest))// if our (destination) image isn't DEEP
        kmax = pixSiz - f0Dest;         // enough (past the f0Dest field) to
        // hold the full depth of pSrc image, set kmax to take what we have.
    if(pSrc->xSiz > (xSiz-x0Dest))      // if our (destination) image isn't WIDE
        imax = xSiz - x0Dest;           // enough...
    if(pSrc->ySiz > (ySiz-y0Dest))     // if our (destinaion) image isn't TALL
        jmax = ySiz - y0Dest;           // enough...
    // Diagnostic printout:--------
    cout << "CByteImg::copyInBlock() copied into file "<< nameOfFile << " (";
    cout << imax << ", " << jmax << ", " << kmax << ")" << endl;
    cout << "\t of pSrc image with size (";
    cout << pSrc->xSiz << ", " << pSrc->ySiz << ", " << pSrc->pixSiz << ")."<<endl;
    //--------------------------
    for(j=0; j < jmax; j++)             // For each scanline of the source block
    {
        for(i=0; i < imax; i++)         // for each pixel in those scanlines
        {
            srcIndx = pSrc->pixIndex(i,j);          // array index, source image
            destIndx = pixIndex(i+x0Dest, j+y0Dest);// array index, pDest image
            for(k=0; k < kmax; k++)
            {
                // Copy k-th field; ALL of src to f0Src offset of dest.
                pImg[destIndx + f0Dest + k] = pSrc->pImg[srcIndx + k];
            }
        }
    }
    return 0;       // normal exit.
}

int CByteImg::makeConstant(GLbyte val)
//-----------------------------------------------------------------------------
// Set all bytes to 'val' in alloc'd memory. Return 1 if image is empty, else 0
{
int i,j,k;
int32_t indx;

    if(xSiz*ySiz*pixSiz ==0)
    {
        cout << "CByteImg::makeConstant() given zero-sized image. Bye" << endl;
        return 1;
    }
    // Diagnostic printout:----------------------
    cout << "CByteImg::makeConstant(" << val << ") called for " << nameOfFile;
    cout << "with x,y,pixSize: ";
    cout << xSiz << ", " << ySiz << ", " << pixSiz << endl;
    //-------------------------------------------
    for(j=0; j<ySiz; j++)
    {
        for(i=0; i<xSiz; i++)
        {
            indx = pixIndex(i,j);        // find buffer address for pixel i,j
            for(k=0; k<pixSiz; k++)
            {
                pImg[indx + k] = val;
            }
        }
    }
    return 0;       // normal exit.
}


int CByteImg::makeChecker(int blockSiz)
//------------------------------------------------------------------------------
// Make a black-and-white checkerboard in the memory already allocated at pImg
// with uniform blocks (blockSize x blockSize). Returns 1 on  error (e.g. buffer
// at pImg smaller than blockSiz x blockSiz), else return 0.
{
int i,j;        // pixel coords at lower-left corner of each checkerboard block
int ib,jb,oddEven;  // checkerboard-block labeling coordinates;
int s,t;            // pixel coordinates WITHIN each checkerboard block.
int k;              // byte number within a pixel
int32_t indx;      // image array index

    if(blockSiz < 0) blockSiz = -blockSiz;      // positive only.
    if(blockSiz == 0 || xSiz*ySiz*pixSiz == 0)
    {
        return 1;
        cout << "\n!?!? CByteImg::makeChecker() zero size!?!?\n" << endl;
    }
    // Count each block in the checkerboard using (ib,jb) coordinates; (0,0) for
    // the left-most, lowermost block, increasing rightwards & upwards.
    // Find pixel position (i,j) at the corner of each checkerboard block,
    // then fill in that block with black or white.
    for(j=0,jb=0; j<ySiz; j+=blockSiz,jb++)    // for every row of blocks,
    {
        for(i=0, ib=0; i<xSiz; i+=blockSiz,ib++) // and every block on that row,
        {
            oddEven = (ib + jb)%2;   // find alternating 0,1 values for blocks.
            // use (s,t) to scan thru all pixels in the block that can fit
            // inside the bitmap.
            for(t=0; t<blockSiz && (t+j)<ySiz; t++)
            {   // for all rows of this block (stay within image: t+j<ysize)
                for(s=0; s<blockSiz && (s+i)<xSiz; s++)
                {   // and all pixels within this row of this block
                    // (stay the limits of the image: s+i < xsize)
                    indx = pixIndex(s+i, j+t);   // find this pixel's address
                    for(k=0; k<pixSiz; k++)    // Write the color
                    {
                        pImg[indx+k] = 255 * (GLbyte)oddEven;  // 0 or 255.
                    }
                }
            }
        }
    }
    return 0;

}

int CByteImg::makeColorRamps(void)
//------------------------------------------------------------------------------
// Make an RGB color (pixSiz==3) test pattern by differently-oriented repeating
// ramps in each colorchannel.  Returns 1 if image size is <= 0.
{
int i,j;
int32_t indx;

    if( xSiz<=0 || ySiz <= 0 || pixSiz <= 0)
    {
        cout << "CByteImg::makeColorRamps() can't make zero-sized image--Bye!";
        cout << endl;
        return 1;
    }
    if(pixSiz != 3 && pixSiz !=4)
    {
        cout << "CByteImg::makeColorRamps() makes L, RGB, or RGBA pixels only -- Bye!";
        cout << endl;
        return 1;
    }

    for(j=0; j<ySiz; j++)     // for each image row,
    {
        for(i=0; i<xSiz; i++)  // for each pixel on that row,
        {
            indx = pixIndex(i,j);       // get array index for pixel (i,j)
            switch(pixSiz)
            {
                case 8:     // start here for 16-bit RGBA:
                case 6:     // start here for 16-bit RGB:
                    cout << "CByteImg::makeColorRamps() 16-bit not done yet.";
                    cout << endl;
                    break;
                case 4:     // start here for 8-bit RGBA:
                    pImg[indx + 3] = (((67 -3*i -2*j)%79)*255)/79;      // alpha
                case 3:     // start here for 8-bit RGB:
                    pImg[indx + 2] = (((17        -j)%41)*255)/41;      // blu
                case 2:     // start here for 16-bit grayscale:
                    pImg[indx + 1] = (((       i + j)%31)*255)/31;      // grn
                case 1:     // start here for 8-bit grayscale:
                    pImg[indx    ] = (((43 +i -2*j)%53)*255)/53;        // red
                    break;
                default:
                    cout << "CByteImg::makeColorRamps() You should NEVER see this!";
                    cout << endl;
                    break;
            }
        }
    }

    return 0;
}

int CByteImg::makeRandom(int blockSiz)
//------------------------------------------------------------------------------
// Make randomly-colored squares to fill the image(blockSize x blockSize) in
// the memory already allocated at pImg.  For individually-random pixels, set
// blockSiz==1.  Return 1 on error, else 0.
{
int i,j;        // pixel coords at lower-left corner of each checkerboard block
int ib,jb;          // checkerboard block coordinates;
int s,t;            // pixel coordinates WITHIN each checkerboard block.
int k;              // color component within a pixel
int32_t indx;      // image array index
GLbyte colr[pixSiz];// randomly-chosen color (RGBA)

    if(blockSiz < 0) blockSiz = -blockSiz;  // OOPS! fix this negative size!
    if(blockSiz == 0 || xSiz*ySiz*pixSiz <= 0 || pixSiz > 4)
    {
        return 1;
        cout << "\n!?!? CByteImg::makeRandom() size wierdness!?!?\n" << endl;
    }
    // Count each colored block using (ib,jb) coordinates; (0,0) for
    // the left-most, lowermost block, increasing rightwards & upwards.
    // Find pixel position (i,j) at lower-left corner of each color block,
    // then use (s,t) coords to fill in that block with a random color.
    for(j=0, jb=0; j<ySiz; j+= blockSiz, jb++)    // for every row of blocks,
    {
        for(i=0, ib=0; i<xSiz; i+= blockSiz, ib++) // and every block on that row,
        {
            for(k=0; k<pixSiz; k++)    // Choose a random color for this block
            {
                colr[k] = (GLbyte)(rand()%256);
                // between 0 and 255
            }
            for(t=0; t<blockSiz && (t+j)<ySiz; t++)
            {   // for all rows of this block (stay within image: t+j<ysize)
                for(s=0; s<blockSiz && (s+i)<xSiz; s++)
                {   // and all pixels within this row of this block
                    // (stay the limits of the image: s+i < xsize)
                    indx = pixIndex(s+i, j+t);   // find this pixel's address
                    for(k=0; k<pixSiz; k++)     // Write the color
                    {
                        pImg[indx+k] = colr[k];    // block's random color.
                    }
                }
            }
        }
    }
    return 0;
}

int CByteImg::readTXTfile(void)
//-----------------------------------------------------------------------------
// read file using format rules for .TXT extension: called by readFile() only!
{
string srcCode;                         // Buffer for contents of .txt file
string txtLineBuf;                      // one line of text read from file.

    cout << "File extension 'txt' : ASCII text file." << endl;
    cout << "First, open the file for reading: \n" << endl;
    //inFile.open(nameOfFile.c_str(), ifstream::in); // open(filename,mode);
    inFile.open(fullBuf.c_str(), ifstream::in);     // open(filename,mode(s));
    if(inFile.is_open())
    {
        cout << "\n----SUCCESS: opened file---" << endl;
        cout << "\t" << nameOfFile << endl;
        cout << "---------for reading.--------" << endl;
    }
    else
    {
        cout << "\n\n!!------Could not open this file: --------\n";
        cout << "\n\t" << fullBuf << endl;
        cout << "\n--------------------------------------------\n Bye." << endl;
        // exit(EXIT_FAILURE);
        return(1);
    }
    cout << "Read in ALL lines of Text..." << endl;

    while(inFile.eof() != true)     // if we're not at the end-of-file then;
    {
        getline(inFile,txtLineBuf); // read one line of text from file;
        srcCode += txtLineBuf;	    // append the line to our source code;
        srcCode += "\n";            // append a newline character
    }
    cout << "=======TEXT WE READ FROM FILE:========" << endl;
    cout << srcCode;
    cout << "======================================" << endl;
    return 0;
}

int CByteImg::readPPMfile(void)
//-------------------------------------------------------------------------------
// read file using format rules for .PPM extension; called by readFile() only!
{
int ysize, xsize, yy, xx;   // file image size in row, columns; indices
int pixmax;                 // RGB pixel value range: 0 to pixmax.
int ri,gi,bi;               // integer RGB pixel values read from file.
int cnt;                    // sscanf() arguments-converted counter.
int32_t indx;               // array index of pixel xx,yy in pImg array.
string txtLineBuf;          // one line of text read from the opened file.

    cout << "File extension 'ppm' : text-format img(Portable Pixel Map) file.";
    cout << endl << "First, open the file for reading: \n" << endl;
    inFile.open(fullBuf.c_str(), ifstream::in | ifstream::binary);
    // open(filename,mode);  Why 'binary'? The 'P3' (text-only) files hold
    // only text, but opening in binary mode won't hurt anything. In the
    // 'P6' (raw) files, we read a text header, but then need to read the
    // image in binary, so we MUST open file in binary mode.
    if(inFile.is_open())
    {
        cout << "\n----SUCCESS: opened file---" << endl;
        cout << "\t" << nameOfFile << endl;
        cout << "---------for reading.--------" << endl;
    }
    else
    {
        cout << "\n\n!!------Could not open this file: --------\n";
        cout << "\n\t" << fullBuf << endl;
        cout << "\n--------------------------------------------\n Bye." << endl;
//        exit(EXIT_FAILURE);
        return 1;
    }
    //.....................PPM file format..........................
    getline(inFile, txtLineBuf);    // read in the 1st line of text:
    cout << "First line (text) in PPM file:" << endl;
    cout << "\t" << txtLineBuf << endl;
    if(txtLineBuf.find("P3") != string::npos) // Does line of text contain 'P3'?
    {                                         // YES: text-only formatted image.
        cout << "Text-only color PPM file (P3-type; unusual, wasteful).";
        cout << "Its file header with comments(if any) is:" << endl;
        do  // read and print each line of the file's header:
        {
            cout << txtLineBuf << endl; //  Print it,
            getline(inFile, txtLineBuf);//  Read next line of text,
        }
        while(txtLineBuf[0]=='#');      // STOP if it's not a 'comment' line;
                                        // (comments begin with '#' char);
        // from this first non-comment line, read cols, rows, pixmax values.
        cnt = sscanf(txtLineBuf.c_str(), " %d %d %d", &xsize, &ysize, &pixmax);
        switch(cnt)                 // how many variables did sscanf() set?
        {
            case 2:                 // only read 2 of three? Usually this means
                inFile >> pixmax;   // the NEXT line contains pixmax...
            case 3:                 // read them ALL properly; done.
                if(pixmax <0 || pixmax > 65535) // usually <=255 or <=2^8 -1
                {
                    cout << "ILLEGAL PPM P3 pixmax value: " << pixmax;
                    cout << endl << "(typ. 0 to 255 (8bpp)";
                    cout << " or 0 to 65535 (16 bit))" << endl;
                    inFile.close();
                    return 1;
                }
                break;              // pixmax was read properly; done.
            default:
                cout << "readPPMfile(), P3 format: I can't read all the file's ";
                cout << " size-specifying text: cnt= " << cnt << ". Bye!" << endl;
                inFile.close();
                return 1;
                break;
        }
        // Set CByteImg size, so it can properly store all the pixels.
        // NOTE!  we only store 1 byte per color, even if the file holds 16-bit
        // RGB color values.
        sizer(xsize,ysize,3);       // we'll store RGB values.
        isTopDownFile = true;       // (true for all PPM files)
        isBGRfile = false;          // (for the P3 PPM files I've seen)
        cout << "Image size (x,y)==(" << xsize << ", " << ysize << ")" << endl;
        cout << "Image RGB pixel values from 0 to " << pixmax << endl;
        cout << "now read in the RGB pixel triplets stored as text:" << endl;
        cout << "------------------------------------------------" << endl;
        for(yy=ySiz-1; yy>=0; yy--)    // from top scanline downwards,
        {
            for(xx=0; xx<xSiz; xx++)   // for each pixel in this scanline
            {
                //cout << "Pixel (x,y)=(" << xx << ", " << yy << ")\t";
                inFile >> ri;           // Read ASCII text integer from file
                inFile >> gi;
                inFile >> bi;
                // cout << ri << ", " << gi << ", " << bi << endl;
                // 0 <= ri,gi,bi <= pixmax; and we want to map these values
                // uniformly between 0-255. If we use R = (int)(255.0*(ri/pixmax)),
                // the value of R will drop from 255 to 254 when ri falls even
                // the tiniest amount below pixmax, because type-cast (int)
                // maps 255.0 --> 255, and 254.99999 --> 254. Adding in 0.5
                // instead of 255.0 ensures we get the 254/255 transition when
                // (ri/pixmax) crosses the (254.5/255.0) value:
                indx = pixIndex(xx,yy);
                pImg[indx    ] =
                            (uint8_t)((0.5 + 255.0*(double)ri)/(double)pixmax);
                pImg[indx + 1] =
                            (uint8_t)((0.5 + 255.0*(double)gi)/(double)pixmax);
                pImg[indx + 2] =
                            (uint8_t)((0.5 + 255.0*(double)bi)/(double)pixmax);
            }
        }
    }
    else if(txtLineBuf.find("P6") != string::npos)
    {   // Does line of text contain 'P6'?  YES; text header, binary image
        cout << "Text/Binary PPM file (P6-type: stores 1 or 2 bytes/color).";
        cout << " File header with comments (if any):" << endl;
        do  // read and print each line of the header:
        {
            cout << txtLineBuf << endl; //  Print it,
            getline(inFile, txtLineBuf);//  Read next line of text,
        }
        while(txtLineBuf[0]=='#');      // STOP if it's not a 'comment' line;
                                        // (comments begin with '#' char);
// BETTER WAY TO IMPLEMENT:
// 1) make a 'comments' class; create a commentObj object:
// 2) overload the << and >> operators for ifstream to read/write
// a 'comments' object (checks for leading #),
// 3) Write this very simple, very clear header-reading code:
//      uint8_t magic[2]; // 2 ASCII bytes
//      inFile >> magic >> commentObj >> rows >> cols >> pixmax;
// 4) make an 'image' class; create an image object 'imObj' of
//      the correct size and type from the header info;
// 5) overload the << and >> operators for ifstream to read/write
//      an image object (looping to read text/bytes/byte pairs
//      depending on P3, P6, rows, cols, pixmax)
// 6) write this very simple, very clear image-reading code:
//      inFile >> imObj;
// (Note that the ENTIRE FILE READER is just steps 3,6!)
        cnt = sscanf(txtLineBuf.c_str(), " %d %d %d", &xsize, &ysize, &pixmax);
        switch(cnt)                 // how many variables did sscanf() set?
        {
            case 2:                 // only read 2 of three? Usually this means
                inFile >> pixmax;   // the NEXT line contains pixmax...
                                    // pixmax was read properly; done.
            case 3:                 // read them ALL properly; done.
                if(pixmax <0 || pixmax > 65535) // usually <=255 or <=2^8 -1
                {
                    cout << "ILLEGAL PPM P6 pixmax value: " << pixmax;
                    cout << endl << "(typ. 0 to 255 (8bpp)";
                    cout << " or 0 to 65535 (16 bit))" << endl;
                    inFile.close();
                    return 1;
                }
                break;
            default:
                cout << "readPPMfile(), P6 format: But I can't read all the file's ";
                cout << " size-specifying text. Bye!" << endl;
                inFile.close();
                return 1;
                break;
        }
        cout << "Image size (x,y)==(" << xsize << ", " << ysize << ")" << endl;
        cout << "Image RGB pixel values from 0 to " << pixmax << endl;
        cout << "now read RGB pixel triplets:" << endl;
        cout << "------------------------------------------------" << endl;
        // Set our CByteImg object's size:
        sizer(xsize,ysize,3);           // RGB image; always top-down file
        isTopDownFile = true;           // (origin at upper left of image)
        isBGRfile = true;               // for the PPM P6 files I've seen.
        if(pixmax <= 255)               // read ONE BYTE per pixel color:
        {
            if(pixmax !=255)
            {
                cout << "!?!? WEIRD PPM file: pixmax = " << pixmax << endl;
            }
            for(yy= ySiz-1; yy >= 0; yy--)  // from last scanline to the first,
            {
                for(xx=0; xx<xSiz; xx++)   // for each pixel on this scanline,
                {
                    indx = pixIndex(xx,yy);  // find index where pixel starts,
                    //cout << "8-8-8 bit Pixel (x,y)=(" << xx << ", " << yy << ")\t";
                    inFile.read((char *)byteBuf,3);  // read unformatted data
                    if(inFile.good() )
                    {
                    // SURPRISE! PPM format *ALSO* stores bytes in BGR order:
                    // swap them for RGB order
                    pImg[indx    ] = byteBuf[2];    // R
                    pImg[indx + 1] = byteBuf[1];    // G
                    pImg[indx + 2] = byteBuf[0];    // B
                    // read in r,g,b bytes.
                    //    ri = (int)byteBuf[2];     // R,
                    //    gi = (int)byteBuf[1];     // G,
                    //    bi = (int)byteBuf[0];     // B.
                    //cout << ri << ", " << gi << ", " << bi << endl;
                    }
                    else
                    {
                        cout << "PPM P6 file bad?" << endl;
                        break;
                    }
                }
            }
        }
        else            // pixmax>255, so we read TWO BYTES per pixel color:
        {               // 'Big-endian'--Most-signif. byte first.
            cout << "Read in a 16-bit PPM P6-format file." << endl;
            for(yy= ySiz-1; yy>=0; yy--)      // from last scanline to first,
            {
                for(xx=0; xx<xsize; xx++)   // for each pixel on this scanline
                {
                    indx = pixIndex(xx,yy);
                    //cout << "16-16-16 bit Pixel (x,y)=(" << xx << ", " << yy << ")\t";
                    inFile.read((char *)byteBuf,6); // read unformatted data;
                                                    // (stored little-endian)
                    //ri = (int)lilEndUnpack16(byteBuf);  // blu
                    //gi = (int)lilEndUnpack16(byteBuf+2);// grn
                    //bi = (int)lilEndUnpack16(byteBuf+4);// red
                    //cout << ri << ", " << gi << ", " << bi << endl;
                    // copy the MOST-SIGNIFICANT byte of each 16-bit pixel value
                    // SURPRISE!  PPM P6 format files hold BGR triplets, not RGB
                    pImg[indx    ] = byteBuf[5];    // Red MSB
                    pImg[indx + 1] = byteBuf[3];    // Grn MSB
                    pImg[indx + 2] = byteBuf[1];    // Blu MSB
                }
            }
        }
    }
    else
    {
        cout << "Unknown PPM file type! (doesn't begin with P3 or P6)"<< endl;
        inFile.close();             // bye.
        //exit(EXIT_FAILURE);
        return 1;
    }
    inFile.close();
    return 0;
}

int CByteImg::readBMPfile(void)
//------------------------------------------------------------------------------
// read file using format rules for .BMP extension; called by readFile() only!
{
    cout << "File extension .bmp: Microsoft bitmap image file:" << endl;
    inFile.open(fullBuf.c_str(), ifstream::in | ifstream::binary);
    // args are:  open(filename,mode(s)).  For more, see:
    //          http://www.cplusplus.com/doc/tutorial/files/
    if(inFile.is_open())
    {
        cout << "\n----SUCCESS: opened file---" << endl;
        cout << "\t" << nameOfFile << endl;
        cout << "---------for reading.--------" << endl;
    }
    else
    {
        cout << "\n\n!!------Could not open this file: --------\n";
        cout << "\n\t" << fullBuf << endl;
        cout << "\n--------------------------------------------\n Bye." << endl;
        return(1);              // error exit.
        //exit(EXIT_FAILURE);
    }
    //.....................BMP file format.......................
    inFile.read((char *)byteBuf,2); // read in the 1st 2 bytes.
    byteBuf[2] = '\0';              // append a 'NULL' to make 2-char string
    cout << "1st 2 bytes in file: " << byteBuf << endl;
    cout << "ALWAYS 'BM' in the BMP file format. True?" << endl;
    if(byteBuf[0]=='B' && byteBuf[1]=='M') // (stored !BIG-ENDIAN!)
    {
        cout << "YES! this may be a 24-bit or 32-bit BMP image file...";
        cout << endl;
        isBGRfile = true;           // Microsoft BMP format ALWAYS uses BGR
    }
    else
    {
        cout << "NO! this cannot be a BMP image file. Bye!" << endl;
        inFile.close();         // close the file,
        return(1);              // error exit.
        //exit(EXIT_FAILURE);
    }
/*
    cout << "Next, read in two elaborate 'little-endian' structs:" << endl;
    cout << "\t(Recall: multi-byte words put LSByte first, MSByte last,"<< endl;
    cout << "\tIntel x86 -> little-endian; PowerPC, SPARC -> big-endian)" << endl;
    cout << "\tWe'll read BYTES from the file and CONSTRUCT ints from it." << endl;
*/
    //...........Read in fileHdr...........................................
    // SEE: http://cppcoder.blogspot.com/2007/11/bmp-file-format.html
                                    // make an 'int' from the 2-byte
                                    // identifier we read: 'BM' --> 19778
    fileHdr.bfType = lilEndUnpack16(byteBuf);
    inFile.read((char *)byteBuf,4); // next 4 bytes: BMP file size in bytes,
    fileHdr.bfSize = lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // next 4 bytes: reserved for app that
                                    // created the file, but always '0'
    fileHdr.bfReserved1 = lilEndUnpack16(byteBuf);
    fileHdr.bfReserved2 = lilEndUnpack16(byteBuf+2);
    inFile.read((char *)byteBuf,4); // last 4 bytes: offset in bytes from
                                    // start of file to start of img. data.
    fileHdr.bfOffBits = lilEndUnpack32(byteBuf);
    //.........Read in infoHdr..............................................
    inFile.read((char *)byteBuf,4); // Next 4 bytes: infoHdr size in bytes
    infoHdr.biSize = lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4 bytes: image width in pixels
    infoHdr.biWidth = (int32_t)lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4);
    infoHdr.biHeight =(int32_t)lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,2); // Next 2: # of color planes (ALWAYS 1)
    infoHdr.biPlanes = lilEndUnpack16(byteBuf);
    inFile.read((char *)byteBuf,2); // Next 2: bits per pixel (ALWAYS 24)
    infoHdr.biBitCount = lilEndUnpack16(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4: Compression type (ALWAYS 0).
    infoHdr.biCompression = lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4: Image size in bytes
                                    // (INCLUDES the end-of-scanline padding!)
    infoHdr.biSizeImage = lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4: Display resolution-x (DPI)
    infoHdr.biXPelsPerMeter = (int32_t)lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4: Display resolution-y (DPI)
    infoHdr.biYPelsPerMeter = (int32_t)lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4: # palette colrs (ALWAYS 0)
    infoHdr.biClrUsed = lilEndUnpack32(byteBuf);
    inFile.read((char *)byteBuf,4); // Next 4: min # of colrs (ALWAYS 0)
    infoHdr.biClrImportant = lilEndUnpack32(byteBuf);
    // Error checks:
    if(infoHdr.biPlanes != 1)
    {
        cout << "ERROR: infoHdr.biPlanes = " << infoHdr.biPlanes << endl;
        cout << "   I only read files where biPlanes==1.  Bye." << endl;
        //exit(EXIT_FAILURE);
        inFile.close();
        return(1);                  // error exit.
    }
    if(infoHdr.biBitCount == 24)
    {
        cout << "24 bits/pixel: RGB" << endl;
        pixSiz = 3;     // 3 bytes/pixel
    }
    else if(infoHdr.biBitCount == 32)
    {
        cout << "32 bits/pixel: RGBA" << endl;
        pixSiz = 4;     // 4 bytes/pixel
    }
    else
    {
        cout << "ERROR: infoHdr.biBitCount = " << infoHdr.biBitCount << endl;
        cout << "   I accept only 24 bit (RGB) or 32 bit (RGBA) files.  Bye.";
        cout << endl;
        inFile.close();
        //exit(EXIT_FAILURE);
        return(1);                  // error exit.
    }
    if(infoHdr.biCompression != 0)
    {
        cout << "ERROR: infoHdr.biCompression = " << infoHdr.biCompression;
        cout << endl;
        cout << "   I only read biCompression = 0 files. Bye." << endl;
        inFile.close();
        //exit(EXIT_FAILURE);
        return(1);                  // error exit.
    }
    if(infoHdr.biClrUsed !=0)
    {
        cout << "ERROR: infoHdr.biClrUsed = " << infoHdr.biClrUsed << endl;
        cout << "   I only read biClrUsed = 0 files (no LUTs). Bye." << endl;
        inFile.close();
        //exit(EXIT_FAILURE);
        return(1);                  // error exit.
    }
    if(infoHdr.biClrUsed !=0)
    {
        cout << "ERROR: infoHdr.biClrImportant = " << infoHdr.biClrImportant;
        cout << endl;
        cout << "   I only read biClrImportant = 0 files (no LUTs). Bye.";
        cout << endl;
        inFile.close();
        //exit(EXIT_FAILURE);
        return(1);                  // error exit.
    }
    //----------------Get ready to READ the actual image.
    // Our fstream file-read ptr SHOULD already be at the first byte of the
    // image, but BMP format doesn't guarantee it. Instead, the fileHdr struct
    // member 'bfOffBits' gives us the number of bytes from the start of the
    // file to the first byte of the image. FIRST, is file ptr already there?
    if( inFile.tellg() == fileHdr.bfOffBits)
    {
        cout << "inFile.tellg() matches fileHdr.bfOffbits. Good!" << endl;
    }
    else
    {
        cout << "A little weird: inFile.tellg() = " << inFile.tellg() << endl;
        cout << "doesn't match fileHdr.bfOffbits= " << fileHdr.bfOffBits <<endl;
        cout << "Re-position inFile's file ptr to match bfOffbits." << endl;
        cout << "(we shouldn't have to do this! -- Perhaps the";
        cout << "file holds ICC profiles, color tables, etc.?" << endl;
        // from ios_base::beg (beginning of file, offset the file read
        // pointer in inFile forward by bfOffBits bytes.
        inFile.seekg(fileHdr.bfOffBits, ios_base::beg);
    }
    // Make our CByteImg buffer large enough to hold the image + padding:
    if(infoHdr.biHeight < 0)
    {   // BMP file format: biHeight <0 if the file stores the image scanlines
        // in top-down order (image origin at upper left); >0: bottom-up order.
        isTopDownFile = true;                       // remember it.
        tallTmp = -infoHdr.biHeight;       // (find height as >0)
        cout << "isTopDownFile==true" << endl;
    }
    else
    {
        tallTmp =  infoHdr.biHeight;        // nope. Origin at lower left.
        isTopDownFile = false;
        cout << "isTopDownFile==false" << endl;
    }
    // ALLOCATE the memory we need to store our image..........................
    if(infoHdr.biBitCount == 24)
    {
        sizer(infoHdr.biWidth, tallTmp, 3);     // for RGB
    }
    else if(infoHdr.biBitCount == 32)
    {
        sizer(infoHdr.biWidth, tallTmp, 4);     // for RGBA
    }
    // Read pixel values from file into memory:................................
int i,j;        // pixel, scanline counter
int32_t indx;   // memory buffer address.
GLbyte btmp;     // for BGR->RGB swap.

    if(isTopDownFile==false)    // File's mage origin at lower left corner.
    {
        for(j=0; j<ySiz; j++)      // for each scanline,
        {
            indx = pixIndex(0,j);  // find array index of pixel 0, scanline j
            inFile.read( (char *)(pImg + indx), lineSiz);   // read scanline
        }                          // using 'lineSiz' set by sizer() call
        inFile.close();                             // and we're done!
    }
    else
    {   // file image origin at TOP left corner; read last to first scanline,
        for(j = ySiz-1; j>=0; j--)
        {
            indx = pixIndex(0,j);    // get array index of pix 0 on j-th scanline
            inFile.read( (char *)(pImg + indx), lineSiz); // read scanline.
        }                          // using 'lineSiz' set by sizer() call
        inFile.close();                             // and we're done!
    }
    // But we're STILL not done!  Microsoft BMP format files store 'RGB' pixels
    // with R and B bytes swapped (BGR format, not RGB)(BGRA format, not RGBA)
    for(j=0; j<ySiz; j++)               // for every scanline,
    {
        for(i=0; i<xSiz; i++)           // for every pixel on that scanline
        {
            indx = pixIndex(i,j);
            btmp = pImg[indx];      // save  blue value: pixel value is BGR
            pImg[indx] = pImg[indx+2];// copy red value: pixel value is RGR
            pImg[indx+2] = btmp;        // restore blue: pixel value is RGB
        }
    }
    return 0;
}

void CByteImg::printBMPreport(void)
//------------------------------------------------------------------------------
// print all the current BMP header information
{
    // Print contents we read in:
    cout << "bitmap-INFO-header contents-------------------------------" << endl;
    cout << "biSize: INFO-header size in bytes (usually 40):\t";
    cout << infoHdr.biSize << endl;
    cout << "biWidth,biHeight: image width,height in pixels:\t";
    cout << infoHdr.biWidth << ", " << infoHdr.biHeight << endl;
    cout << "biPlanes:   # color planes (req'd value-- 1):  \t";
    cout << infoHdr.biPlanes << endl;
    cout << "biBitCount: # bits/plane  (req'd value-- 24):  \t";
    cout << infoHdr.biBitCount << endl;
    cout << "biCompression: (req'd value--0: no compression)\t";
    cout << infoHdr.biCompression << endl;
    cout << "biSizeImage: bytes used to store image:        \t";
    cout << infoHdr.biSizeImage << endl;
    cout << "biXPelsPerMeter, biYPelsPerMeter:on-screen size:" << endl;
    cout << "   (Default 96 dpi spacing = 3780, 3780:)\t";
    cout << infoHdr.biXPelsPerMeter << ", " << infoHdr.biYPelsPerMeter << endl;
    cout << "biClrUsed: # of color palette entries(0 req'd):\t";
    cout << infoHdr.biClrUsed << endl;
    cout << "biClrImportant: vital palette entries(0 req'd):\t";
    cout << infoHdr.biClrImportant << endl;
    cout << "-----end bitmap-INFO-Header-------------------------------" << endl;
    // PRINT fileHdr contents:...............................................
    cout << "bitmap-FILE-header contents-------------------------------" << endl;
    cout << "bfType: 2-char ID string (req'd value: 19778):\t";
    cout << fileHdr.bfType << endl;
    cout << "bfSize: size of BMP file in bytes:            \t";
    cout << fileHdr.bfSize << endl;
    cout << "bfReserved1, bfReserved2: (req'd value: 0,0): \t";
    cout << fileHdr.bfReserved1 << ", " << fileHdr.bfReserved2 << endl;
    cout << "bfOffBits: byte address of img start in file: \t";
    cout << fileHdr.bfOffBits << endl;
    cout << "-----end bitmap-File-Header-------------------------------" << endl;
}

void CByteImg::printImgContents(int xQuit, int yQuit)
//------------------------------------------------------------------------------
// print all pixel values: prints only the 1st 'xQuit' columns, 'yQuit' rows,
// or prints ALL columns,rows if xQuit,yQuit == 0.
{
int i, imax, j, jmax, k, lmax;
int32_t indx;

    imax = xQuit;               // set the number of columns, rows to print.
    jmax = yQuit;
    if(imax==0) imax = xSiz;    // if zero, print all columns
    if(jmax==0) jmax = ySiz;    // if zero, print all rows
    if(imax*jmax*pixSiz == 0)
    {
        cout << "CByteImg::printImgContents() given zero-sized image. Bye!";
        cout << endl;
        return;
    }
    cout.setf(ios_base::right, ios_base::basefield );
    cout.width(5);
    cout << "======================================================" << endl;
    cout << "\t\t" << nameOfFile << endl;
    cout << "======================================================" << endl;

    cout.width(3);                      // width of printed numbers
    for(j=0; j<jmax; j++)
    {
        cout << "row(";
        cout.width(3);                      // width of printed numbers
        cout << j << "): ";
        lmax = 10;                      // linewrap: used 10 chars so far;
        for(i=0; i<imax; i++)
        {
            for(k=0; k<pixSiz; k++)
            {
                indx = pixIndex(i,j);
            // have to type-cast TWICE! First to unsigned char so we won't get
            // sign extension, THEN to int, so cout will print it as a number
            // and not as an ASCII character code
                cout.width(3);                      // width of printed numbers
                cout << (int)((unsigned char)pImg[indx +k]);
                lmax += 4;        // count chars used so far
                if(k < pixSiz-1) cout << ","; // comma-separated R,G,B
                else
                {   // end of pixel.
                    cout << "| ";
                    if(lmax + pixSiz*4 >=80)   // next pixel beyond 80 chars?
                    {
                        cout << endl << "          ";   // new line; indent.
                        lmax = 10;
                    }
                }
            }
        }
        cout << endl;               // at least one printed row per scanline
    }
    cout << "======================================================" << endl;
}

int CByteImg::writeTXTfile(void)
//------------------------------------------------------------------------------
// write file using format rules for .TXT: called by writeFile() only
{
    return 0;
}

int CByteImg::writePPMfile(void)
//------------------------------------------------------------------------------
// write file using format rules for .PPM; called by writeFile() only
{
    return 0;
}

int CByteImg::writeBMPfile(void)
//------------------------------------------------------------------------------
// write file using format rules for .BMP; called by writeFile() only.
{
    return 0;
}

uint32_t CByteImg::lilEndUnpack32(const uint8_t *pSrc)
//------------------------------------------------------------------------------
// Return a 32-bit unsigned integer built from adjacent bytes of 'little-endian'
// data taken from the pSrc buffer. To unpack signed values, use this function,
// type-cast the returned result and assign it to a variable of signed type.
{
uint32_t soln;
int i;

//cout << "lilEndUnpack32: pSrc[0,1,2,3]: ";
//cout << (int)pSrc[0] << ", " << (int)pSrc[1] << ", ";
//cout << (int)pSrc[2] << ", " << (int)pSrc[3] << endl;

    soln  = (uint32_t)pSrc[3];          // get the most-significant 8 bits,
//cout << "then: soln= " << soln << ", " ;
    for(i=2; i>=0; i--)                  // for the rest,
    {
        soln  = soln << 8;              // arithmetic shift upwards by 8,
//cout << soln << ", ";
        soln |= (uint32_t)pSrc[i];      // OR with next-most-signif 8 bits,
//cout << soln << ", ";
    }
//cout << "done." << endl;
    return soln;
}

uint16_t CByteImg::lilEndUnpack16(const uint8_t *pSrc)
//------------------------------------------------------------------------------
// Return a 16-bit unsigned integer built from adjacent bytes of 'little-endian'
// data taken from the pSrc buffer. To unpack signed values, use this function,
// type-cast the returned result and assign to a variable of signed type.
{
uint16_t soln;
//cout << "lilEndUnpack16: pSrc[0,1] = " << pSrc[0] << ", " << pSrc[1] << endl;

    soln = (uint16_t)pSrc[1];           // get the most-significant 8 bits,
//cout << "then: soln= " << soln;
    soln = soln << 8;                   // shift upwards by 8 bits (zero fill)
//cout << ", " << soln;
    soln |= (uint16_t)pSrc[0];          // OR with least-significant bits.
//cout << ", " << soln << endl;
    return soln;
}

void CByteImg::lilEndPack32(uint8_t *pDest, const uint32_t src32)
//-----------------------------------------------------------------------------
// Fill byte-buffer pDest with bytes of the src32 integer, (re)arranged into
// little-endian order.
{
uint32_t tmp;
int i;

    tmp = src32;                        // make local copy.
    for(i=0; i<4; i++)
    {
        pDest[i] = (uint8_t)(tmp & 0x000000ff); // least signif bytes first.
        tmp = tmp >> 8;                         // get next 8: shift downwards.
    }
}

void CByteImg::lilEndPack16(uint8_t *pDest, const uint16_t src16)
//------------------------------------------------------------------------------
// Fill byte-buffer pDest with bytes of the src16 integer, (re)arranged into
// little-endian order.
{
uint16_t tmp;
int i;

    tmp = src16;                        // make local copy.
    for(i=0; i<2; i++)
    {
        pDest[i] = (uint8_t)(tmp & 0x000000ff); // least signif bytes first.
        tmp = tmp >> 8;                         // get next 8: shift downwards.
    }
}

void CByteImg::reportGLerr(const char *pIdent)
//------------------------------------------------------------------------------
// (COPIED from CTheApp class)
// Check for error from your most-recent openGL call (begins with gl...).
// No error? does nothing. Error? prints the 'src' string (put your function's
// name here), the openGL enumerated value, and a brief explanation (see pg. 47,
// in the book 'Beginning OpenGL Game Programming', 2nd Ed., Luke Benstead.
// EXAMPLE:  you may wish to check for openGL errors inside your 'myDisplay()'
// callback function just after glPushMatrix() call:
//      myApp.reportGLerr("myDisplay--after glPushMatrix:")
{
GLenum errCode;

    errCode = glGetError();
    switch(errCode)         // retrieves openGL's most-recently generated
    {                           // error, and resets value to 'GL_NO_ERROR')
    case GL_NO_ERROR:
        return;             // no problems; no report.
        break;
    case GL_INVALID_ENUM:
        cout << "!!!GL_INVALID_ENUM error from " << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "An openGL fcn ignored a meaningless, misspelled or invalid" << endl;
        cout << "enumerated value you used (e.g. LIGHT8 or VORTOX_ARRAY)" << endl;;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
    case GL_INVALID_VALUE:
        cout << "!!!GL_INVALID_VALUE error from " << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "An openGL fcn ignored a invalid or out-of-range numerical" << endl;
        cout << "value you fed to it (e.g. field of view set to 500 degrees)" << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
    case GL_INVALID_OPERATION:
        cout << "GL_INVALID_OPERATION error from " << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "The combination of values you passed to an openGL function" << endl;
        cout << "won't work together, or conflicts with openGL's current state" << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
    case GL_STACK_OVERFLOW:
        cout << "GL_STACK_OVERFLOW error from " << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "You added too many items onto an openGL stack. Most often" << endl;
        cout << "caused by too many glPushMatrix() calls to the GL_MODELVIEW," << endl;
        cout <<" GL_PROJECTION, or GL_TEXTURE matrix stacks." << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
    case GL_STACK_UNDERFLOW:
        cout << "GL_STACK_UNDERFLOW error from" << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "You removed an item from an already-empty openGL stack." << endl;
        cout << "Most often caused by too many glPopMatrix() calls to the" << endl;
        cout << "GL_MODELVIEW, GL_PROJECTION, or GL_TEXTURE matrix stacks." << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
    case GL_OUT_OF_MEMORY:
        cout << "GL_OUT_OF_MEMORY error from" << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "Disaster. You over-filled the memory available to openGL," << endl;
        cout << "and may have corrupted or lost part of the openGL state." << endl;
        cout << "Your only option now is to re-start your application." << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
/*  //SOME OPENGL IMPLEMENTATIONS DO NOT DEFINE THESE LAST TWO ERRORS:
    //---------------------------------------------------------------
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        cout << "GL_INVALID_FRAMEBUFFER_OPERATION error from " << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "An openGL framebuffer object wasn't completed, it lacks a" << endl;
        cout << "required setting, attachment or data member."              << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
    case GL_TABLE_TOO_LARGE:
        cout << "GL_TABLE_TOO_LARGE error from" << pIdent << " !!!" << endl;
        cout << gluErrorString(errCode) << endl;
        cout << "----------------------------------------------------------" << endl;
        cout << "Uncommon error: an imaging function (usually glPixelMap()" << endl;
        cout << "tried to create a lookup table larger than available memory" << endl;
        cout << "in openGL.  As this might corrupt openGL state, you should" << endl;
        cout << "re-start your application to prevent spurious results." << endl;
        cout << "----------------------------------------------------------";
        cout << endl << endl;
        break;
*/
    default:
        break;
    }
}
