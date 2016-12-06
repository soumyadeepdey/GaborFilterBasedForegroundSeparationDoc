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
#include "encoding.h"
//#include "yenc.h"
#include "base64.h"


Encoding::Encoding() {

	encoding_type = "base64";
}

char *Encoding::encode(char *inData, int dataSize) {
//int size;
char *out;
string out_string;
	
	//out = (char *) malloc(sizeof(char) * 2 * dataSize + 2);
	//size = sqlite_encode_binary((unsigned char *) inData, dataSize, (unsigned char *) out);

	//out = encode64(inData, dataSize, 800000000);
	out_string = base64_encode((const unsigned char *) inData, dataSize);

	out = (char *) malloc((1 + (int) out_string.size()) * sizeof(char) );
	memset(out, 0, (1 + (int) out_string.size()) * sizeof(char));
	memcpy(out, out_string.c_str(), out_string.size());

	return out;
}



char *Encoding::decode(char *inData, int &dataSize) {
char *out;
string out_string;

	//out = (char *) malloc(sizeof(char) * 2 * (int) strlen(inData));
	//dataSize = sqlite_decode_binary((unsigned char *) inData, (unsigned char *) out);

	out_string = base64_decode(string(inData));

	dataSize = (int) out_string.size();

	out = (char *) malloc((1 + dataSize) * sizeof(char));
	memset(out, 0, (1 + dataSize) * sizeof(char));
	memcpy(out, out_string.c_str(), dataSize);

	return out;
}