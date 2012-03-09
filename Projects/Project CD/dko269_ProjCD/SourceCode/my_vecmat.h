//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
//
/*	Home-brewed vector/matrix classes; helpful for some graphics tasks.
	Uses the C preprocessor  to allow redefinition of vec,matrix element
	type (C++ templates are a more powerful way, but more confusing) by
	changing the #define VECTYPE and a define stmt. for the vector size VV;
	however all the member names include a '3' or '4' that  you should
	change manually if you change 'VV'.

Most functions are pass-by-reference for input parameters, and all use local
temporary storage of intermediate results so that in-place calculations (where
an object invokes a member function using itself as one or more of the
arguments to the function)never causes problems.  Each member function places
calculated results in the data members of the calling object, unless the result
is of a different type; then it is a returned value.

For example, the Mat4 matrix class has a member function named mmpy() that
multiplies a 'left' matrix and a 'rt' matrix.  Suppose we have three Mat4
objects named mmA,mmB, and mmC.  The statement

	 	mmC.mm_mpy(mmA,mmB);

will multiply mmA by mmB to find new values for the mmC matrix. Note that we
always use internal storage, so it is acceptable to make calls like
mmB.mm_mpy(mmB,mmB) for in-place calculations; the temporary storage used in
member functions make sure that the new mmB values are equal to the matrix
product of the old mmB values.

The arbitrary-size matrix class 'matrx' and the integer-valued matrix class
Imat are written for compatibility with code from the book "Numerical Recipes".
Note that they each have  two versions (a '0' and a'1' version) of most member
functions, such as put0(), put1() or get0(),get1().  The zero versions use
zero-indexed addressing (C-style), the '1' versions use 1-indexed address
(FORTRAN style). Note that ALL functions have the 0 or 1 suffix--this helps
prevent accidental use of the wrong function (VERY nasty bug to find).

REVISIONS:
5/14/93		J. Tumblin	CREATED from my3vec.c
9?/??/98	J. Tumblin added separate integer matrix/vector classes Ivec,Imat
12/12/98	J. Tumblin added arbitrary matrix class (from old mtx,matrx)
01/23/01	J. Tumblin more for developing aCShape library work (Comp. Geometry)
01/22/05	J. Tumblin took useful subset to make 'my_vecmat' for students.
07/20/10    J. Tumblin minor updates for CodeBlocks.
==============================================================================*/

#if !defined(__MY_VECMAT)// if we haven't already included this file,
#define __MY_VECMAT		// compile it! (#endif at end-of-file).

#define DIAGNOSTICS TRUE			// helpful ordinary checking.
#define PESKYWARNINGS FALSE			// tediously tight checking.

#define DPF printf					// for non-MS Windows debugging,
//#define DPF TRACE					// for Windows VC++ debugging.

#include <math.h>					// for transcendentals: sin,cos,arctan, etc
#include <stdio.h>					// for printf, scanf, cout, cin, etc.
#include <stdarg.h>					// for  variable-length arguments
#include <stdlib.h>					// for rand(), etc.
#include <time.h>					// for current time (used to seed rand())
#include <string.h>					// for prnt() and textOut() routines.
#include <assert.h>					// for 'assert()', of course.

#define VECTYPE double			// 'float' or 'double'-- sets element data type.
#define IS_FLOAT FALSE
#define IS_DOUBLE TRUE			// define matrix/vector data
    							// type---(double or float)
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif								// VC++ doesn't have M_PI constant !?!?
									// Handy Hint: find it in MS'Calculator'

#ifndef BOOL						// incase your OS doesn't define 'BOOL'
#define BOOL  unsigned char
#endif

#ifndef TRUE						// or 'TRUE'
#define TRUE  1
#endif

#ifndef FALSE						// or 'FALSE'
#define FALSE 0
#endif

#ifndef	ASSERT						// or uses some non-standard 'assert'
#define ASSERT assert
#endif

#define JT_EPSILON 10E-15				// vanishingly close to zero.

//==================
// FWD declarations;
//==================
class Ivec;						// Do fwd-declare of Ivec: Matrx uses it.
class Imat;						// Do fwd-declare of Imat: Matrx uses it.
class Mat4;						// Do fwd-declare of Mat4: Vec4 uses it.
class Matrx;					// DO fwd-...



class Vec4 {
//==============================================================================
/* an 3-D homogeneous 'column vector';
					[row[0] ]
					[row[1] ]
					[row[2] ]
					[row[3] ]										  */

							//-----------------DATA-------------------
    public:
	VECTYPE row[4];
							//-----------------FCNS-------------------
    public:
	Vec4(void) { zero(); };				// void constructor
	//--------------INLINE FUNCTIONS
	Vec4(VECTYPE r0,VECTYPE r1,VECTYPE r2,VECTYPE r3) {
										// explicit constructor
		    row[0] = r0; row[1] = r1; row[2] = r2; row[3] = r3; };
	void put(VECTYPE r0,VECTYPE r1,VECTYPE r2,VECTYPE r3) {// set vec elements
		    row[0] = r0; row[1] = r1; row[2] = r2; row[3] = r3; };
	void zero(void) {					// clear a vector to 0.0
	    row[0] = (VECTYPE)0.0; row[1] = (VECTYPE)0.0;
		row[2] = (VECTYPE)0.0; row[3] = (VECTYPE)0.0; };
	void copy(Vec4& src);				// copy all values from src.
	void toFloat(float dest[]) {        // convert [this] contents to floats
        dest[0] = (float)row[0]; dest[1] = (float)row[1]; // & put them into the
        dest[2] = (float)row[2]; dest[3] = (float)row[3]; }; // dest[] array.
    //--------------FULL-SIZED FUNCTIONS
	void prnt(void);					// prettyprint a vec
	void prnt(char* name);				// print a labeled vec
	VECTYPE norm3(Vec4& src); 			// normalize src, then return length
	                                    // [this] = norm(src(x,y,z)) ignore w.
	VECTYPE norm3(void);                // normalize self, then return length
	                                    // [this] = norm(this(x,y,z)) ignore w.
	VECTYPE norm4(Vec4& src); 			// normalize src, then return length
	                                    // [this] = norm(src(x,y,z)) ignore w.
	VECTYPE norm4(void);                // normalize self, then return length
	                                    // [this] = norm(this(x,y,z)) ignore w.
	void add(Vec4& src1, Vec4& src2);  	// add: [this] = src1+src2
	void addIn(Vec4& src);              // add in: [this] = [this] + src
	void sub(Vec4& src1, Vec4& src2);	// subtract;[this] = src1-src2
	void scale(Vec4& src1, VECTYPE c);	// scale: [this]= src1.*c
	void scale(VECTYPE c);              // scale: [this]= [this]*c
	void lerp3(VECTYPE c, Vec4& src1, Vec4& src2); // 3-element linear blend
                                        // [this]= src1.*(c) + src2.*(1-c)
	void lerp4(VECTYPE c, Vec4& src1, Vec4& src2); // 4-element linear blend;
                                        // WARNING! BLENDS 4th element row[3]!
	VECTYPE dot3(Vec4& src);            // 3D dot product: retn (this).[src]
	VECTYPE dot4(Vec4& src);            // 4D dot product: retn (this).[src]
	VECTYPE dot3(Vec4& src1, Vec4& src2);// 3D dot product; retn [in1].[in2]
	VECTYPE dot4(Vec4& src1, Vec4& src2);// 4D dot product; retn [in1].[in2]
	void cross3(Vec4& src1, Vec4& src2);// cartesian 3D cross product:
                                        // result has row[3]=0.0
										// [this]=[src1]X[src2]
	void cross4(Vec4& src1, Vec4& src2);// homogeneous coord cross prod;
										// [this]=[src1]X[src2]
    void multVec(Vec4& src1, Vec4& src2); // Element-by-element mpy:
                                   // [this].row[x] = src1.row[x] * src2.row[x]
	void mmpy(Mat4& srcM, Vec4& srcV);	// matrix-vector mpy:
										// [this] = [in1mat][in2vec]
	BOOL isZero(void);					// TRUE iff all elements zero.
	void copyTo(Matrx &out);			// resize 'out', copy contents to it.
};

class Mat4 {
//==============================================================================
// matrix data type for 3D homogeneous matrices; NOTE colrow[][] indices
// are just like colrow[x][y] (x,y position within matrix);
// 'col-row' name re-states the index ordering: e.g. colrow[column#][row#]
//		   [ col[0][0]  colrow[1][0]  colrow[2][0] ... ]
//		   [ col[0][1]  colrow[1][1]  colrow[2][1] ... ]
//		   [ col[0][2]  colrow[1][2]  colrow[2][2] ... ]
//		   [   ...         ...			 ...	   ... ]

							//-----------------DATA-------------------
    public:
	VECTYPE colrow[4][4]; 		// gives [column#][row#], equiv to x,y
								// position in matrix
							//-----------------FCNS-------------------
    public:
	Mat4(void) { zero(); };				// void constructor
	void zero(void);					// clear matrix to 0.0
	void ident(void);					// set to identity matrix
	void prnt(void);					// prettyprint matrix
	void prnt(char *name);				// print a labeled matrix
	void mmpy(Mat4& left, Mat4& rt);	// matrix-matrix mpy:
										// [this] = [left][rt]
	void transpose(void);				// in-place transpose; remember, if
                                        // the matrix is orthonormal, its
                                        // transpose is its inverse!
	void copy(Mat4& src);               // make a copy
	void makeTrans(VECTYPE xoff, VECTYPE yoff, VECTYPE zoff);
										// make translation matx
	void makeRot(Vec4& axis, VECTYPE theta_rad);
										// make a matrix to rotate
										// about arbitrary axis.
	void makeScale(VECTYPE xscale, VECTYPE yscale, VECTYPE zscale) {
										// make 3D scale matrix
            zero(); colrow[0][0] = xscale; colrow[1][1] = yscale;
            colrow[2][2] = zscale; colrow[3][3] = (VECTYPE)1.0; };
	BOOL isZero(void);					// TRUE iff all zero elements.
	BOOL isInteger(void);				// TRUE iff all integer elements
	void copyTo(Matrx& out);			// resize 'out', copy contents to it.
};

//==============================================================================
//================================                ==============================
//================================   Arbitrary    ==============================
//================================  Sized Matrix  ==============================
//================================                ==============================
//==============================================================================


class Matrx {
//==============================================================================
// An arbitrary-sized matrix class using a storage method compatible with IMSL
// libraries (linkable FORTRAN).  PLEASE PLEASE PLEASE don't directly access
// the 'a' array; use the 'get()' and 'put()' member functions, because I may
// change the matrix storage methods later to add 'sparse' matrix types, etc.
// Written for use with the usual 0-based array indices of C,C++, BUT you can
// use the 1-based indices common to FORTRAN code and all the "Numerical Recipes
// in C" code by using 'get1()' and put1()' instead of the zero-based 'get0()'
// and 'put0()'.
//	12/14/98: added dyn.alloc'd array of pointers to each row so we are
//				also compatible with Numerical Recipes code.

//private:			    			//-----------------DATA-----------------
public:
    VECTYPE *a;			   			// dynam. allocated space at this ptr holds
									// 2D array of matrix
									// entries in row-major order--
									// A00,A10,A20,.. (all of col 0,
									// then all of col 1, etc)
									// stored value is element
    int xsize;						// # columns in the matrix.
	int ysize;						// # of rows

public:			    			//-----------------Access-----------------
    Matrx(void);					// make an 'empty' object
    ~Matrx(void);					// destructor; de-alloc memory
    BOOL wipe(void);				// release space for matrix.
    void sizer(int iysize,int ixsize);	// allocate space for matrix;
									// sizer(#rows,#columns)
	void sizer(Matrx& src);

	int getXsize(void) { return(xsize);};
	int getYsize(void) { return(ysize);};
    BOOL copy(Matrx& src);			// auto-resizing entry-by-entry copy
    								// (no change to our object's name)
	void put0(int row,int col, VECTYPE val);
	VECTYPE get0(int row, int col);	// read/write 'val' at  row,col.
									// (uses zero-based indexing)
	void put1(int row,int col, VECTYPE val);
	VECTYPE get1(int row, int col);	// read/write 'val' at  row,col.
									// (uses ONE-based indexing)
	void addTo0(int row, int col, VECTYPE val);
									// add 'val' to current value at row,col.
									// (uses zero-based indexing)
	void addTo1(int row, int col, VECTYPE val);
									// add 'val' to current value at row,col.
									// (uses ONE-based indexing)
	void put0Row(int row, VECTYPE c0,...);
	void put0Row(int row, Vec4 &src);
									// Write values at (row,c0),(row,c1),...
	void put0Col(int col, VECTYPE r0,...);
	void put0Col(int col, Vec4 &src);
									// Write values at (r0,col),(r1,col),...
	void copy0Col(int destCol, Matrx &src, int srcCol);
									// Copy one column from 'src' matrix.
	VECTYPE* getPtr0(int row=0, int col=0) { //(zero-based indexing)
									// return pointer to element;
									// default to 1st element.
		return(&(a[row + col*ysize])); };
	VECTYPE* getPtr1(int row=1, int col=1) {// (one-based indexing)
		return(&(a[(row-1) + (col-1)*ysize])); };
								//---------------MATH-----------------------
	void mpyScalar(VECTYPE wt);		// scale every element by 'wt'.
	BOOL mmpy(Matrx& left, Matrx& rt);// replace our current contents with
									// result of matrix multiply [left][rt].
									// Return FALSE on err.
	BOOL augment(Matrx& left, Matrx& rt);
									// place rows of left,rt side-by-side.
									// return FALSE on error.
	BOOL stack(Matrx& top, Matrx& bot);
									// place columns of 'top' above 'bot'.
									// return FALSE on error.
	void swapRows0(int rA,int rB);	// swap contents of rows rA,rB
									// (rA,rB using 0-based indexing)
	void swapCols0(int cA,int cB);	// swap contents of columns cA,cB
									// (cA,cB use 0-based indexing)
	void swapRows1(int rA,int rB);	// swap contents of rows rA,rB
									// (rA,rB using 1-based indexing)
	void swapCols1(int cA,int cB);	// swap contents of columns cA,cB
									// (cA,cB use 1-based indexing)
	BOOL add(Matrx& left,Matrx& rt);// 'this'=left+rt('this' as input is OK)
	BOOL sub(Matrx& left,Matrx& rt);// 'this'=left-rt('this' as input is OK)
//  BOOL add(Matrx& src);			//  add; us += src;
//  BOOL sub(Matrx& src);			//  subtract:  us -= src;
	BOOL transpose(Matrx& src);		// in-place is OK(src=*this)
	BOOL scaleRows(Matrx& scale_vec);
	BOOL GaussJordan_elim(Matrx& a, Matrx& b);
									// in-place,full-pivot, Gauss-Jordan
									// elimination; solves Ax=b given NxN matrix
									// A and NxM matrix b.  A is replaced by its
									// inverse, and b is replaced by x. Returns
									// FALSE if A is singular.
	BOOL invert(void);				// In-place matrix inversion; uses code from
									// GaussJordan_elim() with 'b' vector deleted
private:
	VECTYPE pythag(VECTYPE a, VECTYPE b);
									// computes sqrt(a^2 + b^2) without destructive
									// overflow or underflow.  Used in SVD_full,
									// taken from Numerical Recipes.
public:
	void SVD_test(int cnt=10);			// Test SVD_full() 'cnt' times.
	BOOL isOrthonormal(Matrx& in, VECTYPE errmax=0.01);	// checks columns...
	BOOL SVD_full(Matrx& u, Matrx& ss, Matrx& v);
									//-----SVD (from Numerical Recipes)---------
									// Perform full Singular Value Decomposition
									// Using current contents ('this') as 'A',
									// find matrices that satisfy  A = U S V^T
									// Where U,V are orthonormal (its columns are
									// unit length & orthogonal), S is diagonal.
									// A matrix is unchanged, but U,S,V matrices
									// contents (if any) are always replaced.
									// To save space, S is a column vector, not
									// a diagonal matrix. WARNING: S is NOT
									// in largest-to-smallest order!
									// Returns FALSE error.
	BOOL SVD_fullSort(Matrx& u, Matrx& ss, Matrx& v);
									// SAME as SVD_full except outputs U,S,V are
									// re-ordered so that S elements are in
									// greatest-to-smallest order.
	void randFill(VECTYPE min=(VECTYPE)0.0,VECTYPE max=(VECTYPE)1.0);
									// fill matrix with random #s
									// 		(default to 0.0--1.0 range)
    void zero(void);				// fill matrix with zeroes
	void ident(void);				// make identity matrix.
	void countR(void);				// fill with counting; row-major
	void countC(void);				// fill with counting; column-major
	BOOL isEqual(Matrx &src,VECTYPE tol2=1.0E-15);
									// TRUE iff 'this'=='src', (+/- tol)
	void copyTo(Mat4& out);			// Copy first 4x4 elements to 'out'
	void textOut(FILE* fptr, char* title, int linewidth);
									// make printout in stream 'fptr'
	void prnt(const char *name);	// Print to debug window using 'TRACE'.
									// usage: mymat.prnt("mymat");
};


//==============================================================================
//================================                ==============================
//================================   Integer      ==============================
//================================   Arbitrary    ==============================
//================================  Sized Vectors ==============================
//================================                ==============================
//==============================================================================


class Ivec
//==============================================================================
// Simple integer vector class for use as indices, flags, etc., inspired by
// 'ivector' functions in 'Numerical Recipes in C'.  As with the Matrx class,
// PLEASE PLEASE PLEASE don't access array 'a' directly but instead use the
// 'get' and 'put' functions.  Offers zero-based and 1-based indexing
// (as used in FORTRAN & NR code).
{
private:			    			//-----------------DATA-----------------
    int *a;			   				// dynam. allocated space at this ptr holds
									// 1D array of elements.
    int size;						// # elements.

public:			    			//-----------------FCNS-----------------
    Ivec(void);						// make an 'empty' object
    ~Ivec(void);					// destructor; de-alloc memory
    BOOL wipe(void);				// release space for vector.
    void sizer(int isize);			// allocate space for vector.
	void sizer(Ivec& src);			// match our size to 'src'.

	int getSize(void) { return(size);};
    BOOL copy(Ivec& src);			// auto-resizing entry-by-entry copy
    								// (no change to our object's name)
	void put0(int indx, int val);
	int get0(int indx);				// read/write 'val' at  row,col.
									// (uses zero-based indexing)
	void put1(int indx, int val);
	int get1(int indx);				// read/write 'val' at  row,col.
									// (uses ONE-based indexing)
								//--------------------------------------
    BOOL add(Ivec& src);			//  add; us += src;
    BOOL sub(Ivec& src);			//  subtract:  us -= src;
	void incr0(int indx);			//  increment@indx (zero-based)
	void incr1(int indx);			//	increment@indx (one-based)
	void decr0(int indx);			//  decrement@indx (zero-based)
	void decr1(int indx);			//	decrement@indx (one-based)
								//--------------------------------------
    void zero(void);				// fill vector with zeroes
	void count(void);				// fill with counting.
	void textOut(FILE* fptr, char* title, int linewidth);
									// make printout in stream 'fptr'
};

class Imat
//==============================================================================
// An arbitrary-sized INTEGER matrix class for use as indices, flags, etc.,
// inspired by 'ivector' functions in 'Numerical Recipes in C', and a storage
// method compatible with IMSL libraries (linkable FORTRAN). PLEASE PLEASE
// PLEASE don't directly access the 'a' array; use the 'get()' and 'put()'
// member functions, because I may change the matrix storage methods later to
// add 'sparse' matrix types, etc.
//
// Written for use with the usual 0-based array indices of C,C++, BUT you can
// use the 1-based indices common to FORTRAN code and all the "Numerical Recipes
// in C" code by using 'get1()' and put1()' instead of the zero-based 'get0()'
// and 'put0()'.
//	12/14/98: added dyn.alloc'd array of pointers to each row so we are
//				also compatible with Numerical Recipes code.
//  12/28/2002: DEBUG--wipeCopy(): fixed reversed x,y args in call to sizer()!
{

private:			    		//-----------------DATA-----------------
    int *a;				   			// dynam. allocated space at this ptr holds
									// 2D array of matrix entries in row-major
									// order--A00,A10,A20,.. (all of col 0,then
									// all of col 1, etc) stored value is
									// the matrix element value.
    int xsize;						// # columns in the matrix.
	int ysize;						// # of rows

public:			    			//-----------------FCNS-----------------
    Imat(void);						// make an 'empty' object
    ~Imat(void);					// destructor; de-alloc memory
    BOOL wipe(void);				// release space for matrix.
    void sizer(int iysize,int ixsize);	// allocate space for matrix;
									// sizer(#rows,#columns)
	void sizer(Imat& src);

	int getXsize(void) { return(xsize);};
	int getYsize(void) { return(ysize);};
    BOOL wipeCopy(Imat& src);		// auto-resizing entry-by-entry copy
    								// (no change to our object's name)
	void put0(int row,int col, int val);
	int  get0(int row, int col);	// read/write 'val' at  row,col.
									// (uses zero-based indexing)
	void put1(int row,int col, int val);
	int  get1(int row, int col);	// read/write 'val' at  row,col.
									// (uses ONE-based indexing)
	void put0_Row(int row,int c0,...);// Write a row of values into matrix.
									// (CAREFUL! No over/under-run checks!)
	void put0_Col(int col,int r0,...);// Write a column of values into matrix.
									// (CAREFUL! No over/under-run checks!)

	void put0_OR(int row, int col, int val);	// in-place OR with val
	void put1_OR(int row, int col, int val);	// (zero, one-based addr)
	void put0_AND(int row, int col, int val);	// in-place AND with val
	void put1_AND(int row, int col, int val);	// (zero, one-based addr)
									// Write 'val' bits if 'mask' bits are true.
	void put0_mask(int row, int col, int val, int mask);
	void put1_mask(int row, int col, int val, int mask);

	int* getPtr0(int row=0, int col=0) { //(zero-based indexing)
									// return pointer to element;
									// default to 1st element.
		return(&(a[row + col*ysize])); };
	int* getPtr1(int row=1, int col=1) {// (one-based indexing)
		return(&(a[(row-1) + (col-1)*ysize])); };
								//--------------------------------------
	void swapRows1(int rA,int rB);	// swap contents of rows rA,rB
									// (rA,rB using 1-based indexing)
	void swapCols1(int cA,int cB);	// swap contents of columns cA,cB
									// (cA,cB use 1-based indexing)
    BOOL add(Imat& src);			//  add; us += src;
    BOOL sub(Imat& src);			//  subtract:  us -= src;
	BOOL transpose(Imat& src);		// in-place is OK(src=*this)
	BOOL mmpy(Imat& left, Imat& rt);// replace our current contents
									// with result of matrix multiply
									// [left][rt]. Return FALSE on err.
	BOOL augment(Imat& left, Imat& right);
									// Merge two matrices side-by-side.
									// Return FALSE on err.
	BOOL stack(Imat& top, Imat& bot);
									// Merge to matrices, top-over-bot.
									// Return FALSE on err.
								//--------------------------------------
    void zero(void);				// fill matrix with zeroes
	void ident(void);				// make identity matrix.
	void countR(void);				// fill with counting; row-major
	void countC(void);				// fill with counting; column-major
	void textOut(FILE* fptr, char* title, int linewidth);
									// make printout in stream 'fptr'
	void prnt(char *name);			// Print to debug window via VC++ 'TRACE'
};

#endif // (end of conditional-compile test for __MY_VECMAT at start of file.)
