#include <string>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>



#include "AlethiaParser.h"

using namespace xercesc;
using namespace std;

/**
 *  Constructor initializes xerces-C libraries.
 *  The XML tags and attributes which we seek are defined.
 *  The xerces-C DOM parser infrastructure is initialized.
 */

GetConfig::GetConfig()
{
   try
   {
      XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
   }
   catch( XMLException& e )
   {
      char* message = XMLString::transcode( e.getMessage() );
      cerr << "XML toolkit initialization error: " << message << endl;
      XMLString::release( &message );
      // throw exception here to return ERROR_XERCES_INIT
   }

   // Tags and attributes used in XML file.
   // Can't call transcode till after Xerces Initialize()

      
   TAG_Page = XMLString::transcode("Page");
   ATTR_Pagename = XMLString::transcode("imageFilename");
   ATTR_PagenHeight = XMLString::transcode("imageHeight");
   ATTR_PagenWidth = XMLString::transcode("imageWidth");
   TAG_TextRegion = XMLString::transcode("TextRegion");
   TAG_GraphicRegion = XMLString::transcode("GraphicRegion");
   TAG_ImageRegion = XMLString::transcode("ImageRegion");
   TAG_ChartRegion = XMLString::transcode("ChartRegion");
   TAG_SeparatorRegion = XMLString::transcode("SeparatorRegion"); 
   TAG_Coords = XMLString::transcode("Coords");
   ATTR_id = XMLString::transcode("id");
   ATTR_type = XMLString::transcode("type");
   ATTR_bgColour = XMLString::transcode("bgColour");
   ATTR_orientation = XMLString::transcode("orientation");
   ATTR_readingOrientation = XMLString::transcode("readingOrientation");
   ATTR_readingDirection = XMLString::transcode("readingDirection");
   ATTR_textColour = XMLString::transcode("textColour");
   ATTR_colour = XMLString::transcode("colour");
   ATTR_reverseVideo = XMLString::transcode("reverseVideo");
   ATTR_indented = XMLString::transcode("indented");
   ATTR_primaryLanguage = XMLString::transcode("primaryLanguage");
   ATTR_primaryScript = XMLString::transcode("primaryScript");
   ATTR_embText = XMLString::transcode("embText");
   ATTR_numColors = XMLString::transcode("numColours");
   ATTR_ColorDepth = XMLString::transcode("ColorDepth");
   ATTR_x = XMLString::transcode("x");
   ATTR_y = XMLString::transcode("y");

   m_ConfigFileParser = new XercesDOMParser;
}

/**
 *  Class destructor frees memory used to hold the XML tag and 
 *  attribute definitions. It als terminates use of the xerces-C
 *  framework.
 */

GetConfig::~GetConfig()
{
   // Free memory

   delete m_ConfigFileParser;


   try
   {
 
      XMLString::release( &TAG_Page );
      XMLString::release( &ATTR_Pagename );
      XMLString::release( &ATTR_PagenHeight );
      XMLString::release( &ATTR_PagenWidth );
      XMLString::release( &TAG_TextRegion );
      XMLString::release( &TAG_GraphicRegion );
      XMLString::release( &TAG_ImageRegion );
      XMLString::release( &TAG_ChartRegion );
      XMLString::release( &TAG_SeparatorRegion );
      XMLString::release( &TAG_Coords );
      XMLString::release( &ATTR_id );
      XMLString::release( &ATTR_type );
      XMLString::release( &ATTR_bgColour );
      XMLString::release( &ATTR_orientation );
      XMLString::release( &ATTR_readingOrientation );
      XMLString::release( &ATTR_readingDirection );
      XMLString::release( &ATTR_textColour );
      XMLString::release( &ATTR_colour);
      XMLString::release( &ATTR_reverseVideo );
      XMLString::release( &ATTR_indented );
      XMLString::release( &ATTR_primaryLanguage );
      XMLString::release( &ATTR_primaryScript );
      XMLString::release( &ATTR_embText );
      XMLString::release( &ATTR_numColors );
      XMLString::release( & ATTR_ColorDepth );
      XMLString::release( & ATTR_x );
      XMLString::release( & ATTR_y );
      
   }
   catch( ... )
   {
      cerr << "Unknown exception encountered in TagNamesdtor" << endl;
   }

   // Terminate Xerces

   try
   {
      XMLPlatformUtils::Terminate();  // Terminate after release of memory
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );

      cerr << "XML ttolkit teardown error: " << message << endl;
      XMLString::release( &message );
   }
}

/**
 *  This function:
 *  - Tests the access and availability of the XML configuration file.
 *  - Configures the xerces-c DOM parser.
 *  - Reads and extracts the pertinent information from the XML config file.
 *
 *  @param in configFile The text string name of the HLA configuration file.
 */

page GetConfig::readConfigFile(string& configFile)
        throw( std::runtime_error )
{
   // Test to see if the file is ok.

   struct stat fileStatus;

   errno = 0;
   if(stat(configFile.c_str(), &fileStatus) == -1) // ==0 ok; ==-1 error
   {
       if( errno == ENOENT )      // errno declared by include file errno.h
          throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
       else if( errno == ENOTDIR )
          throw ( std::runtime_error("A component of the path is not a directory."));
       else if( errno == ELOOP )
          throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
       else if( errno == EACCES )
          throw ( std::runtime_error("Permission denied."));
       else if( errno == ENAMETOOLONG )
          throw ( std::runtime_error("File can not be read\n"));
   }

   // Configure DOM parser.

   m_ConfigFileParser->setValidationScheme( XercesDOMParser::Val_Never );
   m_ConfigFileParser->setDoNamespaces( false );
   m_ConfigFileParser->setDoSchema( false );
   m_ConfigFileParser->setLoadExternalDTD( false );

   try
   {
     page P;
     
      m_ConfigFileParser->parse( configFile.c_str() );

      // no need to free this pointer - owned by the parent parser object
      DOMDocument* xmlDoc = m_ConfigFileParser->getDocument();

      // Get the top-level element: NAme is "GEDI". No attributes for "GEDI"
      
      DOMElement* elementRoot = xmlDoc->getDocumentElement();
      if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

      // Parse XML file for tags of interest: "DL_Document"
      // Look one level nested within "GEDI". (child of GEDI)

      DOMNodeList*      children = elementRoot->getChildNodes();
      const  XMLSize_t nodeCount = children->getLength();

      // For all nodes, children of "root" in the XML tree.
      
      printf("number of node = %ld\n",nodeCount);

      
      
      
      for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
      {
         DOMNode* currentNode = children->item(xx);
         if( currentNode->getNodeType() &&  // true is not NULL
             currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
         {
            // Found node which is an Element. Re-cast node as element
            DOMElement* currentElement
                        = dynamic_cast< xercesc::DOMElement* >( currentNode );
            if( XMLString::equals(currentElement->getTagName(), TAG_Page))
            {
	      // Read attributes of element "PAGE".
		    const XMLCh* pagename = currentElement->getAttribute(ATTR_Pagename);
		    char *imagename;
		    imagename = XMLString::transcode(pagename);
		    printf("Image name = %s\n",imagename);
		    
		   P.SetImageName(imagename);
		    
		     const XMLCh* zonerow = currentElement->getAttribute(ATTR_PagenHeight);
			  char *zonerows = XMLString::transcode(zonerow);
			  int row = atoi(zonerows);
			  
			  P.SetRow(row);
			  
			  
			  const XMLCh* zonecol = currentElement->getAttribute(ATTR_PagenWidth);
			  char *zonecols = XMLString::transcode(zonecol);
			  int col = atoi(zonecols);
			  
			  P.SetCol(col);
			
			  
		    
		    DOMNodeList* Pagechildren = currentElement->getChildNodes();
		    const  XMLSize_t PagenodeCount = Pagechildren->getLength();
		    printf("number of node = %ld\n",PagenodeCount);
		    
		    for( XMLSize_t yy = 0; yy < PagenodeCount; ++yy )
		    {
		      DOMNode* pagecurrentNode = Pagechildren->item(yy);
		      if( pagecurrentNode->getNodeType() &&  // true is not NULL
			  pagecurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		      {
			
			DOMElement* pagecurrentElement
                        = dynamic_cast< xercesc::DOMElement* >( pagecurrentNode );
		
			if( XMLString::equals(pagecurrentElement->getTagName(), TAG_TextRegion) )
			{
			  TextRegion T;
			  
			  const XMLCh* xid = pagecurrentElement->getAttribute(ATTR_id);
			  char *tid = XMLString::transcode(xid);
			  T.setid(tid);
			  printf("id=%s\n",tid);
			  
			  const XMLCh* xtype = pagecurrentElement->getAttribute(ATTR_type);
			  char *ttype = XMLString::transcode(xtype);
			  T.settype(ttype);
			  
			  const XMLCh* xbgc = pagecurrentElement->getAttribute(ATTR_bgColour);
			  char *t_bgcolor = XMLString::transcode(xbgc);
			  T.setbgcolor(t_bgcolor);
			  
			  const XMLCh* xori = pagecurrentElement->getAttribute(ATTR_orientation);
			  char *t_orientation = XMLString::transcode(xori);
			  float orien =(float) atof(t_orientation);
			  T.setorientation(orien);
			  
			  
			  DOMNodeList* Nchildren = currentElement->getChildNodes();
			  const  XMLSize_t NnodeCount = Pagechildren->getLength();
			  printf("number of node = %ld\n",NnodeCount);
			  for( XMLSize_t zz = 0; zz < NnodeCount; ++zz )
			  {
			    DOMNode* NcurrentNode = Nchildren->item(zz);
			    if(NcurrentNode->getNodeType() &&  // true is not NULL
				NcurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
			    {
			      DOMElement* NcurrentElement
			      = dynamic_cast< xercesc::DOMElement* >( NcurrentNode );
		      
			      if( XMLString::equals(NcurrentElement->getTagName(), TAG_Coords) )
			      {
				Point Q;
				
				const XMLCh* xcor = NcurrentElement->getAttribute(ATTR_x);
				char *tempx = XMLString::transcode(xcor);
				int xposi = atoi(tempx);
				Q.x=xposi;
				
				const XMLCh* ycor = NcurrentElement->getAttribute(ATTR_y);
				char *tempy = XMLString::transcode(ycor);
				int yposi = atoi(tempy);
				Q.y=yposi;
				
				T.setCoord(Q);
				
			      }
			    }
			  }
			  
			  //Attribute Specific to Text Region will go here
			  
			  
			  
			  
			  const XMLCh* xRori = pagecurrentElement->getAttribute(ATTR_readingOrientation);
			  char *t_Rorientation = XMLString::transcode(xori);
			  float Rorien =(float) atof(t_Rorientation);
			  T.setReadingOrientation(Rorien);
			  
			  const XMLCh* xRDir = pagecurrentElement->getAttribute(ATTR_readingDirection);
			  char *t_RDir = XMLString::transcode(xRDir);
			  T.setreadingDirection(t_RDir);
			  
			  const XMLCh* xtcol = pagecurrentElement->getAttribute(ATTR_textColour);
			  char *t_TCol = XMLString::transcode(xtcol);
			  T.settextColour(t_TCol);
			  
			  const XMLCh* xrv = pagecurrentElement->getAttribute(ATTR_reverseVideo);
			  char *t_RV = XMLString::transcode(xrv);
			  bool RV;
			  if(strcmp(t_RV,"false"))
			    RV = false;
			  else
			    RV = true;
			  T.setreverseVideo(RV);
			  
			  const XMLCh* xintended = pagecurrentElement->getAttribute(ATTR_indented);
			  char *t_intended = XMLString::transcode(xintended);
			  bool b_intended;
			  if(strcmp(t_intended,"false"))
			    b_intended = false;
			  else
			    b_intended = true;
			  T.setindented(b_intended);
			  
			  const XMLCh* xPL = pagecurrentElement->getAttribute(ATTR_primaryLanguage);
			  char *t_PL = XMLString::transcode(xPL);
			  T.setprimaryLanguage(t_PL);
			  
			  const XMLCh* xPS = pagecurrentElement->getAttribute(ATTR_primaryScript);
			  char *t_PS = XMLString::transcode(xPS);
			  T.setprimaryScript(t_PS);
			  
			  
			  P.FillTextRegion(T);
			  
			}
			else if( XMLString::equals(pagecurrentElement->getTagName(), TAG_SeparatorRegion) )
			{
			  SeparatorRegion S;
			  const XMLCh* xid = pagecurrentElement->getAttribute(ATTR_id);
			  char *tid = XMLString::transcode(xid);
			  S.setid(tid);
			  
			  const XMLCh* xori = pagecurrentElement->getAttribute(ATTR_orientation);
			  char *t_orientation = XMLString::transcode(xori);
			  float orien = atof(t_orientation);
			  S.setorientation(orien);
			  
			  DOMNodeList* Nchildren = currentElement->getChildNodes();
			  const  XMLSize_t NnodeCount = Pagechildren->getLength();
			  printf("number of node = %ld\n",NnodeCount);
			  for( XMLSize_t zz = 0; zz < NnodeCount; ++zz )
			  {
			    DOMNode* NcurrentNode = Nchildren->item(zz);
			    if(NcurrentNode->getNodeType() &&  // true is not NULL
				NcurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
			    {
			      DOMElement* NcurrentElement
			      = dynamic_cast< xercesc::DOMElement* >( NcurrentNode );
		      
			      if( XMLString::equals(NcurrentElement->getTagName(), TAG_Coords) )
			      {
				Point Q;
				
				const XMLCh* xcor = NcurrentElement->getAttribute(ATTR_x);
				char *tempx = XMLString::transcode(xcor);
				int xposi = atoi(tempx);
				Q.x=xposi;
				
				const XMLCh* ycor = NcurrentElement->getAttribute(ATTR_y);
				char *tempy = XMLString::transcode(ycor);
				int yposi = atoi(tempy);
				Q.y=yposi;
				
				S.setCoord(Q);
				
			      }
			    }
			  }
			  
			  //Attribute Specific to Separator Region will go here
			  
			
			  
			  const XMLCh* xc = pagecurrentElement->getAttribute(ATTR_colour);
			  char *t_color = XMLString::transcode(xc);
			  S.setcolor(t_color);
			  
			}
			else if( XMLString::equals(pagecurrentElement->getTagName(), TAG_GraphicRegion) )
			{
			  GraphicRegion G;
			  
			  const XMLCh* xid = pagecurrentElement->getAttribute(ATTR_id);
			  char *tid = XMLString::transcode(xid);
			  G.setid(tid);
			  
			  const XMLCh* xtype = pagecurrentElement->getAttribute(ATTR_type);
			  char *ttype = XMLString::transcode(xtype);
			  G.settype(ttype);
			  
			  const XMLCh* xbgc = pagecurrentElement->getAttribute(ATTR_bgColour);
			  char *t_bgcolor = XMLString::transcode(xbgc);
			  G.setbgcolor(t_bgcolor);
			  
			  const XMLCh* xori = pagecurrentElement->getAttribute(ATTR_orientation);
			  char *t_orientation = XMLString::transcode(xori);
			  float orien = atof(t_orientation);
			  G.setorientation(orien);
			  
			  
			  DOMNodeList* Nchildren = currentElement->getChildNodes();
			  const  XMLSize_t NnodeCount = Pagechildren->getLength();
			  printf("number of node = %ld\n",NnodeCount);
			  for( XMLSize_t zz = 0; zz < NnodeCount; ++zz )
			  {
			    DOMNode* NcurrentNode = Nchildren->item(zz);
			    if(NcurrentNode->getNodeType() &&  // true is not NULL
				NcurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
			    {
			      DOMElement* NcurrentElement
			      = dynamic_cast< xercesc::DOMElement* >( NcurrentNode );
		      
			      if( XMLString::equals(NcurrentElement->getTagName(), TAG_Coords) )
			      {
				Point Q;
				
				const XMLCh* xcor = NcurrentElement->getAttribute(ATTR_x);
				char *tempx = XMLString::transcode(xcor);
				int xposi = atoi(tempx);
				Q.x=xposi;
				
				const XMLCh* ycor = NcurrentElement->getAttribute(ATTR_y);
				char *tempy = XMLString::transcode(ycor);
				int yposi = atoi(tempy);
				Q.y=yposi;
				
				G.setCoord(Q);
				
			      }
			    }
			  }
			  
			  //Attribute Specific to Graphic Region will go here
			  
			
			  
			  const XMLCh* xembText = pagecurrentElement->getAttribute(ATTR_embText);
			  char *t_embText = XMLString::transcode(xembText);
			  bool b_embText;
			  if(strcmp(t_embText,"false"))
			    b_embText = false;
			  else
			    b_embText = true;
			  G.setembText(b_embText);
			  
			  const XMLCh* xNC = pagecurrentElement->getAttribute(ATTR_numColors);
			  char *t_NC = XMLString::transcode(xNC);
			  int NC = atoi(t_NC);
			  G.setnumColors(NC);
			  
			  P.FillGraphicRegion(G);
			}
			else if( XMLString::equals(pagecurrentElement->getTagName(), TAG_ImageRegion) )
			{
			  ImageRegion I;
			  
			  const XMLCh* xid = pagecurrentElement->getAttribute(ATTR_id);
			  char *tid = XMLString::transcode(xid);
			  I.setid(tid);
			  
			  const XMLCh* xtype = pagecurrentElement->getAttribute(ATTR_type);
			  char *ttype = XMLString::transcode(xtype);
			  I.settype(ttype);
			  
			  const XMLCh* xbgc = pagecurrentElement->getAttribute(ATTR_bgColour);
			  char *t_bgcolor = XMLString::transcode(xbgc);
			  I.setbgcolor(t_bgcolor);
			  
			  const XMLCh* xori = pagecurrentElement->getAttribute(ATTR_orientation);
			  char *t_orientation = XMLString::transcode(xori);
			  float orien = atof(t_orientation);
			  I.setorientation(orien);
			  
			  
			  DOMNodeList* Nchildren = currentElement->getChildNodes();
			  const  XMLSize_t NnodeCount = Pagechildren->getLength();
			  printf("number of node = %ld\n",NnodeCount);
			  for( XMLSize_t zz = 0; zz < NnodeCount; ++zz )
			  {
			    DOMNode* NcurrentNode = Nchildren->item(zz);
			    if(NcurrentNode->getNodeType() &&  // true is not NULL
				NcurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
			    {
			      DOMElement* NcurrentElement
			      = dynamic_cast< xercesc::DOMElement* >( NcurrentNode );
		      
			      if( XMLString::equals(NcurrentElement->getTagName(), TAG_Coords) )
			      {
				Point Q;
				
				const XMLCh* xcor = NcurrentElement->getAttribute(ATTR_x);
				char *tempx = XMLString::transcode(xcor);
				int xposi = atoi(tempx);
				Q.x=xposi;
				
				const XMLCh* ycor = NcurrentElement->getAttribute(ATTR_y);
				char *tempy = XMLString::transcode(ycor);
				int yposi = atoi(tempy);
				Q.y=yposi;
				
				I.setCoord(Q);
				
			      }
			    }
			  }
			  
			  //Attribute Specific to Graphic Region will go here
			  
			   
			   char* ColorDepth;
			   
			  const XMLCh* xembText = pagecurrentElement->getAttribute(ATTR_embText);
			  char *t_embText = XMLString::transcode(xembText);
			  bool b_embText;
			  if(strcmp(t_embText,"false"))
			    b_embText = false;
			  else
			    b_embText = true;
			  I.setembText(b_embText);
			  
			  const XMLCh* xCD = pagecurrentElement->getAttribute(ATTR_numColors);
			  char *t_CD = XMLString::transcode(xCD);
			  I.setColorDepth(t_CD);
			  
			  P.FillImageRegion(I);
			  
			}
			else if( XMLString::equals(pagecurrentElement->getTagName(), TAG_ChartRegion) )
			{
			  
			  ChartRegion C;
			  
			  const XMLCh* xid = pagecurrentElement->getAttribute(ATTR_id);
			  char *tid = XMLString::transcode(xid);
			  C.setid(tid);
			  
			  const XMLCh* xtype = pagecurrentElement->getAttribute(ATTR_type);
			  char *ttype = XMLString::transcode(xtype);
			  C.settype(ttype);
			  
			  const XMLCh* xbgc = pagecurrentElement->getAttribute(ATTR_bgColour);
			  char *t_bgcolor = XMLString::transcode(xbgc);
			  C.setbgcolor(t_bgcolor);
			  
			  const XMLCh* xori = pagecurrentElement->getAttribute(ATTR_orientation);
			  char *t_orientation = XMLString::transcode(xori);
			  float orien = atof(t_orientation);
			  C.setorientation(orien);
			  
			  
			  DOMNodeList* Nchildren = currentElement->getChildNodes();
			  const  XMLSize_t NnodeCount = Pagechildren->getLength();
			  printf("number of node = %ld\n",NnodeCount);
			  for( XMLSize_t zz = 0; zz < NnodeCount; ++zz )
			  {
			    DOMNode* NcurrentNode = Nchildren->item(zz);
			    if(NcurrentNode->getNodeType() &&  // true is not NULL
				NcurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
			    {
			      DOMElement* NcurrentElement
			      = dynamic_cast< xercesc::DOMElement* >( NcurrentNode );
		      
			      if( XMLString::equals(NcurrentElement->getTagName(), TAG_Coords) )
			      {
				Point Q;
				
				const XMLCh* xcor = NcurrentElement->getAttribute(ATTR_x);
				char *tempx = XMLString::transcode(xcor);
				int xposi = atoi(tempx);
				Q.x=xposi;
				
				const XMLCh* ycor = NcurrentElement->getAttribute(ATTR_y);
				char *tempy = XMLString::transcode(ycor);
				int yposi = atoi(tempy);
				Q.y=yposi;
				
				C.setCoord(Q);
				
			      }
			    }
			  }
			  
			  //Attribute Specific to Graphic Region will go here
			  
			  
			  const XMLCh* xembText = pagecurrentElement->getAttribute(ATTR_embText);
			  char *t_embText = XMLString::transcode(xembText);
			  bool b_embText;
			  if(strcmp(t_embText,"false"))
			    b_embText = false;
			  else
			    b_embText = true;
			  C.setembText(b_embText);
			  
			  const XMLCh* xNC = pagecurrentElement->getAttribute(ATTR_numColors);
			  char *t_NC = XMLString::transcode(xNC);
			  int NC = atoi(t_NC);
			  C.setnumColors(NC);
			  
			  P.FillChartRegion(C);
			}
		      }
		    }
	      
            }
         }
      }
      
      return P;
      
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );
      ostringstream errBuf;
      errBuf << "Error parsing file: " << message << flush;
      XMLString::release( &message );
   }
}




page GetPageGroundtruth(char* name)
{
  if(name!=NULL)
  {
    string configFile(name);
    GetConfig appConfig;

    page P;
    P = appConfig.readConfigFile(configFile);
    
    return P;
  }
  else
  {
    printf("Give a proper name\n");
    exit(0);
  }
 
}



/* This main is provided for unit test of the class. */

/*
int main()
{
   string configFile="pc-00000205.xml"; // stat file. Get ambigious segfault otherwise.

   GetConfig appConfig;

   page P;
   P = appConfig.readConfigFile(configFile);
   
   printf("IMAGE INFO \n");
   printf("Image Name : %s\n",P.GetImageName());
   
   vector<TextRegion> TR = P.GetTextRegion();
   
   printf("Number of TextRegion : %d\n",TR.size());
   if(!TR.empty())
   {
     printf("Text Region is Present \n");
     
     for(int i=0;i<TR.size();i++)
     {
       TextRegion TempT = TR[i];
       vector<Point> poly = TempT.GetCoord();
     }
   }
   else
   {
     printf("No Text Region is Present\n");
   }
   
   vector<ChartRegion> CR = P.GetChartRegion();
   vector<GraphicRegion> GR = P.GetGraphicRegion();
   vector<ImageRegion> IR = P.GetImageRegion();
   vector<SeparatorRegion> SR = P.GetSeparatorRegion();
   
   

   //cout << "Application option A="  << appConfig.getOptionA()  << endl;
   //cout << "Application option B="  << appConfig.getOptionB()  << endl;

   return 0;
}

*/
