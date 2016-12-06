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
#include <xmmintrin.h>
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <emmintrin.h>
#include "maths.h"


float Maths::DotProductSSENotMultof4(float *v, float *v2, int n) {
__m128 a, b, c, d;
float out_sse[4];
int i, n2;

	d = _mm_setzero_ps();
	if ((n & 3) == 0) { // multiple of 4
		for (i = 0; i < n; i += 4) {
			a = _mm_loadu_ps(v + i);
			b = _mm_loadu_ps(v2 + i);
			c = _mm_mul_ps(a, b);
			d = _mm_add_ps(c, d);
		}
		_mm_storeu_ps(out_sse,d);
	}
	else {  // n not multiple of 4
		n2 = n - 4;	
		for (i = 0; i < n2; i += 4) {
			a = _mm_loadu_ps(v + i);
			b = _mm_loadu_ps(v2 + i);
			c = _mm_mul_ps(a, b);
			d = _mm_add_ps(c, d);
		}
		_mm_storeu_ps(out_sse,d);
		n2 = n - (n & 0x3);
		// do the remaining elements
		for (i = n2; i < n; i++) {
			out_sse[0] += v[i] * v2[i];
		}
	}

	return out_sse[0] + out_sse[1] + out_sse[2] + out_sse[3];
}





void Maths::ZscoreSSE(float *data, float *mean, float *std, float *outputvect, int n) {
__m128 a, b, c, d, e;
int i, n2;

	if ((n & 3) == 0) { // multiple of 4
		for (i = 0; i < n; i += 4) {
			a = _mm_loadu_ps(data + i);
			b = _mm_loadu_ps(mean + i);
			c = _mm_loadu_ps(std + i);
			d = _mm_sub_ps(a, b);
			e = _mm_div_ps(d, c);
			_mm_storeu_ps(outputvect + i, e);
		}
	}
	else {  // n not multiple of 4
		n2 = n - 4;	
		for (i = 0; i < n2; i += 4) {
			a = _mm_loadu_ps(data + i);
			b = _mm_loadu_ps(mean + i);
			c = _mm_loadu_ps(std + i);
			d = _mm_sub_ps(a, b);
			e = _mm_div_ps(d, c);
			_mm_storeu_ps(outputvect + i, e);
		}
		// do the remaining elements
		for (i = n2; i < n; i++) {
			outputvect[i] = (data[i] - mean[i]) / std[i];
		}
	}
}

















// convert from my format to OpenCV format
void ConvertMatrixFormat(Matrix<float> *m1, CvMat *M1, float **data) {
float *d1;
int idx, x, y;

	d1 = (float *) malloc(m1->GetNCols() * m1->GetNRows() * sizeof(float));
	*data = d1;

	// copy data to convert to rowwise
	idx = 0;
	for (y = 0; y < m1->GetNRows(); y++) {
		for (x = 0; x < m1->GetNCols(); x++) {
			d1[idx++] = m1->GetElement(x, y);
		}
	}

	cvInitMatHeader( M1, m1->GetNRows(), m1->GetNCols(), CV_32FC1, d1 );
}




// multiplication of two matrices
Matrix<float> *MultMatrices(Matrix<float> *m1, Matrix<float> *m2) {
Matrix<float> *dataRet;
float *d1, *d2, *d3;
CvMat M1, M2, M3;
int x, y, idx;

	// check if multiplication is consistent
	if (m1->GetNCols() != m2->GetNRows()) {
		printf("multiplication of incompatible matrices\n");
		exit(2);
	}

	dataRet = new Matrix<float>(m1->GetNRows(), m2->GetNCols());


	ConvertMatrixFormat(m1, &M1, &d1);
	ConvertMatrixFormat(m2, &M2, &d2);
	ConvertMatrixFormat(dataRet, &M3, &d3);




	cvMatMulAdd( &M1, &M2, 0, &M3 );

	// copy result
	idx = 0;
	for (y = 0; y < dataRet->GetNRows(); y++) {
		for (x = 0; x < dataRet->GetNCols(); x++) {
			dataRet->SetValue(x, y, d3[idx++]);
		}
	}

	free(d1);
	free(d2);
	free(d3);

	return dataRet;
}



// invert a matrix
Matrix<float> *InvMatrix(Matrix<float> *m) {
float *d1, *d2;
CvMat M1, M2;
Matrix<float> *dataRet;
int x, y, idx;
double ret;

	// check dimensions
	if (m->GetNCols() != m->GetNRows()) {
		printf("matrix must be square to invert\n");
		exit(2);
	}	


	dataRet = new Matrix<float>(m->GetNRows(), m->GetNCols());

	ConvertMatrixFormat(m, &M1, &d1);
	ConvertMatrixFormat(dataRet, &M2, &d2);

	ret = cvInvert(&M1, &M2);

	if (ret == 0) {
		printf("error: matrix cannot be inverted\n");
		exit(2);
	}
		

	// copy result
	idx = 0;
	for (y = 0; y < dataRet->GetNRows(); y++) {
		for (x = 0; x < dataRet->GetNCols(); x++) {
			dataRet->SetValue(x, y, d2[idx++]);
		}
	}


	free(d1);
	free(d2);

	return dataRet;
}



// transpose matrix
Matrix<float> *TransposeMatrix(Matrix<float> *m) {
float *d1, *d2;
CvMat M1, M2;
Matrix<float> *dataRet;
int x, y, idx;


	dataRet = new Matrix<float>(m->GetNCols(), m->GetNRows());

	ConvertMatrixFormat(m, &M1, &d1);
	ConvertMatrixFormat(dataRet, &M2, &d2);

	cvTranspose( &M1, &M2 );

	// copy result
	idx = 0;
	for (y = 0; y < dataRet->GetNRows(); y++) {
		for (x = 0; x < dataRet->GetNCols(); x++) {
			dataRet->SetValue(x, y, d2[idx++]);
		}
	}


	free(d1);
	free(d2);

	return dataRet;
}