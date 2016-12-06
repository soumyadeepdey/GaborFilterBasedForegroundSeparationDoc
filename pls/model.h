/* 
   http://www.liv.ic.unicamp.br/~wschwartz/softwares.html

   Copyright (C) 2010-2011 William R. Schwartz

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   William R. Schwartz williamrobschwartz [at] gmail.com
*/
#ifndef _MODEL_H_
#define _MODEL_H_
#include "headers.h"
#include "maths.h"
#include "pls.h"


class Model : public PLS {

protected:
	int nfactors;							// number of factors outputed for this model
	int nfeatures;							// number of features input in this model

	// friend classes
	friend class Storage;

public:
	Model();

	// load a stored PLS model
	Model(string filename);

	// build PLS model when response variable has only two values (for classification)
	void CreatePLSModel(Matrix<float> *mPos, Matrix<float> *mNeg, int nfactors);

	// build PLS model for any response variable
	void CreatePLSModel(Matrix<float> *X, Vector<float> *Y, int nfactors);

	// save model to a file
	void SaveModel(string filename);

	// project feature vector using this model
	Vector<float> *ProjectFeatureVector(Vector<float> *feat);
	
	// project feature matrix
	Matrix<float> *ProjectFeatureMatrix(Matrix<float> *featMat);

	// get number of features
	int GetNumberFeatures() { return nfeatures; } 
};




#endif
