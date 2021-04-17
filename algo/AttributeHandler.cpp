
#include "StdAfx.h"
#include "AttributeHandler.h"
#include "xmlparser.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#define SIZE =26;



using namespace std;
using namespace secureparser;

class SumData
{
	public:
		int *_r;
		int *_d;
		int *_f;
	SumData()
	{
		_r = _d = _f = NULL;
	}
		
	~SumData()
	{
		if(_r)
		{
			delete [] _r;
			_r = NULL;
		}
		if(_d)
		{
			delete [] _d;
			_d = NULL;
		}
		if(_f)
		{
			delete [] _f;
			_f = NULL;
		}
	}


};

int extractPatternValues(SumData *data,vector<string> attr,int pCount);


/*Calculate Agent Score for the login user with reference 
 *to the reference pattern stored previously 
 */
bool MatchAttribute::getScore(string pAttribute,string pReferenceAttributes,string pChildNode)
{
	
	XmlParser *xmlparserObj = NULL;
	bool retVal = false;
	string lUserAgent = "";
	try
	{
		if(!pReferenceAttributes .empty())
		{			
			xmlparserObj = new  XmlParser();
			vector<string> attr = xmlparserObj->getAtributeScore(pAttribute,pChildNode);
			if(xmlparserObj)
			{
				delete xmlparserObj;
				xmlparserObj = NULL;
			}

			if(attr.size() > 0)
			{
				lUserAgent = attr[0];		   

				if(!lUserAgent.empty())
				{
					xmlparserObj = new  XmlParser();
					vector<string> refAttr = xmlparserObj->getAtributeScore(pReferenceAttributes,pChildNode);
					if(xmlparserObj)
					{
						delete xmlparserObj;
						xmlparserObj = NULL;
					}

					for(int j=0; j< refAttr.size(); j++)
					{
						string lReferenceUserAgent = refAttr[j];
						if(!lReferenceUserAgent.empty() && lReferenceUserAgent == lUserAgent)
						  return true;
					}
				}
			}
		}
	}
	catch(...)
	{
		if(xmlparserObj)
			delete xmlparserObj;	
	}
	
	return false;
}





/*Calculate Agent Score for the login user with reference 
 *to the reference pattern stored previously 
 */
double MatchPattern::getPatternScore(string pAttribute,string pReferenceAttributes,string pChildNode,string pPossibleTotalScore,string pDwellAdjValue,string pFlightAdjValue,string pReflectionAdjValue){
	
	
	XmlParser *xmlparserObj = NULL;
	double finalResult = 0;
	string lUserPAttern = "";
	char delims[] = "_";
	SumData *data = NULL;
	int iCount=0;

	if(pReferenceAttributes .empty()){			
			return finalResult;
	}

	xmlparserObj = new  XmlParser();	
	vector<string> attr = xmlparserObj->getAtributeScore(pAttribute,pChildNode);
	
	if(xmlparserObj)
	{
		delete xmlparserObj ;
		xmlparserObj = NULL;
	}
	
	if(attr.size() > 0)
	{
	  	for(int i=0; i< attr.size(); i++)
		{
			string st = attr[i];
			size_t pos = 0;
			while(pos < st.length())
			{
				pos = st.find('_', pos);
				if(pos<st.length())
				{
					pos++;
					iCount++;
				}
			}	        
		}
	 }
	iCount = iCount + 1;
	
	xmlparserObj = new  XmlParser();	
	vector<string> refAttr = xmlparserObj->getAtributeScore(pReferenceAttributes,pChildNode);	
	int lTotal = refAttr.size();	
	
	if(xmlparserObj )
	{
		delete xmlparserObj ;
		xmlparserObj = NULL;
	}
	
	//Put All the Dwell,Flight and reflection time values into a 2-dimensional array
	if(refAttr.size() > 0){	
		data = new SumData[refAttr.size()];
		iCount = extractPatternValues(data,refAttr,iCount);		
	}
	
	  double lSumRefl = 0;
	  double lSumRef2 = 0;
	  double lSumDwellLast = 0;	 

	  map<int,double> lDwellValue;
	  map<int,double> lFlightValue;

	  //Add DWELL,FLIGHT and REFLECTION vertically
	  for(int i=0; i< refAttr.size(); i++)
	  {	
		for(int j=0; j<iCount; j++)
		{
			if(j==0){

				lSumRefl = lSumRefl + data[i]._r[j];
				
			}else if(j== iCount-1){				
				
				lSumRef2 = lSumRef2 + data[i]._r[j];
				
			}else if(j== iCount-2){

				lSumDwellLast = lSumDwellLast + data[i]._d[j];

			}else if(i>0 && (j>0 && j<=iCount-3)){ 
			
				double lTempFlight = lFlightValue.find(j)->second ; 
				double lTempDwell =  lDwellValue.find(j)->second ;
						
				lFlightValue[j] = data[i]._f[j] + lTempFlight;
				lDwellValue[j] = data[i]._d[j] + lTempDwell;
			
				
			}else{	
			
				lFlightValue[j] = data[i]._f[j];
				lDwellValue[j] = data[i]._d[j];

			}
		}
	  }

	 
	 double lAvgStartRef=0;
	 double lAvgEndRef=0;
	 double lAvgEndDwell=0;

	 vector<double> lAvgDwellValue;
	 vector<double> lAvgFlightValue;

	 //Take the average of all the dwell , flight and reflection values.	 
	 lAvgStartRef = lSumRefl/lTotal;
     lAvgEndRef = lSumRef2/lTotal;
	 lAvgEndDwell = lSumDwellLast/lTotal;
	 
	 for(int p=1;p<iCount-2;p++){
		double lTotalDwell = lDwellValue.find(p)->second ;
		double lTotalFlight = lFlightValue.find(p)->second ;		
		lAvgDwellValue.push_back(lTotalDwell/lTotal);
		lAvgFlightValue.push_back(lTotalFlight/lTotal);
	 }	 
	 
	//Calculate Standard deviation against all the flight,dwell and reflection vlues
	//Sum of all Dwell,Flight and Reflection values

	vector<int> lSqrValueOfDwell;
	vector<int> lSqrValueOfFlight;
	map<int,double> lSqDwellValue;
	map<int,double> lSqFlightValue;

	int lTempSqrStartRef = 0;
	int lTempSqrEndRef = 0;
	long lTempSqrEndDwell = 0;
	long lTempSqrDwell = 0;
	long lTempSqrFlight = 0;
	int lCntF = 0;
	int lCntF1 = 0;
    int lCntDAvg=0;
	int lCntD = 0;
	int lCntD1 = 0;
    int lCntFAvg=0;
	int lValueN = 0;
	for(int k=0; k< refAttr.size(); k++)
	  {	
		for(int l=0; l<iCount; l++)
		{
			if(l==0){
				lTempSqrStartRef = lTempSqrStartRef + pow(data[k]._r[l]-lAvgStartRef,2);
			}else if(l== iCount-1){	
				lTempSqrEndRef = lTempSqrEndRef + pow(data[k]._r[l]-lAvgEndRef,2);	
			}else if(l== iCount-2){				
				lTempSqrEndDwell = lTempSqrEndDwell + pow(data[k]._d[l]-lAvgEndDwell,2);
				lValueN ++;
			}else if(k>0 && (l>0 && l<=iCount-3)){ 
			
				int lTempDwellSqrValue = 0;
				int lTempFlightSqrValue = 0;
				double lTempDwellV = 0;
				double lTempFlightV = 0; 
				while(lCntDAvg <lAvgDwellValue.size()){ 
					 if(NULL == lSqDwellValue[lCntD]){
					     lTempDwellV = lSqrValueOfDwell[lCntD];						 
					 }else{
						 lTempDwellV = lSqDwellValue.find(lCntD)->second;						 
					 }
					 
					 lTempDwellV = lTempDwellV + pow(data[k]._d[l]-lAvgDwellValue[lCntDAvg],2);
					
					 if(NULL == lSqDwellValue[lCntD]){						 
						 lSqDwellValue[lCntD] = lTempDwellV;
					 }else{
						 lSqDwellValue[lCntD] = lTempDwellV;
					 }
					 lCntD = lCntD + 1;
					 lTempDwellV = 0;					 
					 lCntDAvg++;
					 break;
				}				

				while(lCntFAvg <lAvgFlightValue.size()){					
					 if(NULL == lSqFlightValue[lCntF]){
					     lTempFlightV = lSqrValueOfFlight[lCntF];
					 }else{						 
						 lTempFlightV = lSqFlightValue.find(lCntF)->second;
					 }
					 lTempFlightV = lTempFlightV + pow(data[k]._f[l]-lAvgFlightValue[lCntFAvg],2);					
					 if(NULL == lSqFlightValue[lCntF]){
						 lSqFlightValue[lCntF] = lTempFlightV;
					 }else{
						 lSqFlightValue[lCntF] = lTempFlightV;
					 }

					 lCntF ++;
					 lTempFlightV = 0;					 
					 lCntFAvg = lCntFAvg + 1;
					 break;
				} 
				if(lCntDAvg == lAvgDwellValue.size() && lCntFAvg == lAvgFlightValue.size()){				
					lCntDAvg = 0;
					lCntFAvg = 0;
					lCntF = 0;
					lCntD = 0;
				}
				
			}else{
				 double lTempDwellSqrValue = 0;
				 double lTempFlightSqrValue = 0;
                 
				 while(lCntD1 < lAvgDwellValue.size()){				
					lTempDwellSqrValue = lTempDwellSqrValue + pow(data[k]._d[l]-lAvgDwellValue[lCntD1],2);
					lSqrValueOfDwell.push_back(lTempDwellSqrValue);
					lTempDwellSqrValue = 0;
					lCntD1 ++;
					break;
				 }
				 while(lCntF1 < lAvgFlightValue.size()){
					lTempFlightSqrValue = lTempFlightSqrValue + pow(data[k]._f[l]-lAvgFlightValue[lCntF1],2);		
					lSqrValueOfFlight.push_back(lTempFlightSqrValue);
					lTempFlightSqrValue = 0;
					lCntF1++;					
					break;
				 }

			}
		}
	  }
	 
	 if(data)
	{
	  delete[] data;
	  data = NULL;
	}

	cout<<"************************** 1"<<endl;
	 int lTempSize = lSqDwellValue.size();	 
	 lSqDwellValue[lTempSize] = lTempSqrEndDwell;
	 
	 int lT = 0;
	 int lU = 0;
	 double lStdDwell = 0 ;
	 double lStdFlight = 0 ;
	 vector<double> lDwStdVector;
	 vector<double> lFlStdVector;
	

	 double lStdStRef = sqrt(lTempSqrStartRef/(lValueN-1));
	 double lStdEnRef = sqrt(lTempSqrEndRef/(lValueN-1));

	 cout<<"STD VALUE START REFLECTION = "<<lStdStRef<<endl;
	 while(lT < lSqDwellValue.size()){	
		 double lTempSqrRootDwValue = sqrt(lSqDwellValue.find(lT)->second/(lValueN-1));	
		 cout<<"STD VALUE DWELL = "<<lTempSqrRootDwValue<<endl;
		 lDwStdVector.push_back(lTempSqrRootDwValue);
		 lStdDwell = lStdDwell + lTempSqrRootDwValue;
		 lT++;
	 }
	 while(lU < lSqFlightValue.size()){	
		 double lTempSqrRootFlValue = sqrt(lSqFlightValue.find(lU)->second/(lValueN-1));
		 cout<<"STD VALUE FLIGHT = "<<lTempSqrRootFlValue<<endl;
		 lFlStdVector.push_back(lTempSqrRootFlValue);
		 lStdFlight = lStdFlight + lTempSqrRootFlValue;
		 lU++;
	 }
     cout<<"STD VALUE END REFLECTION = "<<lStdEnRef<<endl;

	 double lRefStdAvgValue = (lStdStRef + lStdEnRef)/2;
	 double lDwellStdAvgValue = lStdDwell/lDwStdVector.size();
	 double lFlightStdAvgValue = lStdFlight/lFlStdVector.size();
	 

     double lRefStDivValue = lRefStdAvgValue/lStdStRef;
	 double lRefEnDivValue = lRefStdAvgValue/lStdEnRef;
	 
     vector<double> lDwDivVector;
	 vector<double> lFlDivVector;

	 for(int iL=0;iL<lDwStdVector.size();iL++){ 
         double lTemp = lDwellStdAvgValue/lDwStdVector[iL];		
		 lDwDivVector.push_back(lTemp);
	 }	 
	 for(int iM=0;iM< lFlStdVector.size();iM++){
		 double lTemp = lFlightStdAvgValue/lFlStdVector[iM];		
		 lFlDivVector.push_back(lTemp);
	 }
	
	 int lCountInput = 0;
	 int iCnt=0; 
	 int jCnt=0;
	 double lStRefDiffV = 0;
	 double lEnRefDiffV = 0;
	 double lTempDwell = 0;
	 double lTempFlight = 0;
	 vector<double> lDwDifVector;
	 vector<double> lFlDifVector;
	 SumData *dataLogin = NULL;

	 if(attr.size() > 0)
	 {			    
		dataLogin = new SumData[attr.size()];		
		lCountInput = extractPatternValues(dataLogin,attr,iCount);
		lCountInput = iCount;
	 }
	int lTotalCount = 0;

	 for(int iI=0; iI< attr.size(); iI++)
	  {	
		for(int jJ=0; jJ<lCountInput; jJ++)
		{
			if(jJ==0){

				double lTemp = dataLogin[iI]._r[jJ] - lAvgStartRef;

				if(lTemp < 0){
					lStRefDiffV = -(lTemp);
				}else{
					lStRefDiffV = lTemp;
				}

			}else if(jJ== iCount-1){				
				double lTemp = dataLogin[iI]._r[jJ] - lAvgEndRef;
				if(lTemp < 0){
					lEnRefDiffV = -(lTemp);
				}else{
					lEnRefDiffV = lTemp;
				}
			
			}else if(jJ== iCount-2){				
				double lTemp = dataLogin[iI]._d[jJ] - lAvgEndDwell;
				if(lTemp < 0){
					lTempDwell = -(lTemp);
				}else{
					lTempDwell = lTemp;
				}
				lDwDifVector.push_back(lTempDwell);
				lTotalCount++;

			}else if(iI>0 && (jJ>0 && jJ<=iCount-3)){ 
			
				while(iCnt< lAvgDwellValue.size()){
					double lTemp = dataLogin[iI]._d[jJ]-lAvgDwellValue[iCnt];	
					if(lTemp < 0){
						lTempDwell = -(lTemp);
					}else{
						lTempDwell = lTemp;
					}
					lDwDifVector.push_back(lTempDwell);
					iCnt++;
					break;
				 }
				
				 while(jCnt < lAvgFlightValue.size()){					
					double lTemp = dataLogin[iI]._f[jJ]- lAvgFlightValue[jCnt];	
					if(lTemp < 0){
						lTempFlight = -(lTemp);
					}else{
						lTempFlight = lTemp;
					}
					
					lFlDifVector.push_back(lTempFlight);
					jCnt++;
					break;
				 }	
				 lTotalCount++;
				
			}else{
			
				 while(iCnt< lAvgDwellValue.size()){				
					double lTemp = dataLogin[iI]._d[jJ]-lAvgDwellValue[iCnt];	
					if(lTemp < 0){
						lTempDwell = -(lTemp);
					}else{
						lTempDwell = lTemp;
					}					
					lDwDifVector.push_back(lTempDwell);
					iCnt++;
					break;
				 }				 
				 while(jCnt < lAvgFlightValue.size()){					 
					double lTemp = dataLogin[iI]._f[jJ]- lAvgFlightValue[jCnt];						
					if(lTemp < 0){
						lTempFlight = -(lTemp);
					}else{
						lTempFlight = lTemp;
					}
					
					lFlDifVector.push_back(lTempFlight);
					jCnt++;
					break;
				 }
				 	lTotalCount++;
				 
			}
		}
	  }
	 
	  if(dataLogin)
	  {
		  delete[] dataLogin;
		  dataLogin = NULL;
	  }
cout<<"************************** 2"<<endl;
	 int d2 = 0;
	 int f2 = 0;
	 
	 double lDwellAdjValue = strtod(pDwellAdjValue.c_str(),NULL);
	 double lFlightAdjValue = strtod(pFlightAdjValue.c_str(),NULL);
	 double lReflectionAdjValue = strtod(pReflectionAdjValue.c_str(),NULL);
     double lPossibleTotalScore = strtod(pPossibleTotalScore.c_str(),NULL);

	 
	 double lTempDwAdjValue = 0;
	 double lTempFlAdjValue = 0;


	 double lStRefMulV = lRefStDivValue * lStRefDiffV;
	 double lEnRefMulV = lRefEnDivValue * lEnRefDiffV;

	 double lAdjStRef = lStRefMulV * lReflectionAdjValue; 
	 double lAdjEnRef = lEnRefMulV * lReflectionAdjValue;
	 
    
	 for(int d1=0; d1<lDwDifVector.size(); d1++){			 
		 while(d2<lDwDivVector.size()){		
			double lTempMulDwellV = lDwDifVector[d1] * lDwDivVector[d2];			
			lTempDwAdjValue = lTempDwAdjValue + (lTempMulDwellV * lDwellAdjValue);
			d2++;
			break;
		 }	

	 }	 

	 for(int f1=0; f1<lFlDifVector.size();f1++){		
		 while(f2<lFlDivVector.size()){			
			double lTempMulFlightV = lFlDifVector[f1] * lFlDivVector[f2];
			lTempFlAdjValue = lTempFlAdjValue + (lTempMulFlightV * lFlightAdjValue);
			f2++;
			break;
		 }

	 }
	
	double lTotalAdjPatternValue = lAdjStRef+lAdjEnRef+lTempDwAdjValue+lTempFlAdjValue;	
	int lTempTotal = lTotalCount + (lTotalCount-1)+2;	
	cout<<"TotalAdjPatternValue = "<<lTotalAdjPatternValue<< " :: TOTAL = "<<lTempTotal<<endl;
	cout<<"TOTAL AVERAGE = "<<lTotalAdjPatternValue/lTempTotal<<endl;
	finalResult = 100-(lTotalAdjPatternValue/lTempTotal);
	cout<<"finalResult = "<<finalResult<<endl;
	return finalResult;
}



/**
 * Loop through the vector of pattern values and 
 * put into a 2-dimensional array 
 */
int extractPatternValues(SumData *data,vector<string> attr,int pCount){    
	    int iCount= 0;
		int iCnt = pCount;

	  	for(int i=0; i< attr.size(); i++)
		{	  	
			data[i]._r = new int[iCnt];
			data[i]._d = new int[iCnt];
			data[i]._f = new int[iCnt];			
			char *str = const_cast<char*>(attr[i].c_str());
			char delims[] = "_";
			char *result = strtok( str , delims );				
			
			string res = result;
			size_t ret = res.find_first_of('r');

			iCount = 0;
			if(ret != string::npos)
			{
				size_t pos;
				pos = res.find("r");
				string str3 = res.substr (pos+1);
				double intReturn = atoi(str3.c_str());
				
				data[i]._r[iCount] = intReturn;				
				data[i]._d[iCount] = 0;				
				data[i]._f[iCount] = 0;				
			}

			while( result != NULL ) 
			{				
				iCount++;
				result = strtok( NULL, delims );				
				if(result != NULL){					
				   res = result;
				}				
				size_t ret = res.find_first_of('r');
				if(ret == string::npos){					
					size_t posD=0;
					size_t posF=0;

					string strD;
					string strF;

					double lDwellValue=0;
					double lFlightValue=0;

					posD = res.find("d");
					posF = res.find("f");

					strD = res.substr (posD+1,posF-1);
					lDwellValue = atoi(strD.c_str());

					if(posF<=pCount-2){					   
					   strF = res.substr (posF+1);					   
					   lFlightValue = atoi(strF.c_str());
					}else{
						lFlightValue = 0;
					}
					
					data[i]._r[iCount] = 0;
					data[i]._d[iCount] =  lDwellValue;
					data[i]._f[iCount] = lFlightValue;
					
				}else{
					size_t pos;
					pos = res.find("r");
					string str3 = res.substr (pos+1);
					double intReturn = atoi(str3.c_str());					
					data[i]._r[iCount] = intReturn;
					data[i]._d[iCount] = 0;
					data[i]._f[iCount] = 0;					
				}				
			}
		}		
    return iCount;
}





