//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
//
//	Home-brewed vector/matrix classes; helpful for graphics tasks.
//		See my_vecmat.h for detailed explanation.
//
//	01/20/2005 J. Tumblin -- Created.
//  02/15/2007 J. Tumblin -- MS-Visual Studio requires " delete [xsize]a; "
//                           but DevCPP accepts only "delete [] a;". Updated.
//
//#include "stdafx.h"					// for VC++ precompiled headers.
#include "my_vecmat.h"

//==============================================================================
//========================                ======================================
//========================   4x4 Vectors  ======================================
//========================                ======================================
//==============================================================================

#undef VV
#define VV 4			// 4 element vectors, 4x4 element matrix

//======================= Vec4 Member fcns =====================================
//=======================------------------=====================================



void Vec4::prnt(void)
//------------------------------------------------------------------------------
// Unlabeled prettyprint of Vec4 elements.
{
    DPF("\nvec4:\t %f,/t%f,/t%f/t%f/n",row[0],row[1],row[2],row[3]);

}

void Vec4::prnt(char *name)
//------------------------------------------------------------------------------
// Labeled prettyprint of Vec4 elements.
{
	DPF("\n%s:\t%f\t%f\t%f\t%f\n",name,row[0],row[1],row[2],row[3]);
}

VECTYPE Vec4::norm3(Vec4& src)
//------------------------------------------------------------------------------
//Replace 'self' with unit-length version of 'src' vector; return length.
// NOTE!  norm3 computes only (x,y,z) terms; *IGNORES* 4th 'w' term!
{
int i;
VECTYPE length = (VECTYPE)0.0;
VECTYPE tmp    = (VECTYPE)0.0;

    for(i=0; i< VV-1; i++)
	{									// get vector length.
		length += src.row[i] * src.row[i];
	}
    tmp = length - (VECTYPE)1.0;		// is it  approx 1.0 already?
	if(tmp*tmp < JT_EPSILON)
	{
		*this = src;
		return((VECTYPE)1.0);			// already normalized.
	}

#if(IS_FLOAT==TRUE)
    length = sqrtf(length);
#endif

#if(IS_DOUBLE==TRUE)
    length = sqrt(length);
 //   DPF("Vec4:doublenorm");
#endif

    if(length*length > JT_EPSILON)
	{								// normalize it.
		tmp = (VECTYPE)1.0/ length;
		for(i=0; i< VV-1; i++)
		{
	 		row[i] = tmp * src.row[i];
		}
		row[3] = src.row[3];          // just COPY the 'w' or 4th term.
		return(length);
	}
    else
	{
#if(DIAGNOSTICS==TRUE)
		DPF("\n ???Vec4::norm3() given 0-length src vector!!!");
#endif
		return((VECTYPE)0.0);
	}
}

VECTYPE Vec4::norm3(void)
//------------------------------------------------------------------------------
//Replace 'self' with unit-length version; return length.
// NOTE!  norm3 computes only (x,y,z) terms; *IGNORES* 4th 'w' term!
{
int i;
VECTYPE length = (VECTYPE)0.0;
VECTYPE tmp    = (VECTYPE)0.0;

    for(i=0; i< VV-1; i++)
	{									// get vector length.
		length += row[i] * row[i];
	}
    tmp = length - (VECTYPE)1.0;		// is it ~1.0 already?
	if(tmp*tmp < JT_EPSILON)
	{
		return((VECTYPE)1.0);			// already normalized.
	}

#if(IS_FLOAT==TRUE)
    length = sqrtf(length);
#endif

#if(IS_DOUBLE==TRUE)
    length = sqrt(length);
 //   DPF("Vec4:doublenorm");
#endif

    if(length*length > JT_EPSILON)
	{								// normalize it.
		tmp = (VECTYPE)1.0/ length;
		for(i=0; i< VV-1; i++)
		{
	 		row[i] = tmp * row[i];
		}                         // ignore the 'w' or 4th term.
		return(length);
	}
    else
	{
#if(DIAGNOSTICS==TRUE)
		DPF("\n ???Vec4::norm3(void) given 0-length src vector!!!");
#endif
		return((VECTYPE)0.0);
	}
}

VECTYPE Vec4::norm4(void)
//------------------------------------------------------------------------------
//Replace 'self' with unit-length version of 'src' vector; return length.
// NOTE!  norm4 INCLUDES ALL 4 TERMS! (x,y,z,w)! terms!  w=1 may cause trouble!
{
int i;
VECTYPE length = (VECTYPE)0.0;
VECTYPE tmp    = (VECTYPE)0.0;

    for(i=0; i< VV; i++)
	{									// get vector length.
		length += row[i] * row[i];
	}
    tmp = length - (VECTYPE)1.0;		// is it ~1.0 already?
	if(tmp*tmp < JT_EPSILON)
	{
		return((VECTYPE)1.0);			// already normalized.
	}

#if(IS_FLOAT==TRUE)
    length = sqrtf(length);
#endif

#if(IS_DOUBLE==TRUE)
    length = sqrt(length);
 //   DPF("Vec4:doublenorm");
#endif

    if(length*length > JT_EPSILON)
	{								// normalize it.
		tmp = (VECTYPE)1.0 / length;
		for(i=0; i< VV; i++)
		{
	 		row[i] = tmp * row[i];
		}
		return(length);
	}
    else
	{
#if(DIAGNOSTICS==TRUE)
		DPF("\n ???Vec4::norm(void) given 0-length src vector!!!");
#endif
		return((VECTYPE)0.0);
	}
}

void Vec4::copy(Vec4& src)
//------------------------------------------------------------------------------
{
int i;

	for(i=0; i<VV; i++) row[i] = src.row[i];
}

void Vec4::add(Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// Vector addition: [this]= src1 + src2
{
int i;
VECTYPE tmp;

    for(i=0; i<VV; i++)
	{
		tmp = src1.row[i] + src2.row[i];
		row[i] = tmp;
	}
}

void Vec4::addIn(Vec4& src)
//------------------------------------------------------------------------------
// add in: [this] = [this] + src
{
int i;

    for(i=0; i<VV; i++)
    {
        row[i] = row[i] + src.row[i];
    }
}

void Vec4::sub(Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// subtract vectors: [this] = src1 - src2
{
int i;
VECTYPE tmp;

    for(i=0; i<VV; i++)
	{
		tmp = src1.row[i] - src2.row[i];
		row[i] = tmp;
	}
}

void Vec4::scale(Vec4& src1, VECTYPE c)
//------------------------------------------------------------------------------
// Multiply by scalar: [this]= src1 * c
{
int i;

    for(i=0; i<VV; i++)
	{
		row[i] = src1.row[i]*c;
	}
}

void Vec4::scale(VECTYPE c)
//------------------------------------------------------------------------------
// Multiply self by scalar: [this]= [this]*c
{
int i;

    for(i=0; i<VV; i++)
	{
		row[i] = row[i]*c;
	}
}

void Vec4::lerp3(VECTYPE c, Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// 3-element linear blend:   [this]= src1.*(c) + src2.*(1-c)
// OK to use [this] as either input. CAUTION: blends ONLY rows 0,1,2.
{
int i;
VECTYPE tmp;

    for(i=0; i<VV-1; i++)
	{
		tmp = src1.row[i]*c + src2.row[i]*(1.0 - c);
		row[i] = tmp;
	}
}

void Vec4::lerp4(VECTYPE c, Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// 4-element linear blend;  [this]= src1.*(c) + src2.*(1-c)
// OK to use [this] as either input. WARNING! BLENDS 4th element row[3]!
{
int i;
VECTYPE tmp;

    for(i=0; i<VV; i++)
	{
		tmp = src1.row[i]*c + src2.row[i]*(1.0 - c);
		row[i] = tmp;
	}
}

VECTYPE Vec4::dot3(Vec4& src)
//------------------------------------------------------------------------------
// vector dot product: return( [this] <dot> src )
// CAREFUL!!! IGNORES the 4th element in dot product:
// out = x1*x2 + y1*y2 + z1*z2.  (IGNORES w1, w2!)
{
VECTYPE out=(VECTYPE)0.0;
int i;

    for(i=0; i <VV-1; i++)
	{
		out += src.row[i] * row[i];
	}
    return(out);
}
VECTYPE Vec4::dot4(Vec4& src)
//------------------------------------------------------------------------------
// vector dot product: return( [this] <dot> src )
// CAREFUL!!! INCLUDES the 4th element in dot product:
// out = x1*x2 + y1*y2 + z1*z2 + w1*w2.
{
VECTYPE out=(VECTYPE)0.0;
int i;

    for(i=0; i <VV; i++)
	{
		out += src.row[i] * row[i];
	}
    return(out);
}

VECTYPE Vec4::dot3(Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// vector dot product: [this] = src1 <dot> src2
// CAREFUL!!! IGNORES the 4th element in dot product:
// out = x1*x2 + y1*y2 + z1*z2.  (IGNORES w1, w2!)
{
VECTYPE out=(VECTYPE)0.0;
int i;

    for(i=0; i <VV-1; i++)
	{
		out += src1.row[i] * src2.row[i];
	}
    return(out);
}

VECTYPE Vec4::dot4(Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// vector dot product: [this] = src1 <dot> src2
// CAREFUL!!! INCLUDES the 4th element in dot product:
// out = x1*x2 + y1*y2 + z1*z2 + w1*w2.
{
VECTYPE out=(VECTYPE)0.0;
int i;

    for(i=0; i <VV; i++)
	{
		out += src1.row[i] * src2.row[i];
	}
    return(out);
}

void Vec4::cross3(Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// Cross product of the 1st 3 terms of two 4-element vectors;
// IGNORES the 4th element; sets it to zero.
{
Vec4 tmp;
Vec4 *pOut;

    pOut = this;                       // put output in 'this', but check:
    if(&src1 == this || &src2 == this) // are we using OURSELVES as an input?
    {
        pOut = &tmp;                   // then put computed result in tmp;
    }
    pOut->row[0] = src1.row[1]* src2.row[2] - src1.row[2]* src2.row[1];
    pOut->row[1] = src1.row[2]* src2.row[0] - src1.row[0]* src2.row[2];
    pOut->row[2] = src1.row[0]* src2.row[1] - src1.row[1]* src2.row[0];
    pOut->row[3] = (VECTYPE)0.0;
    if(pOut == &tmp)
    {                                  // is result in temp storage?
        copy(tmp);                     // then copy it.
    }
}

void Vec4::mmpy(Mat4& srcM, Vec4& srcV)
//------------------------------------------------------------------------------
//  Matrix-Vector multiply: [this]=[srcM][srcV]
{
int i,j;
Vec4 dest;							// local store for result;

    for(j=0; j<VV; j++)
	{
		for(i=0; i<VV; i++)
		{
		    dest.row[j] += srcM.colrow[i][j] * srcV.row[i];
		}
	}
    *this = dest;					// copy results to calling Vec4
}

void Vec4::multVec(Vec4& src1, Vec4& src2)
//------------------------------------------------------------------------------
// Element-by-element mpy of src1 and src2 vectors to make [this]:
// [this].row[x] = src1.row[x] * src2.row[x].  Useful for color calcs.
{
int i;

    for(i=0; i<VV; i++)
    {
        row[i] = src1.row[i] * src2.row[i];
    }
}

BOOL Vec4::isZero(void)
//------------------------------------------------------------------------------
// TRUE iff all elements are integer-valued.
{
int i;
BOOL out;
	out = TRUE;
	for(i=0; i<VV; i++)
	{
		if(row[i] != (VECTYPE)0.0)
		{							 // stop if ANY non-zero value found.
			out = FALSE;
			break;
		}
	}
	return(out);
}

//======================= Mat4 Member fcns =====================================
//=======================------------------=====================================


void Mat4::zero(void)
//------------------------------------------------------------------------------
// clear matrix to zero.
{
int i,j;

    for(i=0; i<VV; i++)
	{
		for(j=0; j<VV; j++)
		{
	 		colrow[i][j] = (VECTYPE)0.0;
	 	}
	}
}

void Mat4::ident(void)
//------------------------------------------------------------------------------
// set to identity matrix.
{
int i;

    zero();								// apply Mat4::zero() to the
										// current instance of Mat4;
    for(i=0; i< VV; i++)
	{
		colrow[i][i] = (VECTYPE)1.0;	// put 1.0 on the diagonal
	}
}

void Mat4::prnt(void)
//------------------------------------------------------------------------------
// prettyprint a matrix.
{
int i,j;


    DPF("\n----------Mat4------------\n");
    for(j=0; j< VV; j++)
	{
		for(i=0; i< VV; i++)
		{
	    	DPF("%f\t",colrow[i][j]);
	    }
		DPF("\n");
	}
}

void Mat4::prnt(char *name)
//------------------------------------------------------------------------------
// prettyprint a LABELED matrix.
{
int i,j;


    DPF("\n---------Mat4:%s------------\n",name);
    for(j=0; j< VV; j++)
	{
		for(i=0; i< VV; i++)
		{
			DPF("%f\t",colrow[i][j]);
	    }
		DPF("\n");
	}
}

void Mat4::mmpy(Mat4& left, Mat4& rt)
//------------------------------------------------------------------------------
/*  Matrix-Matrix mpy: 			[t h i s]   [l e f t][r t - -]
								[- - - -] = [- - - -][- - - -]
								[- - - -]   [- - - -][- - - -]
								[- - - -]   [- - - -][- - - -]		  */
{
int i,j,h;      // column, row, position
Mat4 tmp;		// Compute answer here first, then copy to output
                // (permits use of 'this' as left, right, or both)

    for(j=0; j<VV; j++)
	{
		for(i=0; i<VV; i++)
		{
			tmp.colrow[i][j]=(VECTYPE)0.0;
			for(h=0; h<VV; h++)
			{
				tmp.colrow[i][j] +=
				left.colrow[h][j] * rt.colrow[i][h];
			}
	    }
	}
    *this = tmp;						// copy result to 'self'
}

void Mat4::transpose(void)
//------------------------------------------------------------------------------
//   Replace 'self' with it's transpose.
{
int i,j;
VECTYPE tmp;

    for(i=0; i<VV; i++)
		{
		for(j=0; j<i; j++)
		{								// swap anti-diagonals.
	    	tmp = colrow[i][j];
	    	colrow[i][j] = colrow[j][i];
	    	colrow[j][i] = tmp;
	    }
	}
}

void Mat4::copy(Mat4& src)
//------------------------------------------------------------------------------
// Replace 'self' contents by copying contents of src
{
int i,j;

	for(j=0; j<VV; j++)
	{
		for(i=0; i<VV; i++)
		{
			colrow[i][j] = src.colrow[i][j];
		}
	}
}

void Mat4::makeTrans(VECTYPE xoff, VECTYPE yoff, VECTYPE zoff)
//------------------------------------------------------------------------------
// Make a 3D translation matrix;
{
	ident();			// write an identity matrix over 'self'
	colrow[3][0] = xoff;	// write the x,y,z offset values
	colrow[3][1] = yoff;
	colrow[3][2] = zoff;
};

void Mat4::makeRot(Vec4& axis, VECTYPE theta_rad)
//------------------------------------------------------------------------------
// Make 3D rotation matrix 'it' by rotating 'theta_rad' radians about
// the 'axis' vector, rotation centered at the origin.  This replaces
// concatenation of xrot,yrot,zrot matrices; already concat'd here.
{
Vec4 normaxis;						// normalized axis vector.
VECTYPE axlong;						// length of axis vector
VECTYPE cos_t,sin_t;
VECTYPE nx2,ny2,nz2;				// squared terms.


    axlong = normaxis.norm3(axis);	// get a normalized axis;

#if(IS_FLOAT==TRUE)				 	// get proper type sin,cos.
    cos_t = cosf(theta_rad);
    sin_t = sinf(theta_rad);
#endif
#if(IS_DOUBLE==TRUE)
    cos_t = cos(theta_rad);
    sin_t = sin(theta_rad);
#endif

#define NX normaxis.row[0]			// 'shorthand' defines.
#define NY normaxis.row[1]
#define NZ normaxis.row[2]

    nx2 = NX * NX;
    ny2 = NY * NY;
    nz2 = NZ * NZ;
    ident();						// write ident matrix, then
					// main diagonal
    colrow[0][0] = nx2 + ((VECTYPE)1.0-nx2)*cos_t;
    colrow[1][1] = ny2 + ((VECTYPE)1.0-ny2)*cos_t;
    colrow[2][2] = nz2 + ((VECTYPE)1.0-nz2)*cos_t;
					// 1st cross-diagonal
    colrow[1][0] = NX*NY*((VECTYPE)1.0-cos_t) - NZ*sin_t;
    colrow[0][1] = NX*NY*((VECTYPE)1.0-cos_t) + NZ*sin_t;
					// 2nd cross-diagonal
    colrow[2][0] = NX*NZ*((VECTYPE)1.0-cos_t) + NY*sin_t;
    colrow[0][2] = NX*NZ*((VECTYPE)1.0-cos_t) - NY*sin_t;
					// 3rd cross-diagonal
    colrow[2][1] = NY*NZ*((VECTYPE)1.0-cos_t) - NX*sin_t;
    colrow[1][2] = NY*NZ*((VECTYPE)1.0-cos_t) + NX*sin_t;
}

BOOL Mat4::isZero(void)
//-----------------------------------------------------------------------------
// TRUE iff all elements are zero-valued.
{
int i,j;
BOOL out;

	out = TRUE;
	for(j=0; j<VV; j++)
	{
		for(i=0; i<VV; i++)
		{
			if(colrow[i][j] != (VECTYPE)0.0)
			{
				i = j  = VV;				// stop if ANY non-zero found
				out = FALSE;
			}
		}
	}
	return(out);
}

BOOL Mat4::isInteger(void)
//-----------------------------------------------------------------------------
// TRUE iff all elements are integer-valued.
{
int i,j;
BOOL out;

	out = TRUE;
	for(j=0; j<VV; j++)
	{
		for(i=0; i<VV; i++)
		{
			if((VECTYPE)((int)colrow[i][j]) != colrow[i][j])
			{
												// stop if ANY non-
				i = j  = VV;					// integer value found.
				out = FALSE;
			}
		}
	}
	return(out);
}

void Mat4::copyTo(Matrx& out)
//------------------------------------------------------------------------------
// Copy contents to 'Matrx' class object 'out'.  Resize 'out' as needed.
{
int i,j;

#if(PESKYWARNINGS==TRUE)
	if((out.getXsize()!=VV) || (out.getYsize()!=VV))
		DPF("!Mat4::copyTo() resized the destination matrix!");
#endif

	out.sizer(VV,VV);
	for(j=0; j<VV; j++)
	{
		for(i=0; i<VV; i++)
		{
			out.put0(j,i,colrow[i][j]);
		}
	}
}

//=========================== Matrx Member fcns ================================
//===========================------------------=================================


Matrx::Matrx(void)
//------------------------------------------------------------------------------
// Constructor for an 'empty' matrix object.
{
    a = NULL;					// make an empty array for pixels.
    xsize = 0;
	ysize = 0;
}


Matrx::~Matrx(void)
//------------------------------------------------------------------------------
// destructor; de-alloc memory before we destroy the object.
{
    if(a != NULL)
	{
    	delete[] a;		// release memory, if any
    	a = NULL;					// (for VC++)
    }
#if (PESKYWARNINGS==TRUE)
    else
	{
		DPF("\n!!empty matrix called destructor!!\n");
	}
#endif
    xsize = 0;						// set size to zero.
	ysize = 0;
}


BOOL Matrx::wipe(void)
//------------------------------------------------------------------------------
// release space for matrix; make our size zero. If already zero sized,
// return FALSE, else return TRUE.
{
    if(a==NULL)
	{								// array is already empty;
#if(PESKYWARNINGS==TRUE)
    	DPF("\n!!Matrx::wipe called on empty object!!\n");
#endif
		xsize = 0;					// be sure its other members are
									// also benign.
		ysize = 0;
		return(FALSE);
	}
    delete[] a;			// release all pixel memory.
	a = NULL;						// (for VC++)
    xsize = 0;
	ysize = 0;
    return(TRUE);
}

void Matrx::sizer(int iysize,int ixsize)
//------------------------------------------------------------------------------
// allocate space for matrix.
{
    if(a != NULL)
	{								// do we have ANY entries at all?
#if(PESKYWARNINGS==TRUE)
    	DPF("\n!!Matrx::sizer called on nonempty object!!\n");
#endif
									// YES. correct # of entries?
		if(xsize != ixsize || ysize != iysize)
		{
			wipe();					// NO discard all existing entries;
		}
		else
		{
			return;					// YES. no resizing needed!
		}
	}
    a = new VECTYPE[ixsize*iysize];	//make array of size 'xsize*ysize'
#if(DIAGNOSTICS==TRUE)
    if(a==NULL)
	{
		DPF("\n Matrx::sizer() unable to allocate memory!\n");
		exit(1);
	}
#endif
    xsize = ixsize;
	ysize = iysize;
}

void Matrx::sizer(Matrx& src){
//------------------------------------------------------------------------------
// Allocate memory for the same number of elements found in 'src'.

	if(src.a==NULL || src.xsize==0 || src.ysize==0)
	{
		DPF("\n!!Matrx::sizer called with empty src object!\n");
		exit(1);
	}
    if(a != NULL)
	{								// do we have ANY pixels at all?
#if(PESKYWARNINGS==TRUE)			// YES;  warn user.
    	DPF("\n!!Matrx::sizer called on nonempty object!!\n");
#endif
		if(xsize != src.xsize || ysize != src.ysize)
		{							// do we have the correct # of pix?
			wipe();					// NO-- so discard all & start over:
		}
		else
		{						// yes-- no resizing needed!
			return;
		}
	}
    a = new VECTYPE[src.xsize * src.ysize];	// make a pixel array/
#if(DIAGNOSTICS==TRUE)
    if(a==NULL)
	{
		DPF("\n Matrx::sizer() unable to allocate memory!\n");
		exit(1);
	}
#endif
    xsize = src.xsize;
	ysize = src.ysize;
}

BOOL Matrx::copy(Matrx& src)
//------------------------------------------------------------------------------
// auto-resizing entry-by-entry copy (no change to our object's name).
// If size of 'src' is different from us, 'wipe' our current contents,
//  and change our size to match 'src' before copy.
// If a 'wipe' was necessary, return 'FALSE', else return 'TRUE'.
{
BOOL out=TRUE;

    if(xsize != src.xsize || ysize != src.ysize)
	{
    								// src size != us, so wipe
		wipe();
		sizer(src);
		out=FALSE;
	}
	memcpy(a, src.a, xsize * ysize * sizeof(VECTYPE));
    return(out);
}

void Matrx::put0(int row,int col, VECTYPE val)
//------------------------------------------------------------------------------
// write 'val' from spec'd row, col. using zero-based indexing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
		a[row + col*ysize] = val;
}

void Matrx::put1(int row, int col, VECTYPE val)
//------------------------------------------------------------------------------
// read 'val' from spec'd row, col.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
		a[(row-1) + (col-1)*ysize] = val;
}

void Matrx::put0Row(int row, VECTYPE c0,...)
//------------------------------------------------------------------------------
// put0Row(rr,c0,c1,c2,c3) writes a new row of coefficients on row 'rr'.  The
// 0-th element (leftmost) is c0, followed by c1,c2,c3, etc.
// CAREFUL! DOES NOT CHECK FOR TOO MANY/TOO FEW ARGUMENTS!!
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
#endif

va_list vl;									// holds variable argument list ...
int j;

	put0(row,0,c0);							// write the fixed argument, and
	va_start(vl,c0);						// (start var. arg. list process)
	for(j=1; j<getXsize(); j++)				// an entire row of
	{
		put0(row,j,va_arg(vl,VECTYPE));		// unspecified arguments.
	}
	va_end(vl);								// finished variable arg. process.
}

void Matrx::put0Row(int row, Vec4 &src)
//------------------------------------------------------------------------------
// put0Row(rr,myvec) copies row vector 'myvec' to row 'rr' in 'this' matrix.
// The 0-th element (leftmost) on 'this' row is the 0-th element in myvec.
// CAREFUL! DOES NOT CHECK MATRIX AND VECTOR SIZES
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);			// out-of-range row number!
	ASSERT(xsize>=4);						// matrix too small!
#endif

	put0(row,0,src.row[0]);
	put0(row,1,src.row[1]);
	put0(row,2,src.row[2]);
	put0(row,3,src.row[3]);
}

void Matrx::put0Col(int col, VECTYPE r0,...)
//------------------------------------------------------------------------------
// put0Col(cc,r0,r1,c2,r3) writes a new column of coefficients on column 'cc'.
// The 0-th element (uppermost) is set to equal r0, followed by r1,r2,r3, etc.
// CAREFUL!  DOES NOT CHECK FOR TOO MANY/TOO FEW ARGUMENTS!!
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(col>=0 && col<xsize);
#endif

va_list vl;									// holds variable argument list...
int i;

	put0(0,col,r0);							// write the fixed argument, and
	va_start(vl,r0);						// (start var. arg. list process)
	for(i=1; i<getYsize(); i++)
	{
		put0(i,col,va_arg(vl,VECTYPE));
	}
	va_end(vl);								// finished var. arg. list process.
}

void Matrx::put0Col(int col, Vec4 &src)
//------------------------------------------------------------------------------
// put0Col(cc,myvec) copies column vector 'myvec' to column 'cc' of 'this'.
// The 0-th element (topmost) on the column is set to equal the 0-th in the
// vector, and so forth.
// CAREFUL! DOES NOT CHECK MATRIX AND VECTOR SIZES!
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(col>=0 && col<xsize);			// out-of-range row number!
	ASSERT(ysize>=4);						// matrix too small!
#endif

	put0(0,col,src.row[0]);
	put0(1,col,src.row[1]);
	put0(2,col,src.row[2]);
	put0(3,col,src.row[3]);
}

void Matrx::copy0Col(int destCol, Matrx &src, int srcCol)
//------------------------------------------------------------------------------
// Copy one column from 'src' matrix.  Column 'srcCol' of matrix 'src' is copied
// element-by-element to column 'destCol' in 'this'.
// CAREFUL! DOES NOT CHECK ALL MATRIX AND VECTOR SIZES!
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(destCol>=0 && destCol<xsize);		// out-of-range column number!
	ASSERT(srcCol>=0 && srcCol<src.getXsize());	// out-of-range column number!
	ASSERT(src.getYsize()==getYsize());			// column size mismatch!!
#endif

int i,imax;

	imax = src.getYsize();
	if(imax > getYsize())						// is src column size > dest?
	{
		imax = getYsize();						// truncate, and
		ASSERT(FALSE);							// complain.
	}
	for(i=0; i<imax; i++)				// do element-by-element copy.
	{
		put0(i,destCol,src.get0(i,srcCol));
	}
}

VECTYPE Matrx::get0(int row, int col)
//------------------------------------------------------------------------------
// read 'val' from spec'd row, col.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
	return(a[row + col*ysize]);
}

VECTYPE Matrx::get1(int row, int col)
//------------------------------------------------------------------------------
// read 'val' from spec'd row, col. using 1-based indexing
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
	return(a[(row-1) + (col-1)*ysize]);
}

BOOL Matrx::add(Matrx &left, Matrx& rt)
//------------------------------------------------------------------------------
// 'this' = left + right. Return FALSE if mismatched matrix sizes.
{
int i;
Matrx* ptmp;
BOOL useTmp;

	ptmp = this;					//  point to ourselves, the output matrix;
	useTmp = FALSE;					//  and assume we'll compute answers there.
	if(&left==this || &rt==this)	// but if output overwrites an input,
	{
		ptmp = new Matrx;			// make a temporary output matrix;
		ptmp->sizer(rt);
		useTmp=TRUE;				// and set flag so we copy it to output.
	}
									// check for size mismatches.
#if(DIAGNOSTICS==TRUE)
	ASSERT(xsize == left.getXsize() && xsize == rt.getXsize()); //ensure size match
	ASSERT(ysize == left.getYsize() && ysize == rt.getYsize());
#endif

	for(i=0; i<xsize*ysize; i++)
	{
		ptmp->a[i] = left.a[i] + rt.a[i];
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		copy(*ptmp); 						// replace 'this' with temp.
		ptmp->wipe();						// and discard temp. version.
		delete ptmp;
	}
	return(TRUE);
}

BOOL Matrx::sub(Matrx &left, Matrx& rt)
//------------------------------------------------------------------------------
// 'this' = left - right. Return FALSE if mismatched matrix sizes.
{
int i;
Matrx* ptmp;
BOOL useTmp;

									// check for size mismatches.
#if(DIAGNOSTICS==TRUE)
	ASSERT(xsize != left.getXsize() || xsize != rt.getXsize()); //size mismatch!
	ASSERT(ysize != left.getYsize() || ysize != rt.getYsize());
#endif

	ptmp = this;					//  point to ourselves, the output matrix;
	useTmp = FALSE;					//  and assume we'll compute answers there.
	if(&left==this || &rt==this)	// but if output overwrites an input,
	{
		ptmp = new Matrx;			// make a temporary output matrix;
		ptmp->sizer(rt);
		useTmp=TRUE;				// and set flag so we copy it to output.
	}
	for(i=0; i<xsize*ysize; i++)
	{
		ptmp->a[i] = left.a[i] - rt.a[i];
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		copy(*ptmp); 						// replace 'this' with temp.
		ptmp->wipe();						// and discard temp. version.
		delete ptmp;
	}
	return(TRUE);
}

void Matrx::addTo0(int row, int col, VECTYPE val)
//------------------------------------------------------------------------------
// Add 'val' to the element currently stored at (row,col).  Return FALSE if
// element doesn't exist.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
	a[row + col*ysize] += val;
}

void Matrx::addTo1(int row, int col, VECTYPE val)
//------------------------------------------------------------------------------
// Add 'val' to the element currently stored at (row,col).  Return FALSE if
// element doesn't exist.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
	a[(row-1) + (col-1)*ysize] += val;
}

void Matrx::ident()
//------------------------------------------------------------------------------
// Make 'this' into an identity matrix without resizing.  'this' must be square.
{
int i,imax;

	imax = getXsize();
	ASSERT(getYsize()==imax);				// non-square matrix!!
	zero();									// clear all contents
	for(i=0; i<imax; i++)
	{
		put0(i,i,(VECTYPE)1.0);
	}
}
void Matrx::mpyScalar(VECTYPE wt)
//------------------------------------------------------------------------------
// Multiply all elements in matrix by scalar value 'wt'.
{
int i;

	for(i=0; i<xsize*ysize; i++)
	{
		a[i] *= wt;
	}
}

BOOL Matrx::transpose(Matrx &src)
//------------------------------------------------------------------------------
{
int i,j,imax,jmax;
Matrx destmp;							// temp storage in case src==this.
Matrx *pDest;

	imax = src.getXsize();
	jmax = src.getYsize();
#if(DIAGNOSTICS==TRUE)
	ASSERT(imax==jmax);					// be sure 'src' is square
#endif
	pDest = this;						// point to the output;
	if(this==&src)						// if output is also the input, then
	{
		pDest = &destmp;				// point to a temporary output buffer.
	}
	pDest->sizer(jmax,imax);			// Set output size using swapped x,ysize

    for(i=0; i<imax; i++)
	{
		for(j=0; j<jmax; j++)
		{									// Copy input to output, but
	    	pDest->put0(j,i,get0(i,j));		// with swapped indices
	    }
	}
	return(TRUE);
}


BOOL Matrx::mmpy(Matrx& left, Matrx& rt)
//------------------------------------------------------------------------------
// Replace our current contents with result of matrix multiply [left][rt].
// Return FALSE on err.
// Uses a 'temp' matrix to store intermediate results, so a matrix object can
// use ITSELF as an argument to mmpy.
// EXAMPLE;  Matrx us,them;		us.mmpy(us,them)
// (this is SLOWER but more flexible than requiring non-self inputs)
{
Matrx *ptmp;
int r,c,k,rmax,cmax,kmax;
VECTYPE val;
BOOL useTmp;						// true if output held in temp. storage

#if(DIAGNOSTICS==TRUE)
	if(left.getXsize() != rt.getYsize())
	{
		DPF("Matrx::mmpy() size mismatch!!");
		return(FALSE);
	}
#endif
	cmax = rt.getXsize();			// #columns in  output== #columns in rt,
	rmax = left.getYsize();			// #rows in output==#rows in left.
	kmax = rt.getYsize();			// # elements in a col. of rt & row of left
	ptmp = this;					//  point to ourselves, the output matrix;
	useTmp = FALSE;					//  and assume we'll use it.
	if(&left==this || &rt==this)	// if output overwrites an input,
	{
		ptmp = new Matrx;			// make a temporary output matrix;
		useTmp=TRUE;				// and set flag so we copy it to output.
	}
	if(ptmp->getXsize()!=cmax || ptmp->getYsize()!=rmax)
	{
		ptmp->sizer(rmax,cmax);		// resize as needed.
	}
	for(c=0; c<cmax; c++) {
		for(r=0; r<rmax; r++) {
			val = (VECTYPE)0.0;		// accumulate a row/col multiply
			for(k=0; k<kmax; k++) {
				val += left.get0(r,k) * rt.get0(k,c);
				}
			ptmp->put0(r,c,val);
			}
		}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		copy(*ptmp); 						// replace 'this' with temp.
		ptmp->wipe();						// and discard temp. version.
		delete ptmp;
	}
	return(TRUE);
}

BOOL Matrx::augment(Matrx& left, Matrx& rt)
//------------------------------------------------------------------------------
// Combine two matrices side-by-side, resizing 'this' as needed, placing columns
// of 'left' adjacent to the columns of 'right'. Uses a 'temp' matrix to store
// intermediate results, a matrix object can use ITSELF as an argument.
// EXAMPLE; Matrx us,them;	us.augment(us,them);  us.augment(them,us);// is OK.
{
Matrx* ptmp;
int r,c,rmax,cmaxL, cmaxR;
BOOL useTmp;						// true if output held in temp. storage

	ASSERT(left.getYsize()==rt.getYsize()); // ?!?! column size mismatch!?!?!
	rmax = rt.getYsize();			// #rows in output==#rows in Right matrix.
	cmaxL = left.getXsize();		// #columns in left matrx,
	cmaxR = rt.getXsize();			// #columns in Right matrx,
	ptmp = this;					//  point to ourselves, the output matrix;
	useTmp = FALSE;					//  and assume we'll use it.
	if(&left==this || &rt==this)	// if output overwrites an input,
	{
		ptmp = new Matrx;			// make a temporary output matrix;
		useTmp=TRUE;				// and set flag so we copy it to output.
	}
	if((ptmp->getYsize()!=rmax) || (ptmp->getXsize()!=(cmaxL+cmaxR)) )
		ptmp->sizer(rmax,cmaxL+cmaxR);		// (resize as needed)
	for(r=0; r<rmax; r++)
	{
		for(c=0; c<cmaxL; c++)
		{
			ptmp->put0(r,c,left.get0(r,c));
		}
		for(c=0; c<cmaxR; c++)
		{
			ptmp->put0(r,c+cmaxL,rt.get0(r,c));
		}
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		copy(*ptmp);						// replace 'this' with temp.,
		ptmp->wipe();						// and discard temp. version.
		delete ptmp;
	}
	return(TRUE);
}

BOOL Matrx::stack(Matrx& top, Matrx& bot)
//------------------------------------------------------------------------------
// Combine two matrices top-tobottom, resizing 'this' as needed, placing rows
// of 'top' above rows of 'bot'. Uses a 'temp' matrix to store intermediate
// results, a matrix object can use ITSELF as an argument.
// EXAMPLE;  Matrx us,them;	us.stack(us,them);  us.stack(them,us);// is OK.
{
Matrx* ptmp;
int r,c,rmaxT,rmaxB,cmax;
BOOL useTmp;						// true if output held in temp. storage.

	ASSERT(top.getXsize()==bot.getXsize());	// ?!?! row size mismatch?!?!
	rmaxT = top.getYsize();					// #rows in top matrix,
	rmaxB = bot.getYsize();					// #rows in bottom matrix,
	cmax = top.getXsize();					// #cols in output==#cols in top
	ptmp = this;					// point to ourselves, the output matrix;
	useTmp = TRUE;					// and assume we'll use it.
	if(&top==this || &bot==this)	// if output overwrites an input,
	{
		ptmp = new Matrx;			// make a temporary output matrix,
		useTmp = TRUE;				// and set flag so we copy it to output.
	}
	if((ptmp->getYsize()!=(rmaxT+rmaxB)) || (ptmp->getXsize()!=cmax))
		ptmp->sizer(rmaxT+rmaxB,cmax);		// (resize as needed)
	for(c=0; c<cmax; c++)
	{
		for(r=0; r<rmaxT; r++)
		{
			ptmp->put0(r,c,top.get0(r,c));
		}
		for(r=0; r<rmaxB; r++)
		{
			ptmp->put0(r+rmaxT,c,bot.get0(r,c));
		}
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		copy(*ptmp);						// replace 'this' with temp.,
		ptmp->wipe();						// and discard temp version.
		delete ptmp;
	}
	return(TRUE);
}

void Matrx::swapRows1(int rA, int rB)
//------------------------------------------------------------------------------
// Exchange all elements in row A with all elements in row B, using
// 1-based indexing for rA,rB.
{
VECTYPE tmp;
int i;

	for(i=1; i<=xsize; i++)
	{
		tmp = get1(rA,i);
		put1(rA,i,get1(rB,i));
		put1(rB,i,tmp);
	}
}
void Matrx::swapCols1(int cA, int cB)
//------------------------------------------------------------------------------
// Exchange all elements in col A with all elements in column B, using
// 1-based indexing for cA,cB.
{
VECTYPE tmp;
int j;

	for(j=1; j<=ysize; j++)
	{
		tmp = get1(j,cA);
		put1(j,cA,get1(j,cB));
		put1(j,cB,tmp);
	}
}
void Matrx::swapRows0(int rA, int rB)
//------------------------------------------------------------------------------
// Exchange all elements in row A with all elements in row B, using
// 0-based indexing for rA,rB.
{
VECTYPE tmp;
int i;

	for(i=0; i<xsize; i++)
	{
		tmp = get0(rA,i);
		put0(rA,i,get0(rB,i));
		put0(rB,i,tmp);
	}
}
void Matrx::swapCols0(int cA, int cB)
//------------------------------------------------------------------------------
// Exchange all elements in col A with all elements in column B, using
// 0-based indexing for cA,cB.
{
VECTYPE tmp;
int j;

	for(j=0; j<ysize; j++)
	{
		tmp = get0(j,cA);
		put0(j,cA,get0(j,cB));
		put0(j,cB,tmp);
	}
}

BOOL Matrx::GaussJordan_elim(Matrx& a, Matrx& b)
//------------------------------------------------------------------------------
// Gauss-Jordan Elimination with full pivoting. Basic matrix inversion and
// solution to matrix eqn Ax=b  where A is an n by n matrix, x is one or more
// unknown column vectors (m==# column vectors), and b is the same number of
// known column vectors of the same size.
// Returns 'FALSE' if no solution (degenerate matrix).
// Does in-place computation; A matrix replaced by Ainverse, B replaced by x.
// a[row][col] holds matrix A, b[row][col] holds multiple b vectors
// Based "Numerical Recipes in C" code rewritten.
{
Ivec indxc,indxr,ipiv;
int i,icol,irow,j,k,l,ll,n,m;
VECTYPE big,dum,pivinv;

//#if(DIAGNOSTICS==TRUE)
	ASSERT(a.getXsize()==a.getYsize() && a.getYsize()==b.getYsize());
//#endif
	n = a.getXsize();				// A is an NxN matrix,
	m = b.getXsize();				// B is an NxM matrix.
	indxc.sizer(n);					// malloc three integer vectors that
	indxr.sizer(n);					// are indexed from 1 (not zero!)
	ipiv.sizer(n);					// ipiv[i] =1 if we have found a pivot for
									// the i-th column of A (and i-th row of b)
									// otherwise zero.
	for(j=1; j<=n; j++)
	{
		ipiv.put1(j,0);					// set ipiv entirely to zero.
	}
	for (i=1; i<=n; i++)				// (for diagonal element i,i)
	{									// for each COLUMN i to be reduced,
		big=(VECTYPE)0.0;
		for(j=1; j<=n; j++)				// Search the entire matrix the pivot;
		{								// look at row j, column k.
			if (ipiv.get1(j) != 1)		// If the j-th row of A doesn't yet
			{							// contain any pivot from a previously
										// reduced column(counted by i),
										// (In other words, if the j-th
										// diagonal doesn't yet have a pivot),
				for(k=1; k<=n; k++)		// THEN look at all the columns in
				{						// the j-th row;
					if(ipiv.get1(k)==0)	// if k-th column contains no pivots
					{					// from a previously reduced row, THEN
						if (fabs(a.get1(j,k)) >= big)
													// find the largest element
						{							// among j-th row,k-th col.
							big=(VECTYPE)fabs((double)a.get1(j,k));
							irow=j;					// and record its location
							icol=k;					// and value as the pivot.
						}
					}
					else if(ipiv.get1(k) > 1)
					{					// if k-th column was used TWICE, then
						ASSERT(FALSE);	// ("gaussj: Singular Matrix-1");
						return(FALSE);
					}
				}
			}// we now have a pivot for column k; mark it!
		}
		ipiv.incr1(icol);
		//
		// We now have the pivot element for column i; exchange rows if needed,
		// to put the pivot element on the diagonal.  Though we move the rows,
		// the COLUMNS are not physically interchanged, only relabeled:
		// indxc[i]==the column of the i-th pivot element,
		//			(remember we are reducing the i-th column) and
		// indxr[i]==the row in which the pivot element was originally located.
		// If indxr[i]!=indxc[i] there is an implied column interchange.
		// The b's will end up in the correct order, and the inverse matrix
		// will be scrambled by columns.
		//
		if (irow != icol)					// Do row interchange if needed;
		{
			a.swapRows1(irow,icol);			// swap rows in A,
			b.swapRows1(irow,icol);			// and in B.
		}
		indxr.put1(i,irow);					// record the pivot for i-th column
		indxc.put1(i,icol);
		if(a.get1(icol,icol) == 0.0)		// If the pivot is non zero,
		{
			ASSERT(FALSE);					//("gaussj: Singular Matrix-2");
			return(FALSE);
		}
											// divide the pivot row by the
		pivinv=(VECTYPE)1.0/a.get1(icol,icol);// pivot element at a[irow][icol]
		a.put1(icol,icol,(VECTYPE)1.0);
		for (l=1;l<=n;l++)
		{
			a.put1(icol,l,	a.get1(icol,l) * pivinv);
		}
		for (l=1;l<=m;l++)
		{
			b.put1(icol,l,	b.get1(icol,l) * pivinv);
		}
		for (ll=1;ll<=n;ll++)				// Now reduce the rows of column i
		{
			if (ll != icol)					// (except for the pivot)
			{
				dum=a.get1(ll,icol);
				a.put1(ll,icol,(VECTYPE)0.0);
				for (l=1;l<=n;l++)
				{
					a.put1(ll,l, a.get1(ll,l) - a.get1(icol,l)*dum);
				}
				for (l=1;l<=m;l++)
				{
					b.put1(ll,l, b.get1(ll,l) - b.get1(icol,l)*dum);
				}
			}
		}
	}
	// Finally, unscramble the column interchanges in the reverse order that
	// we recorded them.
	for (l=n; l>=1; l--)				// in reverse order,
	{
		if (indxr.get1(l) != indxc.get1(l))
		{
			a.swapCols1(indxr.get1(l),indxc.get1(l));
		}
	}
	return(TRUE);
}

BOOL Matrx::invert(void)
//------------------------------------------------------------------------------
// In-place matrix inversion, using Gauss-Jordan elimination with full pivoting.
// (Code copied from GaussJordan_elim() with all matrix b operations deleted).
// Based "Numerical Recipes in C" code.
// Returns 'FALSE' if no solution (degenerate matrix).
{
Ivec indxc,indxr,ipiv;
int i,icol,irow,j,k,l,ll,n,m;
VECTYPE big,dum,pivinv;

//#if(DIAGNOSTICS==TRUE)
	ASSERT(getXsize()==getYsize());
//#endif

	n = getXsize();					// A is an NxN matrix,
	m = 1;							// B is an NxM matrix; column vector.
	indxc.sizer(n);					// malloc three integer vectors that
	indxr.sizer(n);					// are indexed from 1 (not zero!)
	ipiv.sizer(n);					// ipiv[i] =1 if we have found a pivot for
									// the i-th column of A (and i-th row of b)
									// otherwise zero.
	for(j=1; j<=n; j++)
	{
		ipiv.put1(j,0);					// set ipiv entirely to zero.
	}
	for (i=1; i<=n; i++)				// (for diagonal element i,i)
	{									// for each COLUMN i to be reduced,
		big=(VECTYPE)0.0;
		for(j=1; j<=n; j++)				// Search the entire matrix the pivot;
		{								// look at row j, column k.
			if (ipiv.get1(j) != 1)		// If the j-th row doesn't yet
			{							// contain any pivot from a previously
										// reduced column(counted by i),
										// (In other words, if the j-th
										// diagonal doesn't yet have a pivot),
				for(k=1; k<=n; k++)		// THEN look at all the columns in
				{						// the j-th row;
					if(ipiv.get1(k)==0)	// if k-th column contains no pivots
					{					// from a previously reduced row, THEN
						if (fabs(get1(j,k)) >= big)
													// find the largest element
						{							// among j-th row,k-th col.
							big=(VECTYPE)fabs((double)get1(j,k));
							irow=j;					// and record its location
							icol=k;					// and value as the pivot.
						}
					}
					else if(ipiv.get1(k) > 1)
					{					// if k-th column was used TWICE, then
						ASSERT(FALSE);	// ("Matrx::invert() Singular Matrix-1");
						return(FALSE);
					}
				}
			}// we now have a pivot for column k; mark it!
		}
		ipiv.incr1(icol);
		//
		// We now have the pivot element for column i; exchange rows if needed,
		// to put the pivot element on the diagonal.  Though we move the rows,
		// the COLUMNS are not physically interchanged, only relabeled:
		// indxc[i]==the column of the i-th pivot element,
		//			(remember we are reducing the i-th column) and
		// indxr[i]==the row in which the pivot element was originally located.
		// If indxr[i]!=indxc[i] there is an implied column interchange.
		// The b's will end up in the correct order, and the inverse matrix
		// will be scrambled by columns.
		//
		if (irow != icol)					// Do row interchange if needed;
		{
			swapRows1(irow,icol);			// swap rows
		}
		indxr.put1(i,irow);					// record the pivot for i-th column
		indxc.put1(i,icol);
		if(get1(icol,icol) == 0.0)			// If the pivot is non zero,
		{
			ASSERT(FALSE);				//("Matrx::invert()Singular Matrix-2");
			return(FALSE);
		}
											// divide the pivot row by the
		pivinv=(VECTYPE)1.0/get1(icol,icol);// pivot element at [irow][icol]
		put1(icol,icol,(VECTYPE)1.0);
		for (l=1;l<=n;l++)
		{
			put1(icol,l,	get1(icol,l) * pivinv);
		}
		for (ll=1;ll<=n;ll++)				// Now reduce the rows of column i
		{
			if (ll != icol)					// (except for the pivot)
			{
				dum=get1(ll,icol);
				put1(ll,icol,(VECTYPE)0.0);
				for (l=1;l<=n;l++)
				{
					put1(ll,l, get1(ll,l) - get1(icol,l)*dum);
				}
			}
		}
	}
	// Finally, unscramble the column interchanges in the reverse order that
	// we recorded them.
	for (l=n; l>=1; l--)					// in reverse order,
	{
		if (indxr.get1(l) != indxc.get1(l))
		{
			swapCols1(indxr.get1(l),indxc.get1(l));
		}
	}
	return(TRUE);
}


VECTYPE Matrx::pythag(VECTYPE a, VECTYPE b)
//------------------------------------------------------------------------------
// (For internal use by SVD_full) From Numerical Recipes; added 'tmp1' to
// make it less cryptic and remove troublesome macro 'SQR'
{
//#def NRANSI
//#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)
//
//	Computes (sqrt(a^2 + b^2) without destructive overflow or underflow.
{
VECTYPE absa,absb,tmp1;

	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb)
	{
//		return absa*sqrt(1.0+SQR(absb/absa));
		tmp1 = absb/absa;
		return(absa*sqrt(1.0+ (tmp1*tmp1)));
	}
	else
	{
		if(absb==0.0) return(0.0);		// BOTH a and b are +/-0.0
		tmp1 = absa/absb;
		return (absb*sqrt(1.0+ (tmp1*tmp1)));
	}
}
//#undef NRANSI
//#undef SQR(a)
}

void Matrx::SVD_test(int cnt)
//------------------------------------------------------------------------------
// Test the SVD_full() routine by constructing a random matrix, decomposing it,
// testing the U,V matrices for orthogonality, and multiplying USV^T together
// to reconstruct the matrix. Repeat the test 'cnt' times.
{
Matrx A,U,S,sv,V;
int i,j,jmax,r,c;

	srand(time(NULL));					// use clock to seed the random# gen.
	for(i=0; i<cnt; i++)
	{
		r = 1 + rand()%10;				// random # of rows,cols between 1&10
		c = 1 + rand()%10;
		A.sizer(r,c);					// make A matrix
		A.randFill(-1000.0,1000.0);		// elements between +/-1000.0
		A.SVD_full(U,sv,V);				// do SVD,
		jmax = sv.getYsize();			// make square S matrix,
		S.sizer(jmax,jmax);
		S.zero();
		for(j=0; j<jmax; j++)
		{
			S.put0(j,j,sv.get0(j,0));
		}
		if(U.isOrthonormal(U)==FALSE)	// test for orthonormal column vectors
		{
			A.prnt("A matrix");
			U.prnt("U matrix");
			S.prnt("S vector");
			V.prnt("V matrix");
			S.get0(0,0);				// (put breakpoint here)
		}
		if(V.isOrthonormal(V)==FALSE)
		{
			A.prnt("A matrix");
			U.prnt("U matrix");
			S.prnt("S vector");
			V.prnt("V matrix");
			S.get0(0,0);				// (put breakpoint here)
		}

		V.transpose(V);					// multiply it out; find USV^T
		sv.mmpy(U,S);
		S.mmpy(sv,V);
		if(FALSE==A.isEqual(S,0.01))
		{
			A.prnt("A matrix");
			S.prnt("USVT matrix");
			A.sub(S,A);
			A.prnt("Error: USVT-A");
		}
		A.prnt("A matrix");				// Print 'before'
		S.prnt("USVT matrix");			// Print
		A.sub(S,A);
		A.prnt("Error: USVT-A");
	}
}

BOOL Matrx::isOrthonormal(Matrx& in, VECTYPE errmax)
//------------------------------------------------------------------------------
// Returns TRUE iff columns of 'in' are all unit-length and perpendicular to
// each other (e.g. dot product yields zero); otherwise returns false.
{
int i,j,k,imax,jmax;
VECTYPE tmp,length;

	imax = in.getXsize();
	jmax = in.getYsize();
	for(i=0; i<imax; i++)					// check columns for unit length
	{
		length = 0.0;
		for(j=0; j<jmax; j++)
		{
			tmp = in.get0(j,i);
			length += tmp*tmp;
		}
		if(length>(1.0+errmax) || length<(1.0-errmax))
		return(FALSE);
	}
	if(imax>=2)								// if more than 2 columns,
	{
		for(i=0; i<imax; i++)				// check all pairs of columns (i,j)
		{									//  compare column i with
			for(j=i+1;j<imax; j++)			//  column j, and
			{
				length=0.0;
				for(k=0; k<jmax; k++)
				{							// dot product; k-th element.
					length += in.get0(k,i)*in.get0(k,j);
				}
				if(length>errmax)return(FALSE);	// make sure dot product is zero.
			}
		}
	}
	return(TRUE);
}

BOOL Matrx::SVD_full(Matrx& u, Matrx& ss, Matrx& v)
//-----SVD (built from "Numerical Recipes in C" code)---------------------------
// Perform a full Singular Value Decomposition(SVD).  Using current contents
// ('this') as 'A',  find matrices that satisfy  A = U S V^T.  The 'A' matrix
// is unchanged, but U,S,V matrices are resized and contents (if any) replaced.
// Accepts input matrix A of any size, and if A is MxN, then U is MxM, ss is Mx1,
// and V is NxN.  S is a diagonal matrix (all zero except on diagonals), but to
// save space, the ss Matrx object holds only a column vector of diag. values.
// Returns FALSE on error.
//		Recall that columns of V (NOT V^T!!) are unit-length, orthogonal vectors
// that span input space (where x lives in the expression Ax=b). Similarly,
// columns of U are unit-length orthogonal vectors that span output space (where
// b lives in Ax=b), and remember that U^-1 = U^T and V^-1 = V^T (very handy!).
// The S matrix is zero everywhere except on diagonal elements denoted s_i.
// Element s_i is the 'singular value' for the i-th column of U and V.  Each s_i
// tells you how matrix A scales input x along one input dimension as it maps to
// to one output dimension ('dimension' defined by a column of U,V).
// ---'Rank' of A is the number of nonnegative, nonzero singular values.
// ---A zero, small, or negative singular value means an input dimension maps to
// zero in output (it is part of A's 'null space' (I prefer 'input nullspace')),
// or an output dimension is unreachable from any input dimension (it is part of
// A's 'range'; I prefer to say it is part of A's 'output null space').
//
// WARNING! S matrix output is NOT in greatest-to-smallest order! (common in many
// SVD implementations).  Use SVD_fullSort() instead.
//
// REALLY DUMB, port of code from NRecipes: just used get1/put1 in place of
// their 2D array accesses, and copied their 'pythag()' function & macros.
{
									// NRecipes macros (from NRutil.h)
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
static VECTYPE maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))
static int iminarg1,iminarg2;
#define IMIN(a,b) (iminarg1=(a),iminarg2=(b),(iminarg1) < (iminarg2) ?\
        (iminarg1) : (iminarg2))


int flag,i,its,j,jj,k,l,nm;			// NRecipes variables
VECTYPE anorm,c,f,g,h,s,scale,x,y,z;// NRecipes vars cont'd.
//VECTYPE *rv1;						// (replaced by Matrx rv1)
Matrx rv1;
int m,n;							// NRecipes input argument replacement vars.
Matrx aa,w;							// NRecipes input argument matrices

	aa.copy(*this);					// copy our current contents to 'a'
	m = aa.getYsize();				// find # of rows and columns
	n = aa.getXsize();
	v.sizer(n,n);
//	rv1=vector(1,n);
	rv1.sizer(1,n);					// set up our temp. array
	w.sizer(n,1);					// set up a column vector for singular vals

// ----------from Numerical Recipes(modified)--------------------------------
//void svdcmp(float **a, int m, int n, float w[], float **v)
// 4/23/02: JTumblin typed in comments from book, changed to modern indenting
// 4/23/02: JTumblin replaced all array accesses with get1(), put1() calls.
//			Assumes a[i][j] means row i, column j.
//
// Given a matrix a[1...m][1...n], this routine computes its singular value
// decomposition, A= UWV^T.  The matrix U replaces A on output.  The diagonal
// matrix of singular values W is output as a vector w[1...n].  The matrix V
// (not the transpose V^T) is output as v[1...n][1...n].
//{
//float pythag(float a, float b);
//int flag,i,its,j,jj,k,l,nm;
//VECTYPE anorm,c,f,g,h,s,scale,x,y,z,*rv1;

//	rv1=vector(1,n);
	g=scale=anorm=(VECTYPE)0.0;		// Householder reduction to bidiagonal form.
	for (i=1;i<=n;i++)
	{
		l=i+1;
//		rv1[1][i]=scale*g;
		rv1.put1(1,i,scale*g);
		//
		g=s=scale=(VECTYPE)0.0;
		if (i <= m)
		{
			for (k=i;k<=m;k++)
			{
				scale += fabs(aa.get1(k,i));
			}
			if (scale)
			{
				for (k=i;k<=m;k++)
				{
					//a[k][i] /= scale;
					aa.put1(k,i,aa.get1(k,i) / scale);
					s += aa.get1(k,i)*aa.get1(k,i);
				}
				f=aa.get1(i,i);
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				aa.put1(i,i,f-g);
				for (j=l;j<=n;j++)
				{
					for (s=(VECTYPE)0.0,k=i;k<=m;k++)
					{
						s += aa.get1(k,i)*aa.get1(k,j);
					}
					f=s/h;
					for (k=i;k<=m;k++)
					{
						//a[k][j] += f*a[k][i];
						aa.put1(k,j,
							aa.get1(k,j) + f*aa.get1(k,i));
					}
				}
				for (k=i;k<=m;k++)
				{
					//a[k][i] *= scale;
					aa.put1(k,i,
						aa.get1(k,i) * scale);
				}
			}
		}
		w.put1(i,1, scale *g);
		g=s=scale=(VECTYPE)0.0;
		if (i <= m && i != n)
		{
			for (k=l;k<=n;k++)
			{
				scale += fabs(aa.get1(i,k));
			}
			if (scale)
			{
				for (k=l;k<=n;k++)
				{
				//	a[i][k] /= scale;
					aa.put1(i,k,
						aa.get1(i,k) / scale);
					s += aa.get1(i,k)*aa.get1(i,k);
				}
				f=aa.get1(i,l);
				g = -SIGN(sqrt(s),f);
				h=f*g-s;
				aa.put1(i,l,f-g);
				for (k=l;k<=n;k++)
				{
//					rv1[1][k]=a[i][k]/h;
					rv1.put1(1,k,aa.get1(i,k)/h);
//
				}
				for (j=l;j<=m;j++)
				{
					for (s=(VECTYPE)0.0,k=l;k<=n;k++)
					{
						s += aa.get1(j,k)*aa.get1(i,k);
					}
					for (k=l;k<=n;k++)
					{
					//	a[j][k] += s*rv1.get1(1,k);
						aa.put1(j,k,
							aa.get1(j,k) + s*rv1.get1(1,k));
					}
				}
				for (k=l;k<=n;k++)
				{
					// a[i][k] *= scale;
					aa.put1(i,k, aa.get1(i,k)*scale);
				}
			}
		}
		anorm=FMAX(anorm,(fabs(w.get1(i,1))+fabs(rv1.get1(1,i))));
	}
	for (i=n;i>=1;i--)	// Accumulation of right-hand transfromations.
	{
		if (i < n)
		{
			if (g)
			{
				for (j=l;j<=n;j++)
				{				// double division to avoid possible underflow.
					v.put1(j,i, (aa.get1(i,j) / aa.get1(i,l))/g);
				}
				for (j=l;j<=n;j++)
				{
					for (s=(VECTYPE)0.0,k=l;k<=n;k++)
					{
						s += aa.get1(i,k) * v.get1(k,j);
					}
					for (k=l;k<=n;k++)
					{
						// v[k][j] += s*v[k][i];
						v.put1(k,j, v.get1(k,j) + s*v.get1(k,i));
					}
				}
			}
			for (j=l;j<=n;j++)
			{
				// v[i][j]=v[j][i]=(VECTYPE)0.0;
				v.put1(i,j,(VECTYPE)0.0);
				v.put1(j,i,(VECTYPE)0.0);
			}
		}
		v.put1(i,i,(VECTYPE)1.0);
		g=rv1.get1(1,i);
		l=i;
	}
	for (i=IMIN(m,n);i>=1;i--) // Accumulation of left-hand transformations
	{
		l=i+1;
		g=w.get1(i,1);
		for (j=l;j<=n;j++)
		{
			aa.put1(i,j, (VECTYPE)0.0);
		}
		if (g)
		{
			g=(VECTYPE)1.0 / g;
			for (j=l;j<=n;j++)
			{
				for (s=(VECTYPE)0.0,k=l;k<=m;k++)
				{
					s += aa.get1(k,i)*aa.get1(k,j);
				}
				f=(s/aa.get1(i,i))*g;
				for (k=i;k<=m;k++)
				{
					// a[k][j] += f*a[k][i];
					aa.put1(k,j, aa.get1(k,j) + f*aa.get1(k,i));
				}
			}
			for (j=i;j<=m;j++)
			{
				// a[j][i] *= g;
				aa.put1(j,i, aa.get1(j,i) *g);
			}
		}
		else
		{
			for (j=i;j<=m;j++)
			{
				aa.put1(j,i,(VECTYPE)0.0);
			}
		}
		// ++a[i][i];
		aa.addTo1(i,i,1);
	}
	for (k=n;k>=1;k--)		// Diagonalization of the bidiagonal form;
							// loop over singular values, and over alloweed iterations.
	{
		for (its=1;its<=30;its++)
		{
			flag=1;
			for (l=k;l>=1;l--) //  Test for splitting.
			{
				nm=l-1;			// Note that rv1[1][1] is always zero.
				if ((float)(fabs(rv1.get1(1,l))+anorm) == anorm)
				{
					flag=0;
					break;
				}
				if ((float)(fabs( w.get1(nm,1) )+anorm) == anorm)break;
			}
			if (flag)
			{
				c=(VECTYPE)0.0;			// cancellation of rv1[1][1], if l>1.
				s=(VECTYPE)1.0;
				for (i=l;i<=k;i++)
				{
					f=s*rv1.get1(1,i);
					rv1.put1(1,i,c*rv1.get1(1,i));
					if ((float)(fabs(f)+anorm) == anorm)break;
					g=w.get1(i,1);
					h=pythag(f,g);
					w.put1(i,1,h);
					h=(VECTYPE)1.0 / h;
					c=g*h;
					s = -f*h;
					for (j=1;j<=m;j++)
					{
						y=aa.get1(j,nm);
						z=aa.get1(j,i);
						aa.put1(j,nm, y*c+z*s);
						aa.put1(j, i, z*c-y*s);
					}
				}
			}
			z=w.get1(k,1);
			if (l == k)						// Convergence.
			{
				if (z < (VECTYPE)0.0)		// Singular Value is made nonnegative.
				{
					w.put1(k,1,-z);
					for (j=1;j<=n;j++)
					{
						v.put1(j,k, -v.get1(j,k));
					}
				}
				break;
			}
			//if (its == 30) nrerror("no convergence in 30 svdcmp iterations");
#if(DIAGNOSTICS==TRUE)
			ASSERT(its<30);					// ?!?! SVD did not converge!)
#endif
			x=w.get1(l,1);					// Shift from bottom 2-by-2 minor.
			nm=k-1;
			y=w.get1(nm,1);
			g=rv1.get1(1,nm);
			h=rv1.get1(1,k);
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g=pythag(f, (VECTYPE)1.0);
			f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			c=s= (VECTYPE)1.0;						// Next QR transformation:
			for (j=l;j<=nm;j++)
			{
				i=j+1;
				g=rv1.get1(1,i);
				y=w.get1(i,1);
				h=s*g;
				g=c*g;
				z=pythag(f,h);
				rv1.put1(1,j,z);
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=1;jj<=n;jj++)
				{
					x=v.get1(jj,j);
					z=v.get1(jj,i);
					v.put1(jj,j, x*c+z*s);
					v.put1(jj,i, z*c-x*s);
				}
				z=pythag(f,h);
				w.put1(j,1,z);				// Rotation can be arbitrary if z=0.
				if (z)
				{
					z=(VECTYPE)1.0 / z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (jj=1;jj<=m;jj++)
				{
					y=aa.get1(jj,j);
					z=aa.get1(jj,i);
					aa.put1(jj, j, y*c+z*s);
					aa.put1(jj, i, z*c-y*s);
				}
			}
			rv1.put1(1,l,(VECTYPE)0.0);
			rv1.put1(1,k,f);
			w.put1(k,1,x);
		}
	}
//	free_vector(rv1,1,n);
//}
#undef SIGN					// limit scope of the NRecipes macro.
#undef FMAX
#undef IMIN
//-----------End Numerical Recipes (modified)--------------------------------
// Now 'a' matrix holds 'u' matrix values (and 'this' still holds input values)
	u.copy(aa);				// copy U result to the right place, and...
	ss.copy(w);				// copy 'w' matrix to ss, resizing as needed.
	return(TRUE);			// Success!  we're done!
}

BOOL Matrx::SVD_fullSort(Matrx &u, Matrx &ss, Matrx &v)
//------------------------------------------------------------------------------
// SAME as SVD_full except outputs U,S,V are re-ordered so that S elements are
// in greatest-to-smallest order. Careful! the 'v' matrix is V, not V^T !!
{
BOOL isOK;
int i,imax,j, iBig;

	isOK = SVD_full(u, ss, v);			// do the regular SVD.
	if(isOK==FALSE) return(FALSE);		// error! do nothing.
	imax = ss.getXsize();
#if(PESKYWARNINGS==TRUE)
	ASSERT(imax>ss.getYsize());			// !?!? SS is NOT a column vector?!?!
#endif
	// do a cheezy exchange sort (qsort is such a pain...).
	for(i=0; i<imax; i++)				// for each element in ss column vector,
	{
		iBig = i;						// init: 'element i of ss is largest'.
		for(j=i; j<imax; j++)			// look for any larger element beyond i:
		{
			if(ss.get0(j,0) > ss.get0(iBig,0))	// if j-th element is larger,
				iBig = j;				// then remember it.
		}
		ss.swapRows0(i,iBig);			// swap rows of ss column vector,
		u.swapCols0(i,iBig);			// swap output basis vector columns
		v.swapCols0(i,iBig);			// swap input basis vector columns.
	}
	return(TRUE);
}



void Matrx::randFill(VECTYPE min, VECTYPE max)
//------------------------------------------------------------------------------
// fill matrix with random #s.
{
int r,c;

	if(min<max)
	{
		for(r=0; r<ysize; r++)
		{									// for each row,
			for(c=0; c<xsize; c++)
			{
				put0(r,c,
				min + (max-min)*(VECTYPE)(rand()/(float)RAND_MAX));
			}
		}
	}
	else
	{
		for(r=0; r<ysize; r++)
		{									// for each row,
			for(c=0; c<xsize; c++)
			{
				put0(r,c,(VECTYPE)rand());
			}
		}
	}
}

void Matrx::zero(void)
//------------------------------------------------------------------------------
// set all pixels to 0.0
{
int r,c;

	for(r=0; r<ysize; r++)
	{									// for each row,
		for(c=0; c<xsize; c++)
		{
			put0(r,c,(VECTYPE)0.0);
		}
	}
}

void Matrx::countR(void)
//------------------------------------------------------------------------------
// fill with row-major-order counting
// (1 2 3)
// (4 5 6)
// (7 8 9);
{
int i,r,c;

	i=1;
	for(r=0; r<ysize; r++)
	{									// for each row,
		for(c=0; c<xsize; c++)
		{
			put0(r,c,(VECTYPE)i);
			i++;
		}
	}
}

void Matrx::countC(void)
//------------------------------------------------------------------------------
// fill with column-major-order counting
// (1 4	7)
// (2 5	8)
// (3 6	9)
{
int i,r,c;

	i=1;
	for(c=0; c<xsize; c++)
	{										// for each column,
		for(r=0; r<ysize; r++)
		{
			put0(r,c,(VECTYPE)i);
			i++;
		}
	}
}

BOOL Matrx::isEqual(Matrx &src,VECTYPE tol2)
//------------------------------------------------------------------------------
// Element-by-element, compare 'this' to 'src' matrix. Return TRUE iff all
// elements match (or differ by less than sqrt(tol2)
// AND matrices are the same size.
{
int i,j,imax,jmax;
VECTYPE diff;

	if(getXsize()!=src.getXsize() || getYsize()!=src.getYsize())
	{
		return(FALSE);						// mismatched size!
	}
	imax = getXsize();
	jmax = getYsize();
	for(i=0; i<imax; i++)
	{
		for(j=0; j<jmax; j++)
		{
			diff = src.get0(j,i) - get0(j,i);
			if(diff*diff > tol2) return(FALSE);
		}
	}
	return(TRUE);
}

void Matrx::copyTo(Mat4& out)
//------------------------------------------------------------------------------
// copy first 4x4 elements to 'out'.  Complain if there's a size mismatch
{
int i,j;

#if(DIAGNOSTICS==TRUE)
	ASSERT(getXsize()>=4 && getYsize()>=4);
#endif
	for(j=0; j<4; j++)
	{
		for(i=0; i<4; i++)
		{
			out.colrow[i][j] = get0(j,i);
		}
	}
}

void Matrx::textOut(FILE* fptr, char* title=NULL, int linewidth=80)
//------------------------------------------------------------------------------
// Pretty-print the contents of this matrix to the stream 'fptr' (usually
// a file).
{
int r,c;
char* msg;								// text message buffer
char nu[40];							// string holding 1 element.
int line;

#if(DIAGNOSTICS==TRUE)
	if(0==xsize || 0==ysize)
	{
		DPF("Matrx::textOut() given zero-sized matrix!");
		return;
	}
#endif

	if(title!=NULL)
	{									// print a title,
		fprintf(fptr,
		"\n\n------------------%s--------------------------\n",title);
	}
	else
	{
		fprintf(fptr,
		"\n\n----------------------------------------------\n");
	}
	msg = new char[linewidth+1];		// make a line buffer.
	line=0;								// # of chars already printed
										// on this line.
	msg[0] = '\0';						// empty the line buffer.

	for(r=0; r<ysize; r++)
	{									// for each row of matrix,
		for(c=0; c<xsize; c++)
		{								// & each element in the row,
			sprintf(nu,"%f ",get0(r,c));// make an ASCII string;
			line += strlen(nu); 		// see if it fits this line;
			if(line < linewidth)
			{							// YES! append the entry
				strcat(msg,nu);			// to our growing 'msg' buffer
										// and 'tab' to next position
				while(line%8!=0 && line<linewidth)
				{
					strcat(msg," ");
					line++;
				}
			}
			else
			{							// NO! print what we've got,
				fprintf(fptr,"%s\n     ",msg);	// go to next line,
				line=5;					// and indent by 5 spaces,
				msg[0] = '\0';			// clear the msg buffer,
				strcat(msg,nu);			// and add our new ASCII string
			}
		}
		fprintf(fptr,"%s",msg);			// end-of-row. print msg buffer.
		line=0;		  					// & clear it for the next line.
		msg[0] = '\0';
	}
	fprintf(fptr,"\n\n");				// double-space at end.
	delete msg;							// free line buffer memory.
}

void Matrx::prnt(const char *name)
//Revised May 3,2001 J Tumblin
//------------------------------------------------------------------------------
// Pretty-print the contents of this matrix using 'DPF' macro defined at
// top of file (usually VC++/MFC's 'TRACE' macro).  'Linewidth' is max. allowed
// width of text line.
{
#define JT_WIDE		120					// # of chars allowed on one line.
int r,c;								// row, column.
char msg[JT_WIDE+1];					// text message buffer
char digits[JT_WIDE+1];					// string holding 1 numerical element.
char nu[JT_WIDE+1];						// Any req'd leading spaces+'digits[]
int line;								// width of current text line.
int col;								// width of current column
#if(DIAGNOSTICS==TRUE)
	if(0==xsize || 0==ysize) {
		DPF("Matrx::prnt() given zero-sized matrix!");
		return;
		}
#endif
#define JT_TABSTOP	8					// column width in printouts.

	// !!!DON'T SPLIT THE NEXT LINE!!!! DPF is a MACRO!!!
	DPF("\n-----------%s: %d rows, %d cols----------\n",name,getYsize(),getXsize());
	for(r=0; r<ysize; r++)				// for each row of matrix,
	{
		msg[0] = '\0';					// empty the line buffer.
		line = 0;						// init.# of chars printed on this line.
		col = 0;						// init. # of chars in current column;
		for(c=0; c<xsize; c++)			// & each element in the row,
		{								// Make orderly printed columns:-------
			sprintf(digits,"%f ",get0(r,c));// Make a string of digits,
			nu[0] = '\0';				// (empty the column-holding buffer)
			col = strlen(digits);		// init column width to # of digits,
			if(get0(r,c)>=0.0 && col<(JT_WIDE-1))
										// for non-negative#s, add a leading
			{							// space to the column buffer:
				strcat(nu," ");
				col++;					//(that's one more char for this column)
			}
			if(c!=0)					// For all but the 1st matrix column,
			{							// add leading spaces to make all column
				while(col%JT_TABSTOP!=0 && col<(JT_WIDE-1))
				{						// widths an integer multiple of TABSTOP
					strcat(nu," ");
					col++;
				}
			}
			strcat(nu,digits);			// NOW put digits after leading spaces
										// to complete the column of text.
			if(line+col < (JT_WIDE-1))	// Will this new column fit on the line?
			{
				strcat(msg,nu);			// YES! Put column on this line.
				line += col;
			}
			else						// NO! print what we've got,
			{
				DPF("%s\n     ",msg);// indent the next line by 5 spaces,
				line=5;					// shorten text on it by 5 chars,
				msg[0] = '\0';			// clear the line buffer, and
										// continue this long row of the
										// matrix; for 1st continued column,
				if(get0(r,c)>=(VECTYPE)0.0)	strcat(msg," ");
										// add a leading space for non-neg. #s,
				strcat(msg,digits);		// and then the digits of the element.
			}
		}
		DPF("%s\n",msg);			// matrix end-of-row; print msg buffer.
		line=0;		  					// & clear it for the next line.
		msg[0] = '\0';
	}
//	DPF("\n\n");					// double-space at end.
#undef JT_TABSTOP
#undef JT_WIDE
}


//=========================== Ivec Member fcns  ================================
//===========================------------------=================================

Ivec::Ivec(void)
//------------------------------------------------------------------------------
// Constructor for an 'empty' integer vector object.
{
    a = NULL;					// make an empty array.
    size = 0;
}


Ivec::~Ivec(void)
//------------------------------------------------------------------------------
// destructor; de-alloc memory before we destroy the object.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(a!=NULL);
#endif
		wipe();
}


BOOL Ivec::wipe(void)
//------------------------------------------------------------------------------
// release space for vector; make our size zero. If already zero sized,
// return FALSE, else return TRUE.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(a!=NULL);
#endif
    if(a==NULL)
	{								// array is already empty;
		size = 0;					// be sure other members are also benign.
		return(FALSE);
	}
    delete[] a;					// release all pixel memory.
	a = NULL;						// (for VC++)
    size = 0;
    return(TRUE);
}

void Ivec::sizer(int isize)
//------------------------------------------------------------------------------
// allocate space for vector.
{
    if(a != NULL)
	{								// do we have ANY entries at all?
									// YES. correct # of entries?
		if(size != isize)
		{
			wipe();					// NO discard all existing entries;
		}
		else
		{
			return;					// YES. no resizing needed!
		}
	}
    a = new int[isize];				//make array of size 'xsize*ysize'
#if(DIAGNOSTICS==TRUE)
	ASSERT(a!=NULL);				// sizer unable to allocate memory!
#endif
    size = isize;
}

void Ivec::sizer(Ivec& src)
//------------------------------------------------------------------------------
// Allocate memory for the same number of elements found in 'src'.
{
	ASSERT(src.a!=NULL && src.size!=0);
								// Ivec::sizer called with empty src object!!

    if(a != NULL)
	{								// do we have ANY pixels at all?
		if(size != src.size)
		{							// do we have the correct # of elements?
			wipe();					// NO-- so discard all & start over:
		}
		else
		{							// yes-- no resizing needed!
			return;
		}
	}
    a = new int[src.size];			// make a pixel array
	ASSERT(a!=NULL);				// ?!? couldn't alloc array!
	size = src.size;
}

BOOL Ivec::copy(Ivec& src)
//------------------------------------------------------------------------------
// auto-resizing entry-by-entry copy (no change to our object's name).
// If size of 'src' is different from us, 'wipe' our current contents,
//  and change our size to match 'src' before copy.
// If a 'wipe' was necessary, return 'FALSE', else return 'TRUE'.
{
BOOL out=TRUE;

    if(size != src.size)
	{								// src size != us, so wipe
		wipe();
		sizer(src.size);
		out=FALSE;
	}
	memcpy(a, src.a, size * sizeof(int));
    return(out);
}

void Ivec::put0(int indx, int val)
//------------------------------------------------------------------------------
// write 'val' to spec'd position using zero-based indexing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>=0 && indx<size);
#endif
		a[indx] = val;
}

void Ivec::put1(int indx, int val)
//------------------------------------------------------------------------------
// read 'val' from spec'd position using 1-based indexing,
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>0 && indx<=size);
#endif
		a[indx-1] = val;
}

int Ivec::get0(int indx)
//------------------------------------------------------------------------------
// read 'val' from spec'd zero-based index.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>=0 && indx<size);
#endif
		return(a[indx]);
}

int Ivec::get1(int indx)
//------------------------------------------------------------------------------
// read 'val' from spec'd one-based index.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>0 && indx<=size);
#endif
		return(a[indx-1]);
}

BOOL Ivec::add(Ivec& src)
//------------------------------------------------------------------------------
// element-by-element add; us += src;
{
int i;

	ASSERT(size==src.size);		// be sure sizes match, then
	for(i=0; i<size; i++)
	{
		a[i] += src.a[i];
	}
	return(TRUE);
}

BOOL Ivec::sub(Ivec& src)
//------------------------------------------------------------------------------
// element-by-element sub; us -= src;
{
int i;

	ASSERT(size==src.size);		// be sure sizes match, then
	for(i=0; i<size; i++)
	{
		a[i] -= src.a[i];
	}
	return(TRUE);
}

void Ivec::incr0(int indx)
//------------------------------------------------------------------------------
// increment element 'indx' using zero-based addressing;
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>=0 && indx<size);
#endif
		a[indx]++;
}

void Ivec::incr1(int indx)
//------------------------------------------------------------------------------
// increment element 'indx' using one-based addressing;
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>0 && indx<=size);
#endif
		a[indx-1]++;
}

void Ivec::decr0(int indx)
//------------------------------------------------------------------------------
// decrement element 'indx' using zero-based addressing;
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>=0 && indx<size);
#endif
		a[indx]--;
}

void Ivec::decr1(int indx)
//------------------------------------------------------------------------------
// decrement element 'indx' using one-based addressing;
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(indx>0 && indx<=size);
#endif
		a[indx-1]--;
}

void Ivec::zero(void)
//------------------------------------------------------------------------------
// set all elements to 0
{
int i;

	for(i=0; i<size; i++)
	{
		a[i] -= 0;
	}
}

void Ivec::count(void)
//------------------------------------------------------------------------------
// set all elements to 0
{
int i;

	for(i=0; i<size; i++)
	{
		a[i] = i;
	}
}

//=========================== Imat Member fcns =================================
//===========================------------------=================================


Imat::Imat(void)
//------------------------------------------------------------------------------
// Constructor for an 'empty' matrix object.
{
	a = NULL;					// make an empty array for pixels.
	xsize = 0;
	ysize = 0;
}


Imat::~Imat(void)
//------------------------------------------------------------------------------
// destructor; de-alloc memory before we destroy the object.
{
	if(a != NULL)
	{
    	delete[] a;		// release memory, if any
    	a = NULL;					// (for VC++)
    }
#if (PESKYWARNINGS==TRUE)
    else
	{
		DPF("\n!!empty matrix called destructor!!\n");
	}
#endif
    xsize = 0;						// set size to zero.
	ysize = 0;
}


BOOL Imat::wipe(void)
//------------------------------------------------------------------------------
// release space for matrix; make our size zero. If already zero sized,
// return FALSE, else return TRUE.
{
    if(a==NULL)
	{								// array is already empty;
#if(PESKYWARNINGS==TRUE)
    	DPF("\n!!Imat::wipe called on empty object!!\n");
#endif
		xsize = 0;					// be sure its other members are
									// also benign.
		ysize = 0;
		return(FALSE);
	}
    delete[] a;		    	    // release all pixel memory.
	a = NULL;						// (for VC++)
    xsize = 0;
	ysize = 0;
    return(TRUE);
}

void Imat::sizer(int iysize,int ixsize)
//------------------------------------------------------------------------------
// allocate space for matrix.
{
    if(a != NULL)
	{								// do we have ANY entries at all?
#if(PESKYWARNINGS==TRUE)
    	DPF("\n!!Imat::sizer called on nonempty object!!\n");
#endif								// YES. correct # of entries?
		if(xsize != ixsize || ysize != iysize)
		{
			wipe();					// NO discard all existing entries;
		}
		else return;					// YES. no resizing needed!
	}
    a = new int[ixsize*iysize];	//make array of size 'xsize*ysize'
#if(DIAGNOSTICS==TRUE)
    if(a==NULL)
	{
		DPF("\n Imat::sizer() unable to allocate memory!\n");
		exit(1);
	}
#endif
    xsize = ixsize;
	ysize = iysize;
}

void Imat::sizer(Imat& src)
//------------------------------------------------------------------------------
// Allocate memory for the same number of elements found in 'src'.
{
	if(src.a==NULL || src.xsize==0 || src.ysize==0)
	{
		DPF("\n!!Imat::sizer called with empty src object!\n");
		exit(1);
	}
    if(a != NULL)
	{								// do we have ANY pixels at all?
#if(PESKYWARNINGS==TRUE)			// YES;  warn user.
    	DPF("\n!!Imat::sizer called on nonempty object!!\n");
#endif
		if(xsize != src.xsize || ysize != src.ysize)
		{
									// do we have the correct # of pix?
			wipe();					// NO-- so discard all & start over:
		}
		else
		{							// yes-- no resizing needed!
			return;
		}
	}
    a = new int[src.xsize * src.ysize];	// make a pixel array/
#if(DIAGNOSTICS==TRUE)
    if(a==NULL)
	{
		DPF("\n Imat::sizer() unable to allocate memory!\n");
		exit(1);
	}
#endif
    xsize = src.xsize;
	ysize = src.ysize;

}

BOOL Imat::wipeCopy(Imat& src)
//------------------------------------------------------------------------------
// auto-resizing entry-by-entry copy (no change to our object's name).
// If size of 'src' is different from us, 'wipe' our current contents,
//  and change our size to match 'src' before copy.
// If a 'wipe' was necessary, return 'FALSE', else return 'TRUE'.
{
BOOL out=TRUE;

    if(xsize != src.xsize || ysize != src.ysize)
	{												// src size != us, so wipe
		wipe();
		sizer(src.ysize,src.xsize);	// CAREFUL! sizer args are rowsize,colsize!!
		out=FALSE;
	}
	memcpy(a, src.a, xsize * ysize * sizeof(int));
    return(out);
}

void Imat::put0(int row,int col, int val)
//------------------------------------------------------------------------------
// write 'val' from spec'd row, col. using zero-based indexing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
		a[row + col*ysize] = val;
}

void Imat::put1(int row, int col, int val)
//------------------------------------------------------------------------------
// read 'val' from spec'd row, col. Use 1-based addressing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
		a[(row-1) + (col-1)*ysize] = val;
}

void Imat::put0_Row(int row, int c0,...)
//------------------------------------------------------------------------------
// put0_Row(rr,c0,c1,c2,c3) writes a new row of coefficients on row 'rr'.  The
// 0-th element (leftmost) is c0, followed by c1,c2,c3, etc.
// CAREFUL! DOES NOT CHECK FOR TOO MANY/TOO FEW ARGUMENTS!!
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
#endif

va_list vl;									// holds variable argument list ...
int j;

	put0(row,0,c0);							// write the fixed argument, and
	va_start(vl,c0);						// (start var. arg. list process)
	for(j=1; j<getXsize(); j++)				// an entire row of
	{
		put0(row,j,va_arg(vl,int));			// unspecified arguments.
	}
	va_end(vl);								// finished variable arg. process.
}

void Imat::put0_Col(int col, int r0,...)
//------------------------------------------------------------------------------
// put0_Col(cc,r0,r1,c2,r3) writes a new column of coefficients on column 'cc'.
// The 0-th element (uppermost) is r0, followed by r1,r2,r3, etc.
// CAREFUL!  DOES NOT CHECK FOR TOO MANY/TOO FEW ARGUMENTS!!
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(col>=0 && col<xsize);
#endif

va_list vl;									// holds variable argument list...
int i;

	put0(0,col,r0);							// write the fixed argument, and
	va_start(vl,r0);						// (start var. arg. list process)
	for(i=1; i<getYsize(); i++)
	{
		put0(i,col,va_arg(vl,int));
	}
	va_end(vl);								// finished var. arg. list process.
}

void Imat::put0_OR(int row,int col, int val)
//------------------------------------------------------------------------------
// arithmetic-OR 'val' with contents of spec'd row, col.
// Use zero-based indexing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
		a[row + col*ysize] |= val;
}

void Imat::put1_OR(int row, int col, int val)
//------------------------------------------------------------------------------
// arithmetic-OR 'val' with contents spec'd row, col.
// Use one-based addressing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
		a[(row-1) + (col-1)*ysize] |= val;
}

void Imat::put0_AND(int row,int col, int val)
//------------------------------------------------------------------------------
// arithmetic-AND 'val' with contents of spec'd row, col.
// Use zero-based indexing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
		a[row + col*ysize] &= val;
}

void Imat::put1_AND(int row, int col, int val)
//------------------------------------------------------------------------------
// arithmetic-AND 'val' with contents spec'd row, col.
// Use one-based addressing.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
		a[(row-1) + (col-1)*ysize] &= val;
}

void Imat::put0_mask(int row, int col, int val, int mask)
//------------------------------------------------------------------------------
// Write 'val' bits, but only where corresponding 'mask' bits are true.
// Use zero-based addressing.
{
int tmp;

#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
		tmp = a[row + col*ysize];			// read stored value,
		tmp = tmp &~mask;					// zero out the masked bits
		tmp |= val & mask;					// set the nonzero masked 'val' bits
		a[row + col*ysize] = tmp;			// write back the result.
}

void Imat::put1_mask(int row, int col, int val, int mask)
//------------------------------------------------------------------------------
// Write 'val' bits, but only where corresponding 'mask' bits are true.
// Use one-based addressing.
{
int tmp;

#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
	tmp = a[(row-1) + (col-1)*ysize];		// read stored value,
	tmp = tmp&~mask;						// force masked bits to zero,
	tmp |= val & mask;						// set the nonzero masked 'val' bits
	a[(row-1) + (col-1)*ysize] = tmp;		// write back the result.
}

int Imat::get0(int row, int col)
//------------------------------------------------------------------------------
// read 'val' from spec'd row, col.
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>=0 && row<ysize);
	ASSERT(col>=0 && col<xsize);
#endif
	return(a[row + col*ysize]);
}

int Imat::get1(int row, int col)
//------------------------------------------------------------------------------
// read 'val' from spec'd row, col. using 1-based indexing
{
#if(PESKYWARNINGS==TRUE)
	ASSERT(row>0 && row<=ysize);
	ASSERT(col>0 && col<=xsize);
#endif
	return(a[(row-1) + (col-1)*ysize]);
}

void Imat::swapRows1(int rA, int rB)
//------------------------------------------------------------------------------
// Exchange all elements in row A with all elements in row B, using
// 1-based indexing for rA,rB.
{
int tmp;
int i;

	for(i=1; i<=xsize; i++)
	{
		tmp = get1(rA,i);
		put1(rA,i,get1(rB,i));
		put1(rB,i,tmp);
	}
}
void Imat::swapCols1(int cA, int cB)
//------------------------------------------------------------------------------
// Exchange all elements in col A with all elements in column B, using
// 1-based indexing for cA,cB.
{
int tmp;
int j;

	for(j=1; j<=ysize; j++)
	{
		tmp = get1(j,cA);
		put1(j,cA,get1(j,cB));
		put1(j,cB,tmp);
	}
}

BOOL Imat::mmpy(Imat& left, Imat& rt)
//------------------------------------------------------------------------------
// CAREFUL! this is integer-only multiply--don't you want Matrx class instead?)
// replace our current contents with result of matrix multiply  [left][rt].
// Return -1 on err.  Uses a 'temp' matrix to store intermediate results, so
// a matrix object can use ITSELF as an argument to mmpy.
// EXAMPLE;  Imat us,them;		us.mmpy(us,them)l
// this is SLOWER but more flexible.
{
Imat* ptmp;
int r,c,k,rmax,cmax,kmax;
int val;
BOOL useTmp;						// true if output held in temp. storage

#if(DIAGNOSTICS==TRUE)
	if(left.getXsize() != rt.getYsize())
	{
		DPF("Imat::mmpy() size mismatch!!");
		return(FALSE);
	}
#endif
	cmax = rt.getXsize();			// #columns in  output== #columns in rt,
	rmax = left.getYsize();			// #rows in output==#rows in left.
	kmax = rt.getYsize();			// # elements in a col. of rt & row of left
	ptmp = this;					//  point to ourselves, the output matrix;
	useTmp = FALSE;					//  and assume we'll use it.
	if(&left==this || &rt==this)	// if output overwrites an input,
	{
		ptmp = new Imat;			// make a temporary output matrix;
		useTmp=TRUE;				// and set flag so we copy it to output.
	}
	if(ptmp->getXsize()!=cmax || ptmp->getYsize()!=rmax)
		ptmp->sizer(rmax,cmax);		// (resize as needed)
	for(c=0; c<cmax; c++)
	{
		for(r=0; r<rmax; r++)
		{
			val = 0;				// accumulate a row/col multiply
			for(k=0; k<kmax; k++)
			{
				val += left.get0(r,k) * rt.get0(k,c);
			}
			ptmp->put0(r,c,val);
		}
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		wipeCopy(*ptmp); 					// replace 'this' with temp.
		ptmp->wipe();						// and discard temp. version.
		delete ptmp;
	}
	return(TRUE);
}

BOOL Imat::augment(Imat& left, Imat& rt)
//------------------------------------------------------------------------------
// Combine two matrices side-by-side, resizing 'this' as needed, placing columns
// of 'left' adjacent to the columns of 'right'. Uses a 'temp' matrix to store
// intermediate results, a matrix object can use ITSELF as an argument.
// EXAMPLE;  Imat us,them;	us.augment(us,them);  us.augment(them,us);// is OK.
{
Imat* ptmp;
int r,c,rmax,cmaxL, cmaxR;
BOOL useTmp;						// true if output held in temp. storage

	ASSERT(left.getYsize()==rt.getYsize()); // ?!?! column size mismatch!?!?!
	rmax = rt.getYsize();			// #rows in output==#rows in Right matrix.
	cmaxL = left.getXsize();		// #columns in left matrx,
	cmaxR = rt.getXsize();			// #columns in Right matrx,
	ptmp = this;					//  point to ourselves, the output matrix;
	useTmp = FALSE;					//  and assume we'll use it.
	if(&left==this || &rt==this)	// if output overwrites an input,
	{
		ptmp = new Imat;			// make a temporary output matrix;
		useTmp=TRUE;				// and set flag so we copy it to output.
	}
	if((ptmp->getYsize()!=rmax) || (ptmp->getXsize()!=(cmaxL+cmaxR)) )
		ptmp->sizer(rmax,cmaxL+cmaxR);		// (resize as needed)
	for(r=0; r<rmax; r++)
	{
		for(c=0; c<cmaxL; c++)
		{
			ptmp->put0(r,c,left.get0(r,c));
		}
		for(c=0; c<cmaxR; c++)
		{
			ptmp->put0(r,c+cmaxL,rt.get0(r,c));
		}
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		wipeCopy(*ptmp);					// replace 'this' with temp.,
		ptmp->wipe();						// and discard temp. version.
		delete ptmp;
	}
	return(TRUE);
}

BOOL Imat::stack(Imat& top, Imat& bot)
//------------------------------------------------------------------------------
// Combine two matrices top-tobottom, resizing 'this' as needed, placing rows
// of 'top' above rows of 'bot'. Uses a 'temp' matrix to store intermediate
// results, a matrix object can use ITSELF as an argument.
// EXAMPLE;  Imat us,them;	us.stack(us,them);  us.stack(them,us);// is OK.
{
Imat* ptmp;
int r,c,rmaxT,rmaxB,cmax;
BOOL useTmp;						// true if output held in temp. storage.

	ASSERT(top.getXsize()==bot.getXsize());	// ?!?! row size mismatch?!?!
	rmaxT = top.getYsize();					// #rows in top matrix,
	rmaxB = bot.getYsize();					// #rows in bottom matrix,
	cmax = top.getXsize();					// #cols in output==#cols in top
	ptmp = this;					// point to ourselves, the output matrix;
	useTmp = TRUE;					// and assume we'll use it.
	if(&top==this || &bot==this)	// if output overwrites an input,
	{
		ptmp = new Imat;			// make a temporary output matrix,
		useTmp = TRUE;				// and set flag so we copy it to output.
	}
	if((ptmp->getYsize()!=(rmaxT+rmaxB)) || (ptmp->getXsize()!=cmax))
		ptmp->sizer(rmaxT+rmaxB,cmax);		// (resize as needed)
	for(c=0; c<cmax; c++)
	{
		for(r=0; r<rmaxT; r++)
		{
			ptmp->put0(r,c,top.get0(r,c));
		}
		for(r=0; r<rmaxB; r++)
		{
			ptmp->put0(r+rmaxT,c,bot.get0(r,c));
		}
	}
	if(useTmp==TRUE)				// if answer is in temporary storage,
	{
		wipeCopy(*ptmp);					// replace 'this' with temp.,
		ptmp->wipe();						// and discard temp version.
		delete ptmp;
	}
	return(TRUE);
}

void Imat::zero(void)
//------------------------------------------------------------------------------
// set all matrix elements to zero.
{
int r,c;

	for(r=0; r<ysize; r++)
	{								// for each row,
		for(c=0; c<xsize; c++)
		{
			put0(r,c,0);
		}
	}
}

void Imat::countR(void)
//------------------------------------------------------------------------------
// fill with row-major-order counting
// (1 2 3)
// (4 5 6)
// (7 8 9);
{
int i,r,c;

	i=1;
	for(r=0; r<ysize; r++)
	{								// for each row,
		for(c=0; c<xsize; c++)
		{
			put0(r,c,i);
			i++;
		}
	}
}

void Imat::countC(void)
//------------------------------------------------------------------------------
// fill with column-major-order counting
// (1 4	7)
// (2 5	8)
// (3 6	9)
{
int i,r,c;

	i=1;
	for(c=0; c<xsize; c++)
	{								// for each column,
		for(r=0; r<ysize; r++)
		{
			put0(r,c,i);
			i++;
		}
	}
}

void Imat::textOut(FILE* fptr, char* title=NULL, int linewidth=80)
//------------------------------------------------------------------------------
// Pretty-print the contents of this matrix to the stream 'fptr' (usually
// a file).
{
int r,c;
char* msg;								// text message buffer
char nu[40];							// string holding 1 element.
int line;

#if(DIAGNOSTICS==TRUE)
	if(0==xsize || 0==ysize)
	{
		DPF("Imat::textOut() given zero-sized matrix!");
		return;
	}
#endif

	if(title!=NULL)
	{									// print a title,
		fprintf(fptr,
		"\n\n------------------%s--------------------------\n",title);
	}
	else
	{
		fprintf(fptr,
		"\n\n----------------------------------------------\n");
	}
	msg = new char[linewidth+1];		// make a line buffer.
	line=0;								// # of chars already printed
										// on this line.
	msg[0] = '\0';						// empty the line buffer.

	for(r=0; r<ysize; r++)
	{									// for each row of matrix,
		for(c=0; c<xsize; c++)
		{								// & each element in the row,
			sprintf(nu,"%d ",get0(r,c));// make an ASCII string;
			line += strlen(nu); 		// see if it fits this line;
			if(line < linewidth)
			{							// YES! append the entry
				strcat(msg,nu);			// to our growing 'msg' buffer
										// and 'tab' to next position
				while(line%8!=0 && line<linewidth)
				{
					strcat(msg," ");
					line++;
				}
			}
			else
			{							// NO! print what we've got,
				fprintf(fptr,"%s\n     ",msg);	// go to next line,
				line=5;					// and indent by 5 spaces,
				msg[0] = '\0';			// clear the msg buffer,
				strcat(msg,nu);			// and add our new ASCII string
			}
		}
		fprintf(fptr,"%s",msg);			// end-of-row. print msg buffer.
		line=0;		  					// & clear it for the next line.
		msg[0] = '\0';
	}
	fprintf(fptr,"\n\n");				// double-space at end.
	delete msg;							// free line buffer memory.
}

void Imat::prnt(char *name)
//------------------------------------------------------------------------------
// Print to debug window in MFC using  'DPF' #defined at top of file:
{
int r,c;
char* msg;								// text message buffer
char nu[40];							// string holding 1 element.
int line;
int linewidth = 81;

#if(DIAGNOSTICS==TRUE)
	if(0==xsize || 0==ysize)
	{
		DPF("Imat::textOut() given zero-sized matrix!");
		return;
	}
#endif

	DPF("\n---------------%s: %d rows x %d columns--------------------\n",
		name,ysize,xsize);
	msg = new char[linewidth];			// make a line buffer.
	line=0;								// # of chars already printed
										// on this line.
	msg[0] = '\0';						// empty the line buffer.

	for(r=0; r<ysize; r++)
	{									// for each row of matrix,
		for(c=0; c<xsize; c++)
		{								// & each element in the row,
			sprintf(nu,"%d ",get0(r,c));// make an ASCII string;
			line += strlen(nu); 		// see if it fits this line;
			if(line < linewidth)
			{							// YES! append the entry
				strcat(msg,nu);			// to our growing 'msg' buffer
										// and 'tab' to next position
				while(line%8!=0 && line<linewidth-1)
				{
					strcat(msg," ");
					line++;
				}
			}
			else
			{							// NO! print what we've got,
				DPF("%s\n     ",msg);// go to next line,
				line=5;					// and indent by 5 spaces,
				msg[0] = '\0';			// clear the msg buffer,
				strcat(msg,nu);			// and add our new ASCII string
			}
		}
		DPF("%s\n",msg);			// end-of-row. print msg buffer.
		line=0;		  					// & clear it for the next line.
		msg[0] = '\0';
	}
//	DPF("\n\n");					// double-space at end.
	delete msg;							// free line buffer memory.
}
#undef JT_EPSILON
#undef PESKYWARNINGS
#undef DIAGNOSTICS


