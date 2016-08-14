#include "PrepareAlethiaGT.h"



vector< SB > PrepareAlethiaGt(page P, vector< SB > blocks)
{
   vector<ChartRegion> CR = P.GetChartRegion(); // value 1
   vector<TextRegion> TR = P.GetTextRegion(); // value 0
   vector<GraphicRegion> GR = P.GetGraphicRegion(); // value 2
   vector<ImageRegion> IR = P.GetImageRegion(); // value 3
   vector<SeparatorRegion> SR = P.GetSeparatorRegion(); // value 4
   
   for(int i=0;i<blocks.size();i++)
      {
	blocks[i].gtflag = false;
	SB B = blocks[i];
	if(B.childs.empty())
	{
	  B.gtflag = false;
	}
	else
	{
	  B.gtflag = false;
	  for(int j=0;j<B.childs.size();j++)
	  {
	    B.childs[j].gtflag = false;
	  }
	}
	blocks[i] = B;
      }
   
   // text Block
   if(!TR.empty())
   {
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	//vector<int> count(B.ChildContour.size(),0);
	if(B.Fvecflag && !B.gtflag)
	{
	  for(int j=0;j<TR.size();j++)
	  {
	    TextRegion T = TR[j];
	    vector<Point> poly = T.GetCoord();
	    if(poly.empty())
	    {
	      printf("Something wrong in %s\n",T.getid());
	      //TR.erase(TR.begin()+j);
	      //exit(0);
	    }
	    else
	    {
	      Point pp = poly[0];
	      poly.push_back(pp);
	      int val;
	      if(B.childs.empty())
	      {
		val = PolygonInsidePolygonTest(poly, B.Contours);
		if(val == 2 || val == 3)
		{
		  B.GtClass = 0;
		  B.gtflag = true;
		  B.bgcolor = T.GetbgColor();
		  B.type = T.GetType();
		}
	      }
	      else
	      {	   
		for(int k=0;k<B.childs.size();k++)
		{
		    val = PolygonInsidePolygonTest(poly, B.childs[k].Contours);
		    if((val == 2 || val == 3) && B.childs[k].Fvecflag && !B.childs[k].gtflag)
		    {
		      B.childs[k].GtClass = 0;
		      B.childs[k].gtflag = true;
		      B.childs[k].bgcolor = T.GetbgColor();
		      B.childs[k].type = T.GetType();
		      B.GtClass = B.childs[k].GtClass;
		      B.gtflag = true;
		      B.bgcolor = T.GetbgColor();
		      B.type = T.GetType();
		    }
		    
		}
		
	      }
	    }
	  }
	}
	blocks[i] = B;
      }
   }
   
   // chart region
   
   if(!CR.empty())
   {
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	//vector<int> count(B.ChildContour.size(),0);
	if(B.Fvecflag  && !B.gtflag)
	{
	  for(int j=0;j<CR.size();j++)
	  {
	    ChartRegion C = CR[j];
	    vector<Point> poly = C.GetCoord();
	    if(poly.empty())
	    {
	      printf("Something wrong in %s\n",C.getid());
	      //TR.erase(TR.begin()+j);
	      //exit(0);
	    }
	    else
	    {
	      Point pp = poly[0];
	      poly.push_back(pp);
	      int val;
	      if(B.childs.empty())
	      {
		val = PolygonInsidePolygonTest(poly, B.Contours);
		if(val == 2 || val == 3)
		{
		  B.GtClass = 1;
		  B.gtflag = true;
		}
	      }
	      else
	      {	   
		for(int k=0;k<B.childs.size();k++)
		{
		    val = PolygonInsidePolygonTest(poly, B.childs[k].Contours);
		    if((val == 2 || val == 3) && B.childs[k].Fvecflag  && !B.childs[k].gtflag)
		    {
		      B.childs[k].GtClass = 1;
		      B.childs[k].gtflag = true;
		      B.GtClass = B.childs[k].GtClass;
		      B.gtflag = true;
		    }
		}
	      
	      }
	    }
	  }
	}
	blocks[i] = B;	
      }
   }
   
   // graphic region
   
   
   if(!GR.empty())
   {
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	//vector<int> count(B.ChildContour.size(),0);
	if(B.Fvecflag  && !B.gtflag)
	{
	  for(int j=0;j<GR.size();j++)
	  {
	    GraphicRegion G = GR[j];
	    vector<Point> poly = G.GetCoord();
	    if(poly.empty())
	    {
	      printf("Something wrong in %s\n",G.getid());
	      //TR.erase(TR.begin()+j);
	      //exit(0);
	    }
	    else
	    {
	      Point pp = poly[0];
	      poly.push_back(pp);
	      int val;
	      if(B.childs.empty())
	      {
		val = PolygonInsidePolygonTest(poly, B.Contours);
		if(val == 2 || val == 3)
		{
		  B.GtClass = 2;
		  B.gtflag = true;
		}
	      }
	      else
	      {	   
		for(int k=0;k<B.childs.size();k++)
		{
		    val = PolygonInsidePolygonTest(poly, B.childs[k].Contours);
		    if((val == 2 || val == 3) && B.childs[k].Fvecflag  && !B.childs[k].gtflag)
		    {
		      B.childs[k].GtClass = 2;
		      B.childs[k].gtflag = true;
		      B.GtClass = B.childs[k].GtClass;
		      B.gtflag = true;
		    }
		    
		}

	      }
	    }
	  }
	}
	blocks[i] = B;	
      }
   }
   
   // image region
   
   
   
   if(!IR.empty())
   {
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	//vector<int> count(B.ChildContour.size(),0);
	if(B.Fvecflag  && !B.gtflag)
	{
	  for(int j=0;j<IR.size();j++)
	  {
	    ImageRegion I = IR[j];
	    vector<Point> poly = I.GetCoord();
	    if(poly.empty())
	    {
	      printf("Something wrong in %s\n",I.getid());
	      //TR.erase(TR.begin()+j);
	      //exit(0);
	    }
	    else
	    {
	      Point pp = poly[0];
	      poly.push_back(pp);
	      int val;
	      if(B.childs.empty())
	      {
		val = PolygonInsidePolygonTest(poly, B.Contours);
		if(val == 2 || val == 3)
		{
		  B.GtClass = 3;
		  B.gtflag = true;
		}
	      }
	      else
	      {	   
		for(int k=0;k<B.childs.size();k++)
		{
		    val = PolygonInsidePolygonTest(poly, B.childs[k].Contours);
		    if((val == 2 || val == 3) && B.childs[k].Fvecflag  && !B.childs[k].gtflag)
		    {
		      B.childs[k].GtClass = 3;
		      B.childs[k].gtflag = true;
		      B.GtClass = B.childs[k].GtClass;
		      B.gtflag = true;
		    }
		}
		
	      }
	    }
	  }
	}
	blocks[i] = B;	
      }
   }
   
   
   // Separator region
   
   
   if(!SR.empty())
   {
      for(int i=0;i<blocks.size();i++)
      {
	SB B = blocks[i];
	//vector<int> count(B.ChildContour.size(),0);
	if(B.Fvecflag  && !B.gtflag)
	{
	  for(int j=0;j<SR.size();j++)
	  {
	    SeparatorRegion S = SR[j];
	    vector<Point> poly = S.GetCoord();
	    if(poly.empty())
	    {
	      printf("Something wrong in %s\n",S.getid());
	      //TR.erase(TR.begin()+j);
	      //exit(0);
	    }
	    else
	    {
	      Point pp = poly[0];
	      poly.push_back(pp);
	      int val;
	      if(B.childs.empty())
	      {
		val = PolygonInsidePolygonTest(poly, B.Contours);
		if(val == 2 || val == 3)
		{
		  B.GtClass = 4;
		  B.gtflag = true;
		}
	      }
	      else
	      {	   
		for(int k=0;k<B.childs.size();k++)
		{
		    val = PolygonInsidePolygonTest(poly, B.childs[k].Contours);
		    if((val == 2 || val == 3) && B.childs[k].Fvecflag  && !B.childs[k].gtflag)
		    {
		      B.childs[k].GtClass = 4;
		      B.childs[k].gtflag = true;
		      B.GtClass = B.childs[k].GtClass;
		      B.gtflag = true;
		    }
		}
		
	      }
	    }
	  }
	}
	blocks[i] = B;	
      }
   }
   
    for(int i=0;i<blocks.size();)
    {
      if(blocks[i].childs.empty())
      {
	if(blocks[i].gtflag)
	{
	  i = i + 1;
	}
	else	
	  blocks.erase(blocks.begin()+i);	
      }
      else
      {
	for(int k=0;k<blocks[i].childs.size();)
	{
	  if(blocks[i].childs[k].gtflag)
	    k = k + 1;
	  else
	    blocks[i].childs.erase(blocks[i].childs.begin()+k);
	}
	if(blocks[i].childs.empty())
	  blocks.erase(blocks.begin()+i);
	else
	  i = i + 1;	
      }
    }
   // Complete
   
   return blocks;
   
   
}
