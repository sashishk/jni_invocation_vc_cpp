// algo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "algo.h"
#include <iostream>

#include "AttributeHandler.h"

using namespace std;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
ALGO_API int nAlgo=0;

// This is an example of an exported function.
ALGO_API int fnAlgo(void)
{
	return 42;
}

// This is an example of an exported function.
ALGO_API bool CAlgo::getScore(const char * pAttribute,const char* pReferenceAttributes,const char *pChildNode)
{
	bool retVal = false;	
	string att(pAttribute);
	string refatt(pReferenceAttributes);
	string node(pChildNode);

	try{
			MatchAttribute lMatchAttribute;
			retVal = lMatchAttribute.getScore(att,refatt,node);		
	 }	
		catch(...)
		{		
			cout<<"Exception Inside Algo::GetScore"<<endl;
		}
	return retVal;
}

ALGO_API double CAlgo::getPatternScore(const char * pAttribute,const char * pReferenceAttributes,const char * pChildNode,const char * pPossibleTotalScore,const char * pDwellAdjValue,const char * pFlightAdjValue,const char * pReflectionAdjValue)
{
	double retVal = 0;
	string att(pAttribute);
	string refatt(pReferenceAttributes);
	string node(pChildNode);
	string pscore(pPossibleTotalScore);
	string dwellscore(pDwellAdjValue);
	string flightscore(pFlightAdjValue);
	string reflectionscore(pReflectionAdjValue);
	try{
		MatchPattern lMatchPatternObj;
		retVal = lMatchPatternObj.getPatternScore(att,refatt,node,pscore,dwellscore,flightscore,reflectionscore);
	}
	catch(...)
	{
		cout<<"Exception Inside Algo::PatternScore"<<endl;
	}
	return retVal;
}


// This is the constructor of a class that has been exported.
// see algo.h for the class definition
CAlgo::CAlgo()
{ 
	return; 
}

