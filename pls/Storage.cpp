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
#include "base64.h"
#include "encoding.h"
#include "pls.h"
#include "storage.h"


Storage::Storage() {
	fs = NULL;
}


void Storage::InitilizeStoragetoWrite(string filename) {

	CV_FUNCNAME( "Storage::InitilizeStoratoWrite"); 
	fs = cvOpenFileStorage( filename.c_str(), 0, CV_STORAGE_WRITE);
	if (fs == NULL) 
		DET_ERROR("couldn't open storage file");
}

void Storage::InitilizeStoragetoRead(string filename) {

	CV_FUNCNAME( "Storage::InitilizeStoragetoRead"); 
	fs = cvOpenFileStorage( filename.c_str(), 0, CV_STORAGE_READ);
	if (fs == NULL) 
		DET_ERROR("couldn't open storage file");
}

int Storage::ReadData(char *data, CvFileNode *parentNode) {
CvFileNode *fsnode;
CvSeq* seq;
CvSeqReader reader;
int i, idx, dataSize, total;
Encoding encode;
char *str, *str2;

	fsnode = cvGetFileNodeByName(fs, parentNode, "data");
	seq = fsnode->data.seq;
	total = seq->total;
	cvStartReadSeq(seq, &reader, 0 );

	idx = 0;
	for (i = 0; i < total; i++) {
		CvFileNode* pt = (CvFileNode*)reader.ptr;
		str = (char *) cvReadString(pt, NULL);
		str2 = encode.decode(str, dataSize);
		memcpy(data + idx, str2, dataSize);
		idx += dataSize;
		free(str2);
		CV_NEXT_SEQ_ELEM( seq->elem_size, reader );
	}

	return idx;
}

Matrix<float> *Storage::ReadMatrix(string name, CvFileNode *parentNode) {
CvFileNode *fsnode;
Matrix<float> *m;
int r, c;
char *MData;
int nbytes;
int retBytes;

	// error hadle
	CV_FUNCNAME( "Storage::ReadMatrix"); 

	fsnode = cvGetFileNodeByName(fs, parentNode, name.c_str());
	r = cvReadIntByName(fs, fsnode, "rows");
	c = cvReadIntByName(fs, fsnode, "cols");
	m = new Matrix<float>(r,c);
	MData = (char *) m->GetData();
	nbytes = m->GetNumBytes();

	retBytes = ReadData(MData, fsnode);

	if (nbytes != retBytes) 
		DET_ERROR("number of bytes read inconsistent with matrix size");

	return m;
}




Vector<float> *Storage::ReadVector(string name, CvFileNode *parentNode) {
CvFileNode *fsnode;
Vector<float> *v;
int r, c;
char *MData;
int nbytes;
int retBytes;

	// error hadle
	CV_FUNCNAME( "Storage::ReadVector"); 

	fsnode = cvGetFileNodeByName(fs, parentNode, name.c_str());
	r = cvReadIntByName(fs, fsnode, "rows");
	c = cvReadIntByName(fs, fsnode, "cols");
	if (r != 1 && c != 1) 
		DET_ERROR("not a vector!");

	v = new Vector<float>(max(r,c));
	MData = (char *) v->GetData();
	nbytes = v->GetNumBytes();

	retBytes = ReadData(MData, fsnode);

	if (nbytes != retBytes) 
		DET_ERROR("number of bytes read inconsistent with vector size");

	return v;
}





void Storage::WriteData(int cols, int rows, float *data, int NumBytes, string name) {
char *str, str2[MAXSTRSTORE + 1];
int strSize, i;
Encoding encode;

	cvStartWriteStruct(fs, name.c_str(), CV_NODE_MAP);
	cvWriteInt(fs, "rows", rows);
	cvWriteInt(fs, "cols", cols);
	str = encode.encode((char *) data, NumBytes);

	// create sequence to store string
	strSize = (int) strlen(str);
	cvStartWriteStruct(fs, "data", CV_NODE_SEQ);
	for (i = 0; i < strSize; i += MAXSTRSTORE) {
		memset(str2, 0, MAXSTRSTORE + 1);
		memcpy(str2, str + i, min(MAXSTRSTORE, strSize - i));
		cvWriteString(fs, NULL, str2, 1);
	}
	cvEndWriteStruct(fs);
	cvEndWriteStruct(fs);
}











void Storage::WriteModel(string filename, Model *model) {
map<string, map<string, string> >::iterator it;
map<string, string>::iterator it2;
map<string, map<string, string> > featParams;
map<string, string> params;
Encoding encode;


	InitilizeStoragetoWrite(filename);
	
	// write model variables
	cvWriteInt(fs, "nfactors", model->nfactors);
	cvWriteInt(fs, "nfeatures", model->nfeatures);
	cvWriteString(fs, "encoding", encode.GetEncodingType().c_str(), 1);

	
	WriteVector(model->GetbVector(), "b");
	WriteMatrix(model->GetPMatrix(), "P");
	WriteMatrix(model->GetTMatrix(), "T");
	WriteMatrix(model->GetWMatrix(), "W");
	WriteMatrix(model->GetWstar(), "WStar");
	WriteVector(model->GetMeanVector(), "XMean");
	WriteVector(model->GetStdVector(), "XStd");
	WriteVector(model->GetYVector(), "Y");

	// release structure
	cvReleaseFileStorage(&fs);
}



void Storage::ReadModel(Model *model, std::string filename) {
Matrix<float> *W = NULL;
Matrix<float> *P	 = NULL;
Matrix<float> *Wstar = NULL;
Matrix<float> *T = NULL;
Matrix<float> *Y = NULL;
Vector<float> *Xmean = NULL;
Vector<float> *Xstd = NULL;
Vector<float> *b = NULL;
map<string, map<string, string> > featParams;
string tmpstring;
vector<string> liststring;


	CV_FUNCNAME( "Storage::ReadModel"); 
	InitilizeStoragetoRead(filename);

	// read model variables
	model->nfactors = cvReadIntByName(fs, NULL, "nfactors", -1);
	model->nfeatures = cvReadIntByName(fs, NULL, "nfeatures", -1);


 	// inialize variable of model
	if (model->nfeatures < 0)
		DET_ERROR("improper size of 'nrawfeatures' (<0)");
	
	// read pls variables
	b = ReadVector("b", NULL);
	P = ReadMatrix("P", NULL);
	W = ReadMatrix("W", NULL);
	T = ReadMatrix("T", NULL);
	Y = ReadMatrix("Y", NULL);
	Wstar = ReadMatrix("WStar", NULL);
	Xmean = ReadVector("XMean", NULL);
	Xstd = ReadVector("XStd", NULL);
	model->SetMatrices(W, Wstar, P, Xmean, Xstd, b);


	cvReleaseFileStorage(&fs);
}