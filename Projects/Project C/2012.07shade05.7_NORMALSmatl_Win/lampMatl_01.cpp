//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
//
// Classes for openGL light sources (CLights) and materials (CMatl) useful for
// its built-in Phong lighting and materials.
//
// 2010.07.05 -- J. Tumblin created,tested in 'shadeTest05.4_matl' starter code.

#include "lampMatl_01.h"

//==============================================================================
//
//  CLight function bodies:
//
//==============================================================================

CLight::~CLight(void)
//------------------------------------------------------------------------------
// default destructor
{
}

CLight::CLight(void)
//------------------------------------------------------------------------------
// default constructor
{
    // default: dim white light overhead, infinitely far away
    I_ambi.put(0.2, 0.2, 0.2, 0.0); // red, grn, blu, alpha
    I_diff.put(0.5, 0.5, 0.5, 0.0);
    I_spec.put(0.5, 0.5, 0.5, 0.0);
    I_pos.put( 0.0, 1.0, 0.0, 0.0);  //x,y,z,w in openGL's current coord system.
    // w?  CAUTION! w=0 means 'vector'; if zero, openGL assumes light position
    // is infinitely far away in the direction given by x,y,z vector.
    // If w=1.0 (or any other nonzero value)then we're describing a local light
    // source at location (x/w, y/w, z/w) in the coordinate system where
    // we called CLight::applyLight() for this object.
}

void CLight::createLamp(int myLampType, GLenum myLampID)
//------------------------------------------------------------------------------
// Use lampType to select one of our pre-defined  kinds of openGL lights;
// sets all parameters and sets this object's lampID to myLampID.
// NOTE! does not enable the light! you must call CLight::applyLamp() for that!
{
    lampID = myLampID;      // record which openGL light-source we'll use
                            // NOTE: set myLampID to
    switch(myLampType)
    {
    case LAMP_POINT:        // point-source lamp at specified location.
        // default: dim white light overhead, infinitely far away
        I_ambi.put(0.0, 0.2, 0.2, 0.0); // red, grn, blu, alpha
        I_diff.put(0.9, 0.9, 0.9, 0.0);
        I_spec.put(1.0, 1.0, 1.0, 0.0);
        I_pos.put( 0.0, 0.0, 0.0, 1.0);
        //x,y,z,w in openGL's current coord. system.
        // CAUTION! w=0 means 'vector'; if zero, openGL assumes light position
        // is infinitely far away in the direction given by x,y,z vector.  If
        // w=1.0 (or any other nonzero value)then we're describing a local light
        // source at location (x/w, y/w, z/w) in the coordinate system where
        // we called CLight::applyLight() for this object.
        break;
    case LAMP_POINT_L_FILL:// point-source lamp infinitely far above and to left
        I_ambi.put(0.2, 0.2, 0.2, 0.0); // red, grn, blu, alpha
        I_diff.put(0.5, 0.5, 0.5, 0.0);
        I_spec.put(1.0, 1.0, 1.0, 0.0);
        I_pos.put(-1.0, 1.0, 0.0, 0.0);
        break;
    case LAMP_POINT_R_FILL:// point-source lamp infinitely far above& to right
        I_ambi.put(0.2, 0.2, 0.2, 0.0); // red, grn, blu, alpha
        I_diff.put(0.4, 0.4, 0.4, 0.0);
        I_spec.put(1.0, 1.0, 1.0, 0.0);
        I_pos.put( 1.0, 1.0, 0.0, 0.0);
        break;
/*    case LAMP_SPOT:
    // not done yet
    case LAMP_TEXTURE:
    // not done yet
    case LAMP_ENVIR_MAP:
    // not done yet
*/    case LAMP_POINT_KEY:// point-source lamp infinitely far overhead
    default:            // (also the default light source)
        I_ambi.put(0.2, 0.2, 0.2, 0.0); // red, grn, blu, alpha
        I_diff.put(0.9, 0.9, 0.9, 0.0);
        I_spec.put(1.0, 1.0, 1.0, 0.0);
        I_pos.put( 0.0, 1.0, 0.0, 0.0);
        //x,y,z,w in openGL's current coord. system.
        // CAUTION! w=0 means 'vector'; if zero, openGL assumes light position
        // is infinitely far away in the direction given by x,y,z vector.  If
        // w=1.0 (or any other nonzero value)then we're describing a local light
        // source at location (x/w, y/w, z/w) in the coordinate system where
        // we called CLight::applyLight() for this object.
        break;
    }
}

void CLight::applyLamp(void)
//------------------------------------------------------------------------------
// Apply contents of this CLight object to the openGL light source specified
//  by CLight::lampID; (whose value is GL_LIGHT0 or GL_LIGHT1 or ... GL_LIGHT7).
{
    // Set openGL's light-source values
    // (AARGH! openGL doesn't have a 'glLightdv()' fcn that accepts a vector
    // of doubles; we can only feed it floats. (or individual doubles))
    GLfloat tmpV[4];                        // holds Vec4 object's contents
                                            // converted from doubles to floats

    I_ambi.toFloat(tmpV);                   // convert to floats, then use it
	glLightfv(lampID, GL_AMBIENT, tmpV);   //  to set openGL ambient values
    I_diff.toFloat(tmpV);
    glLightfv(lampID, GL_DIFFUSE, tmpV);
    I_spec.toFloat(tmpV);
    glLightfv(lampID, GL_SPECULAR, tmpV);
    I_pos.toFloat(tmpV);
    glLightfv(lampID, GL_POSITION, tmpV);

    glEnable(GL_LIGHTING);		// enable OpenGL lighting itself, and
    glEnable(lampID);		    // enable the light source we adjusted.
    							// (lights stay off until we enable them)
}

void CLight::removeLamp(void)
//------------------------------------------------------------------------------
// Disable this lamp, but don't change it in any other way.  To re-enable, call
// CLight::applyLamp() function.
{
    glDisable(lampID);          // turn off our particular openGL lamp.
}


//==============================================================================
//
//  CMatl function bodies:
//
//==============================================================================
/* Material type values for CMatl::matlType; selects among a set of pre-defined
// Phong Lighting materials (value from FS Hill, page 423, Fig 8.17)
#define MATL_CUSTOM         0
#define MATL_RED_PLASTIC    1
#define MATL_GRN_PLASTIC    2
#define MATL_BLU_PLASTIC    3
#define MATL_BLACK_PLASTIC  4
#define MATL_BRASS          5
#define MATL_BRONZE         6
#define MATL_CHROME         7
#define MATL_COPPER         8
#define MATL_GOLD           9
#define MATL_PEWTER        10
#define MATL_SILVER_DULL   11
#define MATL_SILVER_SHINY  12
*/

CMatl::~CMatl(void)
//------------------------------------------------------------------------------
// default destructor
{
}

CMatl::CMatl(void)
//------------------------------------------------------------------------------
// default constructor
{
    K_emit.put(1.0, 0.0, 0.0, 1.0); // DEFAULT: ugly RED emissive light only
    K_ambi.put(0.0, 0.0, 0.0, 1.0); // r,g,b,alpha  ambient reflectance
    K_diff.put(0.0, 0.0, 0.0, 1.0); //              diffuse reflectance
    K_spec.put(0.0, 0.0, 0.0, 1.0); //              specular reflectance
    K_shiny = 1.0f;            // Default (don't set specular exponent to zero!)
    isFaceted = FALSE;      // Default: smooth shaded; uses glShade(GL_SMOOTH);
                            // (if TRUE, uses glShade(GL_FLAT) for faceted surf.
    isTwoSided = FALSE;     // Default: renders only front faces; ignores backs
                            // by using glMaterial(GL_FRONT, ...) calls.If your
                            // object vanishes when isTwoSided==FALSE, your
                            // surface normals are probably reversed; see
                            // glFrontFace() function.
    strncpy(matlName,"Not Initialized!", 80);  // set text label
}

void CMatl::createMatl(int nuMatl)
//------------------------------------------------------------------------------
// Use matlType to select one of our pre-defined kinds of openGL material.
{
    matlType = nuMatl;
    switch(nuMatl)
    {
    case MATL_CUSTOM:
        strncpy(matlName, "MATL_CUSTOM", MATL_STRMAX);       // set text label
        break;
    case MATL_RED_PLASTIC:
        K_emit.put(0.0,     0.0,    0.0,    1.0);
        K_ambi.put(0.1,     0.1,    0.1,    1.0);
        K_diff.put(0.6,     0.0,    0.0,    1.0);
        K_spec.put(0.6,     0.6,    0.6,    1.0);   K_shiny = 100.0f;
        strncpy(matlName, "MATL_RED_PLASTIC", MATL_STRMAX);  // set text label
        break;
    case MATL_GRN_PLASTIC:
        K_emit.put(0.0,     0.0,    0.0,    1.0);
        K_ambi.put(0.05,    0.05,   0.05,   1.0);
        K_diff.put(0.0,     0.6,    0.0,    1.0);
        K_spec.put(0.2,     0.2,    0.2,    1.0);   K_shiny = 60.0f;
        strncpy(matlName, "MATL_GRN_PLASTIC", MATL_STRMAX);  // set text label
        break;
    case MATL_BLU_PLASTIC:
        K_emit.put(0.0,     0.0,    0.0,    1.0);
        K_ambi.put(0.05,    0.05,   0.05,   1.0);
        K_diff.put(0.0,     0.2,    0.6,    1.0);
        K_spec.put(0.1,     0.2,    0.3,    1.0);   K_shiny = 5.0f;
        strncpy(matlName, "MATL_BLU_PLASTIC", MATL_STRMAX);  // set text label
        break;
    case MATL_BLACK_PLASTIC:
        K_emit.put(0.0,     0.0,    0.0,    1.0);
        K_ambi.put(0.0,     0.0,    0.0,    1.0);
        K_diff.put(0.01,    0.01,   0.01,   1.0);
        K_spec.put(0.5,     0.5,    0.5,    1.0);   K_shiny = 32.0f;
        strncpy(matlName, "MATL_BLACK_PLASTIC", MATL_STRMAX);  // set text label
        break;
    case MATL_BLACK_RUBBER:
        K_emit.put(0.0,     0.0,    0.0,    1.0);
        K_ambi.put(0.02,    0.02,   0.02,   1.0);
        K_diff.put(0.01,    0.01,   0.01,   1.0);
        K_spec.put(0.4,     0.4,    0.4,    1.0);   K_shiny = 10.0f;
        strncpy(matlName, "MATL_BLACK_RUBBER", MATL_STRMAX);  // set text label
        break;
    case MATL_BRASS:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.329412, 0.223529, 0.027451, 1.0);
        K_diff.put(0.780392, 0.568627, 0.113725, 1.0);
        K_spec.put(0.992157, 0.941176, 0.807843, 1.0);   K_shiny = 27.8974f;
        strncpy(matlName, "MATL_BRASS", MATL_STRMAX);  // set text label
        break;
    case MATL_BRONZE_DULL:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.2125,   0.1275,   0.054,    1.0);
        K_diff.put(0.714,    0.4284,   0.18144,  1.0);
        K_spec.put(0.393548, 0.271906, 0.166721, 1.0);  K_shiny = 25.6f;
        strncpy(matlName, "MATL_BRONZE_DULL", MATL_STRMAX);  // set text label
        break;
    case MATL_BRONZE_SHINY:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.25,     0.148,    0.06475,  1.0);
        K_diff.put(0.4,      0.2368,   0.1036,   1.0);
        K_spec.put(0.774597, 0.458561, 0.200621, 1.0);  K_shiny = 76.8f;
        strncpy(matlName, "MATL_BRONZE_SHINY", MATL_STRMAX);  // set text label
        break;
    case MATL_CHROME:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.25,     0.25,     0.25,     1.0);
        K_diff.put(0.4,      0.4,      0.4,      1.0);
        K_spec.put(0.774597, 0.774597, 0.774597, 1.0);  K_shiny = 76.8f;
        strncpy(matlName, "MATL_CHROME", MATL_STRMAX);  // set text label
        break;
    case MATL_COPPER_DULL:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.19125,  0.0735,   0.0225,   1.0);
        K_diff.put(0.7038,   0.27048,  0.0828,   1.0);
        K_spec.put(0.256777, 0.137622, 0.086014, 1.0);  K_shiny = 12.8f;
        strncpy(matlName, "MATL_COPPER_DULL", MATL_STRMAX);  // set text label
        break;
    case MATL_COPPER_SHINY:
        K_emit.put(0.0,      0.0,      0.0,       1.0);
        K_ambi.put(0.2295,   0.08825,  0.0275,    1.0);
        K_diff.put(0.5508,   0.2118,   0.066,     1.0);
        K_spec.put(0.580594, 0.223257, 0.0695701, 1.0);  K_shiny = 51.2f;
        strncpy(matlName, "MATL_COPPER_SHINY", MATL_STRMAX);  // set text label
        break;
    case MATL_GOLD_DULL:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.24725,  0.1995,   0.0745,   1.0);
        K_diff.put(0.75164,  0.60648,  0.22648,  1.0);
        K_spec.put(0.628281, 0.555802, 0.366065, 1.0);  K_shiny = 51.2f;
        strncpy(matlName, "MATL_GOLD_DULL", MATL_STRMAX);  // set text label
        break;
    case MATL_GOLD_SHINY:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.24725,  0.2245,   0.0645,   1.0);
        K_diff.put(0.34615,  0.3143,   0.0903,   1.0);
        K_spec.put(0.797357, 0.723991, 0.208006, 1.0);  K_shiny = 83.2f;
        strncpy(matlName, "MATL_GOLD_SHINY", MATL_STRMAX);  // set text label
        break;
    case MATL_PEWTER:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.105882, 0.058824, 0.113725, 1.0);
        K_diff.put(0.427451, 0.470588, 0.541176, 1.0);
        K_spec.put(0.333333, 0.333333, 0.521569, 1.0);  K_shiny = 9.84615f;
        strncpy(matlName, "MATL_PEWTER", MATL_STRMAX);  // set text label
        break;
    case MATL_SILVER_DULL:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.19225,  0.19225,  0.19225,  1.0);
        K_diff.put(0.50754,  0.50754,  0.50754,  1.0);
        K_spec.put(0.508273, 0.508273, 0.508273, 1.0);  K_shiny = 51.2f;
        strncpy(matlName, "MATL_SILVER_DULL", MATL_STRMAX);  // set text label
        break;
    case MATL_SILVER_SHINY:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.23125,  0.23125,  0.23125,  1.0);
        K_diff.put(0.2775,   0.2775,   0.2775,   1.0);
        K_spec.put(0.773911, 0.773911, 0.773911, 1.0);  K_shiny = 89.6f;
        strncpy(matlName, "MATL_SILVER_SHINY", MATL_STRMAX);  // set text label
        break;
    case MATL_EMERALD:
        K_emit.put(0.0,     0.0,      0.0,     1.0);
        K_ambi.put(0.0215,  0.1745,   0.0215,  0.55);
        K_diff.put(0.07568, 0.61424,  0.07568, 0.55);
        K_spec.put(0.633,   0.727811, 0.633,   0.55);   K_shiny = 76.8f;
        strncpy(matlName, "MATL_EMERALD", MATL_STRMAX);  // set text label
        break;
    case MATL_JADE:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.135,    0.2225,   0.1575,   0.95);
        K_diff.put(0.54,     0.89,     0.63,     0.95);
        K_spec.put(0.316228, 0.316228, 0.316228, 0.95);   K_shiny = 12.8;
        strncpy(matlName, "MATL_JADE", MATL_STRMAX);  // set text label
        break;
    case MATL_OBSIDIAN:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.05375,  0.05,     0.06625,  0.82);
        K_diff.put(0.18275,  0.17,     0.22525,  0.82);
        K_spec.put(0.332741, 0.328634, 0.346435, 0.82);   K_shiny = 38.4;
        strncpy(matlName, "MATL_OBSIDIAN", MATL_STRMAX);  // set text label
        break;
    case MATL_PEARL:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.25,     0.20725,  0.20725,  0.922);
        K_diff.put(1.0,      0.829,    0.829,    0.922);
        K_spec.put(0.296648, 0.296648, 0.296648, 0.922);   K_shiny = 11.264f;
        strncpy(matlName, "MATL_PEARL", MATL_STRMAX);  // set text label
        break;
    case MATL_RUBY:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.1745,   0.01175,  0.01175,  0.55);
        K_diff.put(0.61424,  0.04136,  0.04136,  0.55);
        K_spec.put(0.727811, 0.626959, 0.626959, 0.55);   K_shiny = 76.8f;
        strncpy(matlName, "MATL_RUBY", MATL_STRMAX);  // set text label
        break;
    case MATL_TURQUOISE:
        K_emit.put(0.0,      0.0,      0.0,      1.0);
        K_ambi.put(0.1,      0.18725,  0.1745,   0.8);
        K_diff.put(0.396,    0.74151,  0.69102,  0.8);
        K_spec.put(0.297254, 0.30829,  0.306678, 0.8);   K_shiny = 12.8f;
        strncpy(matlName, "MATL_TURQUOISE", MATL_STRMAX);  // set text label
        break;

    default:
        // ugly featureless (emissive-only) red:
        K_emit.put(1.0, 0.0, 0.0, 1.0); // DEFAULT: ugly RED emissive light only
        K_ambi.put(0.0, 0.0, 0.0, 1.0); // r,g,b,alpha  ambient reflectance
        K_diff.put(0.0, 0.0, 0.0, 1.0); //              diffuse reflectance
        K_spec.put(0.0, 0.0, 0.0, 1.0); //              specular reflectance
        K_shiny = 1.0f;        // Default (don't set specular exponent to zero!)
        strncpy(matlName, "MATL_DEFAULT_RED", MATL_STRMAX);  // set text label
        cout << "CMatl::createMatl(): ERROR--invalid matlType value!\n" << endl;
        break;
    }
}

void CMatl::applyMatl(void)
//------------------------------------------------------------------------------
// Apply contents of this CMatl object to the openGL object currently
// being rendered.
{
GLenum myFace;
GLfloat tmpFV[4];
    // tell openGL how to assign on-screen colors between vertices
	if(isFaceted)
	{
	    glShadeModel(GL_FLAT);      // apply flat shading; constant-color
	}
	else
	{
	    glShadeModel(GL_SMOOTH);    // smooth shading: lerp the vertex colors
	}
    // choose which faces to shade:
    if(isTwoSided==true) myFace = GL_FRONT_AND_BACK; else myFace = GL_FRONT;
    // set all the openGL materials properties:
    // UNFORTUNATELY FOR US, openGL won't accept Vec4's 'double' values for
    // materials, so use a clunky conversion member of Vec4:
    K_emit.toFloat(tmpFV);
	glMaterialfv(myFace, GL_EMISSION, tmpFV);
	K_ambi.toFloat(tmpFV);
    glMaterialfv(myFace, GL_AMBIENT,  tmpFV);
    K_diff.toFloat(tmpFV);
	glMaterialfv(myFace, GL_DIFFUSE,  tmpFV);
	K_spec.toFloat(tmpFV);
	glMaterialfv(myFace, GL_SPECULAR, tmpFV);
	glMaterialf(myFace, GL_SHININESS, K_shiny);
}

void CMatl::showName(void)
//------------------------------------------------------------------------------
// Use openGL to print on-screen the ASCII text found in CMatl::matlLabel[]
// array, set to the material name by most-recent call to 'createMatl()'.
// Positions text at 0,0,0 in current coordinate system, and chooses text color
// to be different from material's diffuse reflectance values.
{
int i;
double colr[3];
GLboolean isLit;

    // find the color most-different from K_diff: add or subtract 0.5 to r,g,b:
    if(K_diff.row[0] <0.5) colr[0] = K_diff.row[0] + 0.5;
                      else colr[0] = K_diff.row[0] - 0.5;
    if(K_diff.row[1] <0.5) colr[1] = K_diff.row[1] + 0.5;
                      else colr[1] = K_diff.row[1] - 0.5;
    if(K_diff.row[2] <0.5) colr[2] = K_diff.row[2] + 0.5;
                      else colr[2] = K_diff.row[2] - 0.5;
/*
    colr[0] = 0.1;
    colr[1] = 0.5;
    colr[2] = 0.5;
*/
    glColor3dv(colr);   // set text color
    glGetBooleanv(GL_LIGHTING, &isLit); // is GL_LIGHTING enabled?
    // WHY? see: http://www.sjbaker.org/steve/omniv/opengl_lighting.html
    if(isLit == GL_TRUE) glDisable(GL_LIGHTING);    // if so, briefly disable:
	glRasterPos3d(0,0,0);		// set text's lower-left corner position

	for(i=0; matlName[i] != '\0' && i<MATL_STRMAX; i++)	// for each char,
	{   // display each character as a bitmap.
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, matlName[i]);
        // All the other valid openGL font identifers you can use here instead;
        //  GLUT_BITMAP_TIMES_ROMAN_10
        //  GLUT_BITMAP_TIMES_ROMAN_24
        //  GLUT_BITMAP_HELVETICA_10
        //  GLUT_BITMAP_HELVETICA_12
        //  GLUT_BITMAP_HELVETICA_18
	}
	if(isLit == GL_TRUE) glEnable(GL_LIGHTING);     // re-enable if we disabled.
}
