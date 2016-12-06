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
#ifndef MATHS_H
#define MATHS_H
#include "headers.h"





//////////////////
// class Vector //
//////////////////
template <class T>
class Vector {
	T *realp; // real pointer before align
	T *v;     // data poiter
	int n;        // number of elements

	// allocate memory for the vector
	void AllocateMemory(int n) {
		CV_FUNCNAME("Vector::AllocateMemory");

		realp = (T *) malloc(sizeof(T) * n);

		if (realp == NULL)
			DET_ERROR("No available memory to allocate vector");

		memset(realp, 0, sizeof(T) * n);
		v = realp;
		this->n = n;
		// if change here, check setData function
	}


	void LoadVector(char *filename) {
		FILE *f;
		int c, r;

		f = fopen(filename, "rb");
		fread(&r, sizeof(int), 1, f);
		fread(&c, sizeof(int), 1, f);
		AllocateMemory(max(r,c));
	
		fread(v, sizeof(T), max(r,c), f);

		fclose(f);
	}


public:
	// Initialize
	Vector(int n) { AllocateMemory(n); }

	Vector(bool onlyHeader) {
		realp = NULL;
		v = NULL;
		n = 0;
	}

	Vector(string filename) { LoadVector((char *)filename.c_str()); }

	// load a vector
	Vector(char *filename) { LoadVector(filename); }

	// release structures
	~Vector() {
		free(realp);
	}

	// return number of bytes used for this vector
	int GetNumBytes() { return n * sizeof(T); }


	// retrieve element at row y and column x
	inline T GetElement(int x) { return v[x]; }
	inline T operator[](int x) { return v[x]; }


	// set value
	inline void SetElement(int x, T value) { v[x] = value; }


	// set n elements from p with value 
	void SetRangeElements(int p, int n, T value) {
		int i;
		
		for (i = p; i < n + p; i++)
			SetElement(i, value);
	}

	// retrieve matrix pointer
	inline T *GetData() { return v; }

	// retrive number of elements
	inline int GetNElements() { return n; }

	// reset elements of vector
	inline void ResetVector() { memset(v, 0, sizeof(T) * n);  }

	// select elements from the vector
	inline Vector<T> *SelectElements(vector<int> *selectedElems) {
		Vector<T> *sv;
		int i;

		sv = new Vector<T>((int)selectedElems->size());
		for (i = 0; i < sv->GetNElements(); i++) {
			sv->SetElement(i, v[selectedElems->at(i)]);
		}

		return sv;
	}




	// write vector
	void Write(char *filename) {
		int i;
		FILE *f;
		T value;

		f = fopen(filename, "wb");
		// header
		i = 1;
		fwrite(&i, 1, sizeof(int), f);
		fwrite(&n, 1, sizeof(int), f);

		// data
		for (i = 0; i < n; i++) {
			value = GetElement(i);
			fwrite(&value, 1, sizeof(T), f);
		}

		fclose(f);
	}

	// copy this vector to a new one
	Vector *Copy() { 
		Vector<T> *v1; 
		T *data;
		v1 = new Vector<T>(n);
		data = v1->GetData();
		memcpy(data, this->GetData(), sizeof(T) * n);
		return v1;
	}


	void Write(string filename) { Write((char *) filename.c_str()); }
};








//////////////////
// class Matrix //
//////////////////
template <class T>
class Matrix { // matrix is column based
	T *realp; // real pointer before align
	T *m;     // data pointer
	int r, c;     // number of rows and columns
	int idx;      // index to get elements sequentially

	// allocate memory for the matrix
	inline void AllocateMemory(int r, int c) {

		CV_FUNCNAME("Matrix::AllocateMemory");

		realp = new T[r * c];

		if (realp == NULL)
			DET_ERROR("No available memory to allocate matrix");


	//	realp = (T *) malloc(sizeof(T) * r * c);
		memset(realp, 0, sizeof(T) * (r * c));
		m = realp;	
		this->r = r;
		this->c = c;
		this->idx = 0;
	}

	void LoadMatrix(char *filename) {
		FILE *f;
		int x, y, r, c;
		T var;

		f = fopen(filename, "rb");
		fread(&r, sizeof(int), 1, f);
		fread(&c, sizeof(int), 1, f);
		AllocateMemory(r, c);
	
		// assuming file stores rowise data
		for (y = 0; y < r; y++) {
			for (x = 0; x < c; x++) {
				fread(&var, sizeof(T), 1, f);
				SetValue(x, y, var);
			}
		}

		fclose(f);
	}


public:

	// clear structures
	~Matrix() { 
		delete this->realp; 
		this->realp = NULL;
		this->m = NULL;
		this->r = -1;
		this->c = -1;
	}


	// Initialize
	Matrix(int r, int c) { AllocateMemory(r, c); }

	Matrix(string filename) { LoadMatrix((char *) filename.c_str()); }

	// load a matrix
	Matrix(char *filename) { LoadMatrix(filename); }
	
	// return number of bytes used for this matrix
	int GetNumBytes() { return r * c * sizeof(T); }

	// retrieve element at row y and column x
	inline T GetElement(int x, int y) { return m[x * r + y]; }

	// set value
	inline void SetValue(int x, int y, T value) { m[x * r + y] = value; }

	// retrieve matrix pointer
	inline T *GetData() { return m; }

	// retrieve pointer for a column (column starts on 0)
	inline T *GetColumn(int i) { return m + r*i; }

	// get number of rows
	inline int GetNRows() { return r; }

	// get number of columns
	inline int GetNCols() { return c; }


	Matrix<T> *GetSelectedCols(vector<int> *selectedcols) {
		Matrix<T> *m;
		int i, j, k;

		m = new Matrix<T>(this->GetNRows(), (int) selectedcols->size());
		for (i = 0; i < (int)selectedcols->size(); i++) {
			j = selectedcols->at(i);
			for (k = 0; k < this->GetNRows(); k++) {
				m->SetValue(i, k, this->GetElement(j, k));
			}
		}

		return m;
	}


	// contatenate rows of matrices, first m1 then m2
	Matrix<T> *ConcatenateMatricesRows(Matrix<T> *m1, Matrix<T> *m2) {
		Matrix<T> *m;
		int x, y, idxrow;

			// only copy the second matrix
			if (m1 == NULL) {
				m = m2->Copy();
				return m;
			}

			// test compatibility
			if (m1->GetNCols() != m2->GetNCols()) {
				printf("Incompatible number of columns!\n");
				exit(2);
			}

			m = new Matrix<T> (m1->GetNRows() + m2->GetNRows(), m1->GetNCols());

			idxrow = 0;
			// m1
			for (y = 0; y < m1->GetNRows(); y++) {
				for (x = 0; x < m1->GetNCols(); x++) {
					m->SetValue(x, idxrow, m1->GetElement(x, y));
				}
				idxrow++;
			}
			// m2
			for (y = 0; y < m2->GetNRows(); y++) {
				for (x = 0; x < m2->GetNCols(); x++) {
					m->SetValue(x, idxrow, m2->GetElement(x, y));
				}
				idxrow++;
			}
			
			return m;
	}


	// write to a file - write row-wise
	void Write(char *filename) {
		int i, j;
		FILE *f;
		float value;

		f = fopen(filename, "wb");
		if (f == NULL)  {
			printf("Couldn't open '%s' for writing\n", filename);
			exit(2);
		}

		// header
		fwrite(&r, 1, sizeof(int), f);
		fwrite(&c, 1, sizeof(int), f);

		// data
		for (i = 0; i < r; i++) {
			for (j = 0; j < c; j++) {
				value = (float)GetElement(j, i);
				fwrite(&value, 1, sizeof(float), f);
			}
		}

		fclose(f);
	}


	void Write(string filename) { Write((char *) filename.c_str()); }

	// copy this matrix to a new one
	Matrix *Copy() { 
		Matrix *m1;
		T *data;
		m1 = new Matrix<T>(r, c);
		data = m1->GetData();
		memcpy(data, this->GetData(), sizeof(T) * r * c);
		return m1;
	}


	// retrieve a row of the matrix in a new vector
	inline Vector<T> *GetRow(int i) { 
		Vector<T> *v;
		int j;

		v = new Vector<T>(this->c);
		for (j = 0; j < this->c; j++) {
			v->SetElement(j, this->GetElement(j, i));
		}

		return v;
	}

	void SetRow(Vector<T> *InVector, int r) {
		int i;
		// check sizes
		if (InVector->GetNElements() != this->GetNCols()) {
			printf("error diff n columns\n" );
			exit(2);
		}
		
		for (i = 0; i < InVector->GetNElements(); i++) {
			this->SetValue(i, r, InVector->GetElement(i));
		}
	}

};







class Maths {

public:
	// dot product when n is not multiple of 4
	float DotProductSSENotMultof4(float *v, float *v2, int n);

	void ZscoreSSE(float *data, float *mean, float *std, float *outputvect, int n);
};







///////////////////////
// OpenCV functions //
//////////////////////
// convert matrix format to openCV format - put data used in opencv format
void ConvertMatrixFormat(Matrix<float> *m1, CvMat *M1, float **data);

// multiply two matrices
Matrix<float> *MultMatrices(Matrix<float> *m1, Matrix<float> *m2);

// invert matrix
Matrix<float> *InvMatrix(Matrix<float> *m);

// transpose matrix
Matrix<float> *TransposeMatrix(Matrix<float> *m);



#endif
