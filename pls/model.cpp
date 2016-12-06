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
#include "headers.h"
#include "maths.h"
#include "storage.h"
#include "model.h"


Model::Model() {

	nfactors = -1;
	nfeatures = -1;
}



Model::Model(string filename) {
Storage storage;

	storage.ReadModel(this, filename);
}



void Model::SaveModel(string filename) {
Storage storage;
	
	storage.WriteModel(filename, this);
}



Vector<float> *Model::ProjectFeatureVector(Vector<float> *feat) {
Vector<float> *ret;	

	ret = new Vector<float>(this->nfactors);
	Projection(feat->GetData(), ret->GetData(), nfactors);

	return ret;
}
	


Matrix<float> *Model::ProjectFeatureMatrix(Matrix<float> *featMat) {
Matrix<float> *ret;	
Vector<float> *featV, *auxv;	
int i;

	auxv = new Vector<float>(this->nfactors);
	featV = new Vector<float>(this->nfactors);
	ret = new Matrix<float>(featMat->GetNRows(), this->nfactors);

	for (i = 0; i < featMat->GetNRows(); i++) {
		featV = featMat->GetRow(i);
		Projection(featV->GetData(), auxv->GetData(), nfactors);
		ret->SetRow(auxv, i);
	}

	delete featV;
	delete auxv;

	return ret;
}



void Model::CreatePLSModel(Matrix<float> *mPos, Matrix<float> *mNeg, int nfactors) {
Matrix<float> *X;
Vector<float> *Y;

	this->nfactors = nfactors;

	CV_FUNCNAME("Model::CreatePLSModel"); 
	// set number of features
	nfeatures = mNeg->GetNCols();

	// inialize variable of model
	if (this->nfeatures < 0)
		DET_ERROR("improper size of 'nfeatures' (<0)");

	X = mPos->ConcatenateMatricesRows(mNeg, mPos);
	
	Y = new Vector<float>(X->GetNRows());
	Y->SetRangeElements(0, mNeg->GetNRows(), (float) -1);
	Y->SetRangeElements(mNeg->GetNRows(), mPos->GetNRows(), (float) 1);

	this->runpls(X, Y, nfactors, NULL);

	// clear used data
	delete X;
	delete Y;
}



void Model::CreatePLSModel(Matrix<float> *X, Vector<float> *Y, int nfactors) {
Matrix<float> *Xnew;
Vector<float> *Ynew;

	// number of factors
	this->nfactors = nfactors;

	// number of features
	this->nfeatures = X->GetNCols();

	// copy matrices
	Xnew = X->Copy();
	Ynew = Y->Copy();

	// run PLS
	this->runpls(Xnew, Ynew, nfactors, NULL);

	// clear used data
	delete Xnew;
	delete Ynew;
}