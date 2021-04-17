// xmlparser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "xmlparser.h"
#include <iostream>


using namespace std;
using namespace secureparser;

vector<string> XmlParser::getAtributeScore(string pXmlString, string pNodeName)
{

	vector<string> attr;
	try
	{
		_bstr_t bstrt(pXmlString.c_str());

		// Create an instance of the parser
		CComPtr<IXMLDOMDocument> spXMLDOM;
		HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));
		if ( FAILED(hr) ) throw "Unable to create XML parser object";
		if ( spXMLDOM.p == NULL ) throw "Unable to create XML parser object";


		// Load the XML string...
		VARIANT_BOOL bSuccess = false;
		hr = spXMLDOM->loadXML(bstrt,&bSuccess);
		if ( FAILED(hr) ) throw "Unable to load XML document into the parser";
		if ( !bSuccess ) throw "Unable to load XML document into the parser";


		CComPtr<IXMLDOMNodeList> NodeListPtr;
		string strAttributes("Attributes");
		_bstr_t bstrtNodeName(strAttributes.c_str());

		hr = spXMLDOM->getElementsByTagName(bstrtNodeName,&NodeListPtr);
		long size=0;

		hr = NodeListPtr->get_length(&size);

		if(size > 0)
		{
			for(int i=0; i<size; i++)
			{
				CComPtr<IXMLDOMNode> node;
				NodeListPtr->get_item(i, &node);


				string strAttribute("Attribute");
				_bstr_t bstrtNodeName(strAttribute.c_str());


				CComPtr<IXMLDOMNodeList> NodeListPtrAttributes;
				node->get_childNodes(&NodeListPtrAttributes);

				long sizeAttributes=0;
				NodeListPtrAttributes->get_length(&sizeAttributes);

				for(int j=0; j<sizeAttributes; j++)
				{
					CComPtr<IXMLDOMNode> nodeAttributes;
					NodeListPtrAttributes->get_item(j, &nodeAttributes);

				    CComPtr<IXMLDOMNodeList> NodeListPtrAttribute;
				    nodeAttributes->get_childNodes(&NodeListPtrAttribute);

					long sizeAttribute=0;
					NodeListPtrAttribute->get_length(&sizeAttribute);
					for(int k=0; k<sizeAttribute; k++)
					{
						CComVariant varValue(VT_EMPTY);
						BSTR bstrtText;

						CComPtr<IXMLDOMNode> nodeParam;
						NodeListPtrAttribute->get_item(k, &nodeParam);
						hr = nodeParam->get_text(&bstrtText);
						if ( FAILED(hr) ) throw "Unable to retrieve value text";

						USES_CONVERSION;
						LPTSTR lpstrMsg2 = W2T(bstrtText);

						if(lpstrMsg2 == pNodeName){
							BSTR bstrtTextV;
							CComPtr<IXMLDOMNode> nodeParam1;
							NodeListPtrAttribute->get_item(k+1, &nodeParam1);
							hr = nodeParam1->get_text(&bstrtTextV);
							USES_CONVERSION;
							LPTSTR lpstrValue = W2T(bstrtTextV);
							attr.push_back(lpstrValue);
							SysFreeString(bstrtTextV);
							break;
						}
						SysFreeString(bstrtText);
					}
				}
			}

		}

   }
   catch(char* lpstrErr) {
	   CoUninitialize();
   }
   catch(...) {
	   CoUninitialize();
   }
   return attr;
}




