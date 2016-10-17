#ifndef WriteintoXML_H
#define WriteintoXML_H

#include "StandardHeaders.h"
#include "AlethiaParser/AlethiaParser.h"
#include "SegmentationUnit.h"



void WriteGTXMLFile(page P, char *xmlname, vector<SB> Blocks);

void WriteOutputXMLFile(page P, char *xmlname, vector<SB> Blocks);

#endif