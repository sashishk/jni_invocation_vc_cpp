#ifndef USERATTRIBUTECALCULATION_H_
#define USERATTRIBUTECALCULATION_H_

#include <string>
using namespace std;

/*
 * TO Calculate the agent score from the incoming attributes.
 */
class MatchAttribute {
public:
	bool getScore(string pAttribute,string pReferenceAttributes,string pChildNode);
};

/*
 * TO Calculate the Pattern score from the incoming attributes.
 */
class MatchPattern {
public:
	double getPatternScore(string pAttribute,string pReferenceAttributes,string pChildNode,string pPossibleTotalScore,string pDwellAdjValue,string pFlightAdjValue,string pReflectionAdjValue);
};

#endif /*USERATTRIBUTECALCULATION_H_*/
