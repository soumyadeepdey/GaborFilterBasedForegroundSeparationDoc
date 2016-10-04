#ifndef Alethia_PARSER_HPP
#define Alethia_PARSER_HPP
/**
 *  @file
 *  Class "GetConfig" provides the functions to read the XML data.
 *  @version 1.0
 */
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>


#include "../StandardHeaders.h"

#include <string>
#include <stdexcept>

#include <vector>
using namespace std;

// Error codes

enum {
   ERROR_ARGS = 1, 
   ERROR_XERCES_INIT,
   ERROR_PARSE,
   ERROR_EMPTY_DOCUMENT
};



class Region
{
public:
//   Region()
//   {
//     printf("Region Object Created\n");
//   }
   ~Region()
   {
     //printf("Region Object Destroyed\n");
     cord.clear();
   }
  void setCoord(Point p)
    {
      cord.push_back(p);
    }
    void setid(char *i)
    {
      id = i;
    }
    void settype(char *i)
    {
      type = i;
    }
    void setbgcolor(char *i)
    {
      bgcolor = i;
    }
    void setorientation(float val)
    {
      orientation = val;
    }
    
    char* getid()
    {
      return id;
    }
    
    vector<Point> GetCoord()
    {
      return cord;
    }
    
    char* GetbgColor()
    {
      return bgcolor;
    }
    
    char* GetType()
    {
      return type;
    }
    
    float GetOrientation()
    {
      return orientation;
    }
    
private:
  char* id;
  vector<Point> cord;
  char* type;
  char* bgcolor;
  float orientation;
};

class TextRegion: public Region
{
public:
//     TextRegion()
//     {
//       printf("Text Region Object Created\n");
//     }
//     ~TextRegion()
//     {
//       printf("Text Region Object Destroyed\n");
//     }
    
    void setReadingOrientation(float val)
    {
      readingOrientation = val;
    }
    void setreadingDirection(char *ch)
    {
      readingDirection = ch;
    }
    void settextColour(char *ch)
    {
      textColour = ch;
    }
    void setreverseVideo(bool b)
    {
      reverseVideo = b;
    }
    void setindented(bool b)
    {
      indented = b;
    }
    void setprimaryLanguage(char *ch)
    {
      primaryLanguage = ch;
    }
    void setprimaryScript(char *ch)
    {
      primaryScript = ch;
    }
    
    float getReadingOrientation()
    {
      return readingOrientation;
    }
    char* getreadingDirection()
    {
      return readingDirection;
    }
    char* gettextColour()
    {
      return textColour;
    }
    bool getreverseVideo()
    {
      return reverseVideo;
    }
    bool getindented()
    {
      return indented;
    }
    char* getprimaryLanguage()
    {
      return primaryLanguage;
    }
    char* getprimaryScript()
    {
      return primaryScript;
    }
    
private:
  float readingOrientation;
  char* readingDirection;
  char *textColour;
  bool reverseVideo;
  bool indented; 
  char* primaryLanguage; 
  char* primaryScript;
};

class ChartRegion: public Region
{
public:
//     ChartRegion()
//     {
//       printf("Chart Region Object Created\n");
//     }
//     ~ChartRegion()
//     {
//       printf("Chart Region Object Destroyed\n");
//     }
    void setembText(bool b)
    {
      embText = b;
    }
    void setnumColors(int val)
    {
      numColors = val;
    }
    
    bool getembText()
    {
      return embText;
    }
    
    int getnumColors()
    {
      return numColors;
    }
    
private:
  bool embText;
  int numColors;
};

class ImageRegion: public Region
{
public:
//     ImageRegion()
//     {
//       printf("Image Region Object Created\n");
//     }
//     ~ImageRegion()
//     {
//       printf("Image Region Object Destroyed\n");
//     }
    void setembText(bool b)
    {
      embText = b;
    }
    void setColorDepth(char *ch)
    {
      ColorDepth = ch;
    }
    
    bool getembText()
    {
      return embText;
    }
    char* getColorDepth()
    {
      return ColorDepth;
    }
    
private:
    bool embText;
    char* ColorDepth;
};

class GraphicRegion: public Region
{
public:
//   GraphicRegion()
//     {
//       printf("Graphic Region Object Created\n");
//     }
//   ~GraphicRegion()
//     {
//       printf("Graphic Region Object Destroyed\n");
//     }
   void setembText(bool b)
    {
      embText = b;
    }
    void setnumColors(int val)
    {
      numColors = val;
    }
    
    bool getembText()
    {
      return embText;
    }
    
    int getnumColors()
    {
      return numColors;
    }
    
    
private:
  bool embText;
  int numColors;
};

class SeparatorRegion: public Region
{
public:
  
//   SeparatorRegion()
//     {
//       printf("SeparatorRegion Object Created\n");
//     }
//   ~SeparatorRegion()
//     {
//       printf("SeparatorRegion Object Destroyed\n");
//     }
    
    void setcolor(char *c)
    {
      color = c;
    }
    
    char* getcolor()
    {
      return color;
    }
    
private:
  char *color;
    
};

class MathsRegion: public Region
{
  
};

class NoiseRegion: public Region
{
  
};

class TableRegion: public Region
{
public:  
  
  
    void setembText(bool x)
    {
      embText = x;
    }
      
    bool getembText()
    {
      return embText;
    }
    
    void setLineSeparator(bool x)
    {
      lineSeparator = x;
    }
    
    bool getLineSeparator()
    {
      return lineSeparator;
    }
    
    void setRow(int x)
    {
      row = x;
    }
    
    
    int getRow()
    {
      return row;
    }
    
    void setcol(int x)
    {
      col = x;
    }
    
    int getCol()
    {
      return col;
    }
    
private:
  bool embText;
  int row;
  int col;
  bool lineSeparator;
};

class page
{
public:
//   page()
//   {
//       printf("Page Object Created\n");
//     }
  ~page()
  {
    TR.clear();
    GR.clear();
    IR.clear();
    CR.clear();
    SR.clear();
    NR.clear();
    MR.clear();
    //printf("Page Object Destroyed\n");
  }
  void SetImageName(char* ch)
  {
    imagename = ch;
  }
  void SetRow(int r)
  {
    row = r;
  }
  void SetCol(int c)
  {
    col = c;
  }
  void FillTextRegion(TextRegion x)
  {
    TR.push_back(x);
  }
  void FillGraphicRegion(GraphicRegion x)
  {
    GR.push_back(x);
  }
  void FillImageRegion(ImageRegion x)
  {
    IR.push_back(x);
  }
  void FillChartRegion(ChartRegion x)
  {
    CR.push_back(x);
  }
  
  void FillSeparatorRegion(SeparatorRegion x)
  {
    SR.push_back(x);
  }
  
  void FillNoiseRegion(NoiseRegion x)
  {
    NR.push_back(x);
  }
  
  void FillMathsRegion(MathsRegion x)
  {
    MR.push_back(x);
  }
  
  void FillTableRegion(TableRegion x)
  {
    TabR.push_back(x);
  }
  
  vector<TextRegion> GetTextRegion()
  {
    return TR;
  }
  
  vector<MathsRegion> GetMathsRegion()
  {
    return MR;
  }
  
  vector<GraphicRegion> GetGraphicRegion()
  {
    return GR;
  }
  vector<ImageRegion> GetImageRegion()
  {
    return IR;
  }
  
  vector<ChartRegion> GetChartRegion()
  {
    return CR;
  }
  
  vector<TableRegion> GetTableRegion()
  {
    return TabR;
  }
  
  vector<SeparatorRegion> GetSeparatorRegion()
  {
    return SR;
  }
  
  vector<NoiseRegion> GetNoiseRegion()
  {
    return NR;
  }
  
  char* GetImageName()
  {
    return imagename;
  }
  
  int GetRow()
  {
    return row;
  }
  
  int GetCol()
  {
    return col;
  }
  
private:
  char* imagename;
  int row;
  int col;
  vector<TextRegion> TR;
  vector<GraphicRegion> GR;
  vector<ImageRegion> IR;
  vector<ChartRegion> CR;
  vector<SeparatorRegion> SR;
  vector<NoiseRegion> NR;
  vector<MathsRegion> MR;
  vector<TableRegion> TabR;
};



class GetConfig
{
public:
   GetConfig();
  ~GetConfig();
   page readConfigFile(std::string&) throw(std::runtime_error);

private:
   xercesc::XercesDOMParser *m_ConfigFileParser;


   // Internal class use only. Hold Xerces data in UTF-16 SMLCh type.

   XMLCh* TAG_Page;
   XMLCh* ATTR_Pagename;
   XMLCh* ATTR_PagenHeight;
   XMLCh* ATTR_PagenWidth;
   XMLCh* TAG_TextRegion;
   XMLCh* TAG_MathsRegion;
   XMLCh* TAG_GraphicRegion;
   XMLCh* TAG_ImageRegion;
   XMLCh* TAG_ChartRegion;
   XMLCh* TAG_TableRegion;
   XMLCh* TAG_SeparatorRegion;
   XMLCh* TAG_NoiseRegion;
   XMLCh* TAG_Coords; 
   XMLCh* TAG_Point;
   XMLCh* ATTR_id;
   XMLCh* ATTR_type;
   XMLCh* ATTR_bgColour;
   XMLCh* ATTR_colour;
   XMLCh* ATTR_orientation;
   XMLCh* ATTR_readingOrientation;
   XMLCh* ATTR_readingDirection;
   XMLCh* ATTR_textColour;
   XMLCh* ATTR_reverseVideo;
   XMLCh* ATTR_indented;
   XMLCh* ATTR_primaryLanguage;
   XMLCh* ATTR_primaryScript;
   XMLCh* ATTR_embText;
   XMLCh* ATTR_numColors;
   XMLCh* ATTR_ColorDepth;
   XMLCh* ATTR_x;
   XMLCh* ATTR_y;
   XMLCh* ATTR_row;
   XMLCh* ATTR_col;
   XMLCh* ATTR_LineSeparator;

};

/**
 * @functions: GetPageGroundtruth
 * @Description This function takes an input xml file containing groundtruth data generated by ALETHIA Software
 * @return This function return a datatype of page
 * 
 **/
page GetPageGroundtruth(char *name);


#endif
