/*
 * The implementation of the NIPALS algorithm provided in this library
 * is a translation from the MATLAB version of the NIPALS algorithm 
 * written by Dr. Herve Abdi from The University of Texas at Dallas
 * http://www.utdallas.edu/~herve.
 */
#include "headers.h"
#include "maths.h"
#include "pls.h"
#include <math.h>





void PLS::normaliz(Vector<float> *vector, Vector<float> *retvector) {
float sqr = 0;
float  sqrr; 
int i;

	for (i = 0; i < vector->GetNElements(); i++) {
		sqr += (vector->GetElement(i) * vector->GetElement(i));
		//printf("%d\t%f\n",i,sqr);
	}	
	
	sqrt(sqrr);

	sqrr = sqrt(sqr);
	//printf("sq rt %f\n",sqrr);
	
	for (i = 0; i < vector->GetNElements(); i++) 
	{
		float tval = vector->GetElement(i) /sqrr;
		//printf("%d\t%f\n",i,tval);
		retvector->SetElement(i, tval);
	}
}




// multiply transposed of MrxMc matrix M by vr vector v, result is Mc vector
void PLS::MultiplyTransposeMatrixbyVector(Matrix<float> *M, Vector<float> *v, Vector<float> *retvector) {
float *ptM, *ptv, *ptret;
int i, j;

	ptv = v->GetData();
	ptret = retvector->GetData();
	for (i = 0; i < M->GetNCols(); i++) {
		ptM = M->GetColumn(i);
		ptret[i] = 0.0;
		for (j = 0; j < M->GetNRows(); j++) 
		{
			
			ptret[i] += ptM[j] * ptv[j];
		}
	}
}





// multiply MrxMc matrix M by vr vector v, result is Mr vector
void PLS::MultiplyMatrixbyVector(Matrix<float> *M, Vector<float> *v, Vector<float> *retvector) {
float *ptM, *ptv, *ptret;
int i, j;

	ptv = v->GetData();
	retvector->ResetVector();
	ptret = retvector->GetData();
	for (i = 0; i < M->GetNCols(); i++) {
		ptM = M->GetColumn(i);
		for (j = 0; j < M->GetNRows(); j++) {
			ptret[j] += ptM[j] * ptv[i];
		}
	}
}


// Multiply vectors v1' and v2, returns a single number
float PLS::MultiplyVectorTransposedbyVector(Vector<float> *v1, Vector<float> *v2) {
int i;
float ret = 0;

	for (i = 0; i < v1->GetNElements(); i++)  {
		ret += v1->GetElement(i) * v2->GetElement(i);
	}
	
	return ret;
}



// Multiply vector by scalar, returns a vector size rv
void PLS::MultiplyVectorandScalar(Vector<float> *v, float s, Vector<float> *retvector) {
int i;

	for (i = 0; i < v->GetNElements(); i++)  {
		retvector->SetElement(i, v->GetElement(i) * s);
	}
}


// t is rx1 and p is cx1
void PLS::SubtractFromMatrix(Matrix<float> *M, Vector<float> *t, Vector<float> *p) {
int i, j;
float *ptM, *ptt, *ptp;

	ptt = t->GetData();
	ptp = p->GetData();

	// Xres=Xres-t*p';
	for (j = 0; j < M->GetNCols(); j++) {
		ptM = M->GetColumn(j);
		for (i = 0; i < M->GetNRows(); i++) {
			ptM[i] = ptM[i] - (ptt[i] * ptp[j]);
		}
	}
}


void PLS::SubtractFromVector(Vector<float> *v, Vector<float> *t, float c, float bl) {
int i;
	
	// Yres=Yres-(b(l)*(t*c'));
	for (i = 0; i < v->GetNElements(); i++) {
		v->SetElement(i, v->GetElement(i) - (bl * (t->GetElement(i) * c)));
	//	v[i] = v[i] - (bl * (t[i] * c[0]));
	}
}


void PLS::CopyVector(Vector<float> *v, Vector<float> *retvector) {

	memcpy(retvector->GetData(), v->GetData(), v->GetNElements() * sizeof(float));
}




void PLS::mean(Matrix<float> *M, Vector<float> *retvector) {
float *ptM, *ptv;
int i,j;

	
	retvector->ResetVector();
	
	for (j = 0; j < retvector->GetNElements(); j++) 
	{
	  retvector->SetElement(j,0.0);
	}
	
	ptv = retvector->GetData();
	for (j = 0; j < M->GetNCols(); j++) {
		ptM = M->GetColumn(j);
		for (i = 0; i < M->GetNRows(); i++) {
			ptv[j] += ptM[i];
		}
	}

   	for (i = 0; i < M->GetNCols(); i++) {
		ptv[i] = ptv[i] / (float) M->GetNRows();
	}
}



void PLS::mean(Vector<float> *M, Vector<float> *retvector) {
float *ptM, *ptv;
int j;
	
	retvector->ResetVector();
	for (j = 0; j < retvector->GetNElements(); j++) 
	{
	  retvector->SetElement(j,0.0);
	}
	ptv = retvector->GetData();
	ptM = M->GetData();
	for (j = 0; j < M->GetNElements(); j++) {
		ptv[0] += ptM[j];
	}

	ptv[0] /= (float) M->GetNElements();
}




void PLS::std(Matrix<float> *M, Vector<float> *mean, Vector<float> *retvector) {
float *ptM, *ptret, *ptmean;
int i, j;

	retvector->ResetVector();
	ptret = retvector->GetData();
	ptmean = mean->GetData();

	for (j = 0; j < M->GetNCols(); j++) {
		ptM = M->GetColumn(j);
		for (i = 0; i < M->GetNRows(); i++) {
			ptret[j] += (ptM[i] - ptmean[j]) * (ptM[i] - ptmean[j]);
		}
	}

	for (i = 0; i < M->GetNCols(); i++) {
		ptret[i] = ptret[i] / (((float)M->GetNRows())- (float)1.0);
		ptret[i] = sqrt(ptret[i]);
		if (ptret[i] < EPSILON) {
			ptret[i] = 1;
		}
	}
}



void PLS::std(Vector<float> *M, Vector<float> *mean, Vector<float> *retvector) {
float *ptM, *ptret, *ptmean;
int i;

	retvector->ResetVector();
	ptret = retvector->GetData();
	ptmean = mean->GetData();

	ptM = M->GetData();
	for (i = 0; i < M->GetNElements(); i++) {
		ptret[0] += (ptM[i] - ptmean[0]) * (ptM[i] - ptmean[0]);
	}
	
	ptret[0] /= (((float)M->GetNElements())- (float)1.0);
	ptret[0] = sqrt(ptret[0]);

	if (ptret[0] < EPSILON) {
		ptret[0] = 1;
	}
}





void PLS::zscore(Matrix<float> *M, Vector<float> *mean, Vector<float> *std) {
int i,j;
float *ptM, *ptmean, *ptstd;

	ptmean = mean->GetData();
	ptstd = std->GetData();
	for(j = 0; j < M->GetNCols(); j++){	
		ptM = M->GetColumn(j);
		for (i = 0; i < M->GetNRows(); i++) {
			ptM[i] = (ptM[i] - ptmean[j]) / ptstd[j];
		}
	}
}



void PLS::ComputeWstar() {
Matrix<float> *tmp1, *tmp2, *tmp3;

	
	//Wstar=W*inv(P'*W);
	tmp1 = TransposeMatrix(P);
	tmp2 = MultMatrices(tmp1, W);
	tmp3 = InvMatrix(tmp2);
	Wstar = MultMatrices(W, tmp3);

	delete tmp1;
	delete tmp2;
	delete tmp3;
}



void PLS::zscore(Vector<float> *M, Vector<float> *mean, Vector<float> *std) {
int i;
float *ptM, *ptmean, *ptstd;

	ptmean = mean->GetData();
	ptstd = std->GetData();
	ptM = M->GetData();
	for (i = 0; i < M->GetNElements(); i++) {
		ptM[i] = (ptM[i] - ptmean[0]) / ptstd[0];
	}
}




void PLS::runpls(Matrix<float> *X, Vector<float> *Y, int nfactor, char *OutputDir, float ExplainedX, float ExplainedY) {
float  c; // vectors
int i, j;
float b_l, tmpscalar;  // scalars
int kk;
float dt=0;
int maxsteps, step;
int nsamples, nfeatures;
Vector<float> *t, *u, *t0, *Vcol, *Vrow, *w, *p, *C;
Matrix<float> *U;
Vector<float> *ymean, *ystd;
double sumX2, sumY2, percX, percY, percAux, cumsum, cumsumY;
Matrix<float> *tmpM;
Vector<float> *tmpV;
vector<int> selectedCols;

	// initially, clear current PLS model (if there is one)
	ClearPLS();

	nsamples = X->GetNRows();
	nfeatures = X->GetNCols();


	Yorig = new Vector<float> (nsamples);
	CopyVector(Y, Yorig);

	maxsteps = 100;

	Xmean = new Vector<float> (nfeatures);
	Xstd = new Vector<float> (nfeatures);

	mean(X, Xmean);
	std(X, Xmean, Xstd);
	zscore(X, Xmean, Xstd); 
#if 0
	mean = umd_mean(X, rX, cX);
	printf("Finished means\n");
	std = umd_std(X, mean, rX, cX);
	printf("Finished std\n");
	umd_zscore(X, mean, std, rX, cX);
    printf("Finished z-scores\n");
#endif



	//Xmean->Write("Xmean.matrix");
	//Xstd->Write("Xstd.matrix");



    // write std and mean
 //   WriteMatrix(mean, 1, cX, "Xmean.matrix");
 //   WriteMatrix(std, 1, cX, "Xstd.matrix");
        
	// Y
	ymean = new Vector<float> (1);
	ystd = new Vector<float> (1);
	mean(Y, ymean);
	std(Y, ymean, ystd);
	zscore(Y, ymean, ystd);
//	mean = umd_mean(Y, rY, cY);
//	std = umd_std(Y, mean, rY, cY);
//	umd_zscore(Y, mean, std, rY, cY);
    
    // write std and mean
    //WriteMatrix(mean, 1, cY, "Ymean.matrix");
    //WriteMatrix(std, 1, cY, "Ystd.matrix");





	//u = NULL;
	//t0 = NULL;
	//U = umd_allocateMatrix(rX, nfactor);
	//C = umd_allocateVector(nfactor);
	//T = umd_allocateMatrix(rX, nfactor);
	//P = umd_allocateMatrix(cX, nfactor);
	//W = umd_allocateMatrix(cX, nfactor);
	//b = umd_allocateVector(nfactor);
	
	U = new Matrix<float> (nsamples, nfactor);
	C = new Vector<float> (nfactor);
	T = new Matrix<float> (nsamples, nfactor);
	P = new Matrix<float> (nfeatures, nfactor);
	W = new Matrix<float> (nfeatures, nfactor);
	b = new Vector<float> (nfactor);


	t = new Vector<float> (nsamples);
	u = new Vector<float> (nsamples);
	t0 = new Vector<float> (nsamples);
	Vcol = new Vector<float> (nfeatures);
	Vrow = new Vector<float> (nsamples);
	w = new Vector<float> (nfeatures);
	p = new Vector<float> (nfeatures);


	// compute square of the sum of X and Y
	sumY2 = 0;
	sumX2 = 0;
	for (i = 0; i < Y->GetNElements(); i++) {
		sumY2 += (double) (Y->GetElement(i) * Y->GetElement(i));
	}

	for (i = 0; i < X->GetNCols(); i++) {
		for (j = 0; j < X->GetNRows(); j++) {
			sumX2 += (double) (X->GetElement(i, j) * X->GetElement(i, j));
		}
	}


	//printf("\rsumX: %5.3f, sumY: %5.3f", sumX2, sumY2);
	cumsum = 0;
	cumsumY = 0;

	for (i = 0; i < nfactor; i++) {

		normaliz(Y, t);
		CopyVector(t, u);
		
		

		step = 0;
                do {
			//printf("new while loop, iteration %d\n", step);
			//if (t0 != NULL)
			//	free(t0);

        	//t0 = CopyVector(t, rY);
			CopyVector(t, t0);

			// w=normaliz(Xres'*u)
			//tmp = MultiplyTransposeMatrixbyVector(X, rX, cX, u, rY);
        	//w = normaliz(tmp, cX);
			//free(tmp);
			
			
			
			MultiplyTransposeMatrixbyVector(X, u, Vcol);
			
			
			
			normaliz(Vcol, w);
			
			

			// t=normaliz(Xres*w);
			//tmp = MultiplyMatrixbyVector(X, rX, cX, w, cX);
			//free(t);
			//t = normaliz(tmp, rX);
			//free(tmp);
			MultiplyMatrixbyVector(X, w, Vrow);
			
			
			
			


			// c = normaliz(Yres'*t); c is 1xrY
			//tmpscalar = MultiplyVectorTransposedbyVector(Y, rY, t);
			//c = normaliz(&tmpscalar, 1);
			tmpscalar = MultiplyVectorTransposedbyVector(Y, t);
			c = tmpscalar/tmpscalar;  //dummy step, because it normalizes a constant
			
			// u=Yres*c
			//free(u);
			//u = MultiplyVectorandScalar(Y, rY, c[0]);
			MultiplyVectorandScalar(Y, c, u);
			


			dt = 0;
            for (kk = 0; kk < nsamples; kk++) {
				dt += (t0->GetElement(kk) - t->GetElement(kk)) * (t0->GetElement(kk) - t->GetElement(kk));
				//printf("%d\t%f\t%f\n",kk,t0->GetElement(kk),t->GetElement(kk));
			}
			if (cvIsNaN((double)dt)) {
				printf("\nproblem during PLS: NaN\n");
				exit(2);
			}
            //printf("t - t0: %f\n", dt);
                       
 			step++;
		} while (dt>0.000001 && step<maxsteps);
		
		// p=Xres'*t; p is cX by 1
		//p = MultiplyTransposeMatrixbyVector(X, rX, cX, t, rX);
		MultiplyTransposeMatrixbyVector(X, t, p);


		// b_l=((t'*t)^(-1))*(u'*t); // ||t|| = 1
		b_l = MultiplyVectorTransposedbyVector(u, t);

		// store matrices
		//b[i] = b_l;
		b->SetElement(i, b_l);
	
		for (j = 0; j < nfeatures; j++) {
			P->SetValue(i, j, p->GetElement(j));
			W->SetValue(i, j, w->GetElement(j));
		}
		//for (j = 0; j < cX; j++) {
	   // 	WMELEM(P, j, i, p[j], nfactor);
    	//	WMELEM(W, j, i, w[j], nfactor);
		//}

		for (j = 0; j < nsamples; j++) {
			T->SetValue(i, j, t->GetElement(j));
			U->SetValue(i, j, u->GetElement(j));
		}

		//for (j = 0; j < rX; j++) {
		//	WMELEM(T, j, i, t[j], nfactor);
    	//	WMELEM(U, j, i, u[j], nfactor);
		//}

		C->SetElement(i, c);

		//C[i] = c[j];

		
		

	    // deflation of X and Y
		// Xres=Xres-t*p';
		//SubtractFromMatrix(X, rX, cX, t, rX, p, cX);
		SubtractFromMatrix(X, t, p);

		// Yres=Yres-(b(l)*(t*c'));
		//SubtractFromVector(Y, rY, t, rX, c, 1, b[i]);
		SubtractFromVector(Y, t, c, b->GetElement(i));

		//sprintf(str,"Y%.4d.matrix", i+1);
        //        WriteMatrix(Y, rY, 1, str);      


		// get percentage explained in X
		percX = 0;
		percY = 0;
		percAux = 0;
//#if 0
		for (j = 0; j < T->GetNRows(); j++) {
			percAux += (double) (T->GetElement(i, j) * T->GetElement(i, j));
		}
		for (j = 0; j < P->GetNRows(); j++) { 
			percY += (double) (P->GetElement(i, j) * P->GetElement(i, j));
		}
		percX = (percAux * percY) / sumX2;
		percY = (percAux * (double) (b->GetElement(i) * b->GetElement(i))) / sumY2;
		cumsum += percX;
		cumsumY += percY;

		if (cumsum >= ExplainedX) {
			printf("reached percentage explained of X variable, stopping\n");
			nfactor = i + 1;
			break;
		}
		if (cumsum >= ExplainedY) {
			printf("reached percentage explained of Y variable, stopping\n");
			nfactor = i + 1;
			break;
		}
			

//#endif
		// Debug
		//fprintf(stdout,"End of iteration %2.2d (explained X: %5.3f, Y: %5.3f) cumsum: %5.3f cumsumY: %5.3f\n", i, percX, percY, cumsum, cumsumY);
	}

	//printf("\n");

	// saving only number of factors actually used
	for (i = 0; i < nfactor; i++) {
		selectedCols.push_back(i);
	}


	tmpM = T->GetSelectedCols(&selectedCols);
	delete T;
	T = tmpM;

	tmpM = P->GetSelectedCols(&selectedCols);
	delete P;
	P = tmpM;

	tmpM = W->GetSelectedCols(&selectedCols);
	delete W;
	W = tmpM;

	tmpV = b->SelectElements(&selectedCols);
	delete b;
	b = tmpV;



	//T->Write("T.matrix");
	//W->Write("W.matrix");
	//P->Write("P.matrix");
	//b->Write("b.matrix");
	//Yorig->Write("Y.matrix");



	// write T, W, and P
	//WriteMatrix(T, rX, nfactor, "T.matrix");
	//WriteMatrix(W, cX, nfactor, "W.matrix");
	//WriteMatrix(P, cX, nfactor, "P.matrix");

	// compute Wstar
	//	printf("antes 1\n");
	ComputeWstar();
	//Wstar = W->Copy();
	//printf("depois 1\n");


	// create auxiliar vector for zscore
	zdataV = new Vector<float>(Wstar->GetNRows());


	// set max number of factors
	this->maxFactors = this->Wstar->GetNCols();

	// free memory
	delete u;
	delete t0;
	delete Vcol;
	delete Vrow;
	delete w;
	delete p;
	delete C;
	delete U;
	delete ymean;
	delete ystd;
	delete t;
}



 



PLS::PLS() {

	Xmean = NULL;
	Xstd = NULL;
	T = NULL;
	P = NULL;
	W = NULL;
	Yorig = NULL;
	b = NULL;
	Wstar = NULL;
	zdataV = NULL;
	maxFactors = -1;
}







void PLS::SetMatrices(Matrix<float> *W, Matrix<float> *Wstar, Matrix<float> *P, Vector<float> *Xmean, Vector<float> *Xstd, Vector<float> *b) {

	ClearPLS();

	this->W = W;
	this->Wstar = Wstar;
	this->P = P;
	this->Xmean = Xmean;
	this->Xstd = Xstd;
	this->b = b;
	this->maxFactors = Wstar->GetNCols();


	// create auxiliar vector for zscore
	zdataV = new Vector<float>(Wstar->GetNRows());
}



PLS::~PLS() {

	ClearPLS();
}





void PLS::ClearPLS() {

	if (Xmean != NULL) {
		delete Xmean;
		Xmean = NULL;
	}

	if (Xstd != NULL) {
		delete Xstd;
		Xstd = NULL;
	}

	if (P != NULL) {
		delete P;
		P = NULL;
	}

	if (W != NULL) {
		delete W;
		W = NULL;
	}

	if (Yorig != NULL) {
		delete Yorig;
		Yorig = NULL;
	}

	if (T != NULL) {
		delete T;
		T = NULL;
	}

	if (b != NULL) {
		delete b;
		b = NULL;
	}

	if (Wstar != NULL) {
		delete Wstar;
		Wstar = NULL;
	}

	if (zdataV != NULL) {
		delete zdataV;
		zdataV = NULL;
	}

	maxFactors = -1;
}




void PLS::ClearExtraMatrices() {

	if (T != NULL) {
		delete T;
		T = NULL;
	}

	if (P != NULL) {
		delete P;
		P = NULL;
	}

	if (Yorig != NULL) {
		delete Yorig;
		Yorig = NULL;
	}

	if (W != NULL) {
		delete W;
		W = NULL;
	}

	if (b != NULL) {
		delete b;
		b = NULL;
	}
}




void PLS::ClearExtraMatricesPLSReg() {

	if (Yorig != NULL) {
		delete Yorig;
		Yorig = NULL;
	}

	if (b != NULL) {
		delete b;
		b = NULL;
	}

	if (T != NULL) {
		delete T;
		T = NULL;
	}

	if (P != NULL) {
		delete P;
		P = NULL;
	}

	if (W != NULL) {
		delete W;
		W = NULL;
	}

	if (Wstar != NULL) {
		delete Wstar;
		Wstar = NULL;
	}
}




void PLS::Projection(float *feat, float *retproj, int nfactors) {
int i;

	mat.ZscoreSSE(feat, Xmean->GetData(), Xstd->GetData(), zdataV->GetData(), Wstar->GetNRows());


	for (i = 0; i < nfactors; i++) {	
		retproj[i] = mat.DotProductSSENotMultof4(zdataV->GetData(), Wstar->GetColumn(i), Wstar->GetNRows());
	}
}


void PLS::ExecuteZScore(float *feat, float *zscoreResult) {
	

	mat.ZscoreSSE(feat, Xmean->GetData(), Xstd->GetData(), zscoreResult, Wstar->GetNRows());


}






void PLS::InitializePLSModel(Vector<float> *Xmean, Vector<float> *Xstd, Vector<float> *Yorig, Vector<float> *b, Matrix<float> *T, Matrix<float> *P,
						 Matrix<float> *W, Matrix<float> *Wstar) {

	ClearPLS();
								
	if (Xmean != NULL)
		this->Xmean = Xmean->Copy();

	if (Xstd != NULL)
		this->Xstd = Xstd->Copy();

	if (Yorig != NULL) {
		this->Yorig = Yorig->Copy();
	}

	if (b != NULL) {
		this->b = b->Copy();
	}
	
	if (T != NULL) {
		this->T = T->Copy();
	}

	if (P != NULL) {
		this->P = P->Copy();
	}

	if (W != NULL) {
		this->W = W->Copy();
	}

	if (Wstar != NULL) {
		this->Wstar = Wstar->Copy();
		// create aux variable
		zdataV = new Vector<float>(Wstar->GetNRows());
		// set max number of factors
		this->maxFactors = this->Wstar->GetNCols();
	}
}