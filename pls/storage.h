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
#ifndef _STORAGE_H_
#define _STORAGE_H_
#include "headers.h"
#include "maths.h"
#include "pls.h"
#include "model.h"



#define MAXSTRSTORE 3000

class Storage {
CvFileStorage *fs;

	// initialize writing to storage
	void InitilizeStoragetoWrite(string filename);

	// initialize reading from storage
	void InitilizeStoragetoRead(string filename);

	// write data (either matrix or vector)
	void WriteData(int cols, int rows, float *data, int NumBytes, string name);

	// read data (either matrix of vector), return number of bytes read
	int ReadData(char *data, CvFileNode *parentNode);

	// write feature specs
	void WriteFeatSpecs(map<string, vector<string> > &specs);

	// read feature specs
	map<string, vector<string> > ReadFeatSpecs();

public:

	Storage();

	// write a matrix to cvStorage
	void WriteMatrix(Matrix<float> *m, string name) { WriteData(m->GetNCols(), m->GetNRows(), m->GetData(), m->GetNumBytes(), name); }

	// write a vector to cvStorage
	void WriteVector(Vector<float> *v, string name) { WriteData(v->GetNElements(), 1, v->GetData(), v->GetNumBytes(), name); }

	// load a matrix from cvStorage
	Matrix<float> *ReadMatrix(string name, CvFileNode *parentNode);

	// load a vector from cvStorage
	Vector<float> *ReadVector(string name, CvFileNode *parentNode);

	// write a model to cvStorage
	void WriteModel(string filename, Model *model);

	// load a model from cvStorage
	void ReadModel(Model *model, string filename);
};


#endif
