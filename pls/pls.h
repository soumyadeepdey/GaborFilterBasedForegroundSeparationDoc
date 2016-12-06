/*
 * The implementation of the NIPALS algorithm provided in this library
 * is a translation from the MATLAB version of the NIPALS algorithm 
 * written by Dr. Herve Abdi from The University of Texas at Dallas
 * http://www.utdallas.edu/~herve.
 */

#ifndef PLS_H
#define PLS_H
#include "headers.h"
#include "maths.h"

class PLS {
Vector<float> *Xmean, *Xstd, *Yorig, *b;
Matrix<float> *T, *P, *W, *Wstar;
int maxFactors;		// maximum number of factors for this model

// other variables
Maths mat;
Vector<float> *zdataV;		// variable to hold result of zscore


	void normaliz(Vector<float> *vector, Vector<float> *retvector);
	void MultiplyTransposeMatrixbyVector(Matrix<float> *M, Vector<float> *v, Vector<float> *retvector);
	void MultiplyMatrixbyVector(Matrix<float> *M, Vector<float> *v, Vector<float> *retvector);
	float MultiplyVectorTransposedbyVector(Vector<float> *v1, Vector<float> *v2);
	void MultiplyVectorandScalar(Vector<float> *v, float s, Vector<float> *retvector);
	void SubtractFromMatrix(Matrix<float> *M, Vector<float> *t, Vector<float> *p);
	void SubtractFromVector(Vector<float> *v, Vector<float> *t, float c, float bl);
	void CopyVector(Vector<float> *v, Vector<float> *retvector);
	void mean(Matrix<float> *M, Vector<float> *retvector) ;
	void mean(Vector<float> *M, Vector<float> *retvector) ;
	void std(Matrix<float> *M, Vector<float> *mean, Vector<float> *retvector);
	void std(Vector<float> *M, Vector<float> *mean, Vector<float> *retvector);
	void zscore(Matrix<float> *M, Vector<float> *mean, Vector<float> *std);
	void zscore(Vector<float> *M, Vector<float> *mean, Vector<float> *std);
	void ComputeWstar();

	// clear data not used for PLS regression (leave only Bstar, Ymean, Xstd, Xmean)
	void ClearExtraMatricesPLSReg();

	friend class Storage;



protected:

	// set all matrices initializing with already computed values 
	// Warning: this function COPY all variables
	void InitializePLSModel(Vector<float> *Xmean, Vector<float> *Xstd, Vector<float> *Yorig, Vector<float> *b, Matrix<float> *T, 
			Matrix<float> *P, Matrix<float> *W, Matrix<float> *Wstar);

	// remove matrices not used for projections
	void ClearExtraMatrices();

	// return projection considering n factors
	void Projection(float *feat, float *retproj, int nfactors);

	// execute PLS for maximum number of factors: nfactor
	void runpls(Matrix<float> *X, Vector<float> *Y, int nfactor, char *OutputDir = NULL, float ExplainedX = 1, float ExplainedY = 1);

	// return feature vector running zcore
	void ExecuteZScore(float *feat, float *zscoreResult);

	// set matrices for PLS
	void SetMatrices(Matrix<float> *W, Matrix<float> *Wstar, Matrix<float> *P, Vector<float> *Xmean, Vector<float> *Xstd, Vector<float> *b);


	// clear variables of this class
	void ClearPLS();


	// friend classes
	//friend class Storage;
	friend class Structures;

	// Return matrices and vectors
	Matrix<float> *GetWMatrix() { return W; }
	Matrix<float> *GetTMatrix() { return T; }
	Matrix<float> *GetPMatrix() { return P; }
	Matrix<float> *GetWstar() { return Wstar; }
	Vector<float> *GetbVector() { return b; }
	Vector<float> *GetYVector() { return Yorig; }
	Vector<float> *GetMeanVector() { return Xmean; }
	Vector<float> *GetStdVector() { return Xstd; }
	Vector<float> *GetBstar(int nfactors);

public:
	PLS();
	~PLS();
};


#endif
