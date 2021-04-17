#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <string>
#include <vector>
#include <atlbase.h>
#include <msxml.h>
#include <iostream>
#import <msxml6.dll> raw_interfaces_only no_namespace  
#include <string>
#include <fstream>

#endif /*XMLPARSER_H_*/

using namespace std;

/*
 * Function To Parse The XML String
 */
namespace secureparser{

class XmlParser {
public:
	XmlParser()
	{
		CoInitialize(NULL);
	}
	vector<string>  getAtributeScore(string pXmlString,string pNodeName);

	~XmlParser()
	{
		CoUninitialize();
	}

};

}