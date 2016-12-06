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
#ifndef _HEADERS_H_
#define _HEADERS_H_

#include <list>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <stdio.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"

using namespace std;

#define VERSION 0.0.1

#define EPSILON 0.000000001

// error handler
#define DET_ERROR( Msg )                                       \
{                                                                   \
	fprintf(stdout, "%s(%s.%d): \n  %s\n", __FILE__, cvFuncName, __LINE__, Msg);        \
    exit(-1);                                                          \
}



#endif