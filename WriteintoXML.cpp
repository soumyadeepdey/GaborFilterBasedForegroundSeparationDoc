#include "WriteintoXML.h"

void WriteGTXMLFile(page P, char *xmlname, vector<SB> Blocks)
{
  FILE *fp;
  char *name;
  fp = fopen(xmlname,"w");
  fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
  fprintf(fp,"<PcGts xmlns=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19 http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19/pagecontent.xsd\">\n<Metadata> <Creator>\"Soumyadeep Dey\"</Creator>\n<Created>%s</Created>\n<LastChange>%s</LastChange></Metadata>\n",buf,buf);
  fprintf(fp,"<Page imageFilename=\"%s\" imageWidth=\"%d\" imageHeight=\"%d\">\n",P.GetImageName(),P.GetCol(),P.GetRow());
  
  for(int i=0;i<Blocks.size();i++)
  {
    SB B = Blocks[i];
    if(B.gtflag && B.Fvecflag)
    {
      if(B.GtClass == 0)
      {
	fprintf(fp,"<TextRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	//printf("Size of Contours %d\n",B.Contours.size());
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	
	fprintf(fp,"</Coords></TextRegion>");
      }
      else if(B.GtClass == 1)
      {
	fprintf(fp,"<ChartRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></ChartRegion>");
      }
      else if(B.GtClass == 2)
      {
	fprintf(fp,"<GraphicsRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></GraphicsRegion>");
      }
      else if(B.GtClass == 3)
      {
	fprintf(fp,"<ImageRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></ImageRegion>");
      }
      else if(B.GtClass == 4)
      {
	fprintf(fp,"<SeparatorRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></SeparatorRegion>");
      }
      else if(B.GtClass == 5)
      {
	fprintf(fp,"<NoiseRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></NoiseRegion>");
      }
      else if(B.GtClass == 6)
      {
	fprintf(fp,"<MathsRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></MathsRegion>");
      }
      else if(B.GtClass == 7)
      {
	fprintf(fp,"<TableRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></TableRegion>");
      }
    }
  }
  

  
  fprintf(fp,"</Page>\n</PcGts>");
  fclose(fp);
}


void WriteOutputXMLFile(page P, char *xmlname, vector<SB> Blocks)
{
  FILE *fp;
  char *name;
  fp = fopen(xmlname,"w");
  fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
  fprintf(fp,"<PcGts xmlns=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19 http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19/pagecontent.xsd\">\n<Metadata> <Creator>\"Soumyadeep Dey\"</Creator>\n<Created>%s</Created>\n<LastChange>%s</LastChange></Metadata>\n",buf,buf);
  fprintf(fp,"<Page imageFilename=\"%s\" imageWidth=\"%d\" imageHeight=\"%d\">\n",P.GetImageName(),P.GetCol(),P.GetRow());

  for(int i=0;i<Blocks.size();i++)
  {
    SB B = Blocks[i];
    if(B.gtflag && B.Fvecflag)
    {
      if(B.PredictedClass == 0)
      {
	fprintf(fp,"<TextRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	
	fprintf(fp,"</Coords></TextRegion>");
      }
      else if(B.PredictedClass == 1)
      {
	fprintf(fp,"<ChartRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></ChartRegion>");
      }
      else if(B.PredictedClass == 2)
      {
	fprintf(fp,"<GraphicsRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></GraphicsRegion>");
      }
      else if(B.PredictedClass == 3)
      {
	fprintf(fp,"<ImageRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></ImageRegion>");
      }
      else if(B.PredictedClass == 4)
      {
	fprintf(fp,"<SeparatorRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></SeparatorRegion>");
      }
      else if(B.PredictedClass == 5)
      {
	fprintf(fp,"<NoiseRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></NoiseRegion>");
      }
      else if(B.PredictedClass == 6)
      {
	fprintf(fp,"<MathsRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></MathsRegion>");
      }
      else if(B.PredictedClass == 7)
      {
	fprintf(fp,"<TableRegion id=\"%d\">\n",i);
	fprintf(fp,"<Coords>\n");
	vector<Point> poly;
	approxPolyDP( Mat(B.Contours), poly, 3, true );
	for(int j=0;j<poly.size();j++)
	{
	  Point pp = poly[j];
	  fprintf(fp,"<Point x=\"%d\" y=\"%d\"/>\n",pp.x,pp.y);
	}
	fprintf(fp,"</Coords></TableRegion>");
      }
    }
  }
  

  
  fprintf(fp,"</Page>\n</PcGts>");
}
