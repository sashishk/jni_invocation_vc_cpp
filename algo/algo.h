
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ALGO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ALGO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ALGO_EXPORTS
#define ALGO_API __declspec(dllexport)
#else
#define ALGO_API __declspec(dllimport)
#endif

#include <string>

using namespace std;

// This class is exported from the algo.dll
class ALGO_API CAlgo {
public:
	CAlgo(void);
	// TODO: add your methods here.
	bool getScore(const char * pAttribute,const char* pReferenceAttributes,const char *pChildNode);
	double getPatternScore(const char * pAttribute,const char * pReferenceAttributes,const char * pChildNode,const char * pPossibleTotalScore,const char * pDwellAdjValue,const char * pFlightAdjValue,const char * pReflectionAdjValue);

};

extern ALGO_API int nAlgo;

ALGO_API int fnAlgo(void);


