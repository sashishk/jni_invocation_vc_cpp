#define UNICODE
#define _UNICODE

#include "com_security_jni_JNIInvocation.h"
#include "algo.h"
#include <iostream>
#include <windows.h>


using namespace std;
jobject *lock = new jobject();

char* jstringToChar(JNIEnv* env, jstring jstr)
{
	int length = env->GetStringLength(jstr );
	const jchar* jcstr = env->GetStringChars(jstr, 0 );
	char* rtn = (char*)malloc( length * 2 + 1 );
	int size = 0;
	size = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)jcstr,length,rtn,(length*2 +1 ),NULL,NULL);

	if ( size <= 0 ) return NULL;
		env->ReleaseStringChars(jstr, jcstr );
	rtn[ size ] = '\0';
	//cout<<rtn<<endl;
	return rtn;
}


JNIEXPORT jint JNICALL Java_com_security_jni_JNIInvocation_getScore
  (JNIEnv *env, jobject obj, jstring pAttribute,jstring pReferenceAttributes,jstring pChildNode)
{
	jboolean iscopy;
	jboolean val = false;
	try
	{
		env->ExceptionClear();
 	    env->MonitorEnter(obj);
		CAlgo *algo = new CAlgo();

/*		const char *mAttribute = env->GetStringUTFChars(pAttribute, &iscopy);
		const char *mReferenceAttributes = env->GetStringUTFChars(pReferenceAttributes, &iscopy);
		const char *mChildNode = env->GetStringUTFChars(pChildNode, &iscopy);*/
		
		 char* mAttribute = jstringToChar( env, pAttribute );
		 char *mReferenceAttributes = jstringToChar(env, pReferenceAttributes);
		 char *mChildNode = jstringToChar(env, pChildNode);

		val = algo->getScore(mAttribute,mReferenceAttributes,mChildNode);

/*		env->ReleaseStringUTFChars( pAttribute, mAttribute );
		env->ReleaseStringUTFChars( pReferenceAttributes, mReferenceAttributes );
		env->ReleaseStringUTFChars( pChildNode, mChildNode );*/

		if(mAttribute)
			delete mAttribute;
		if(mReferenceAttributes)
			delete mReferenceAttributes;
		if(mChildNode)
			delete mChildNode;

		if(algo)
			delete algo;

		env->MonitorExit(obj);

		  if(env->ExceptionOccurred()) {
			//cout << "ERROR IN GET SCORE" << endl;
			env->ExceptionDescribe();
			env->ExceptionClear();
		  }

	}catch(...){

		//cout<<"GOT THE EXCEPTION IN SCORE...."<<endl;
	}


	if (val)
	{
		return 1;
	}else{
		return 0;
	}
  }

/*
 * Class:     com_security_jni_JNIInvocation
 * Method:    getPatternScore
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)D
 */

JNIEXPORT jdouble JNICALL Java_com_security_jni_JNIInvocation_getPatternScore
  (JNIEnv *env, jobject obj, jstring pAttribute,jstring pReferenceAttributes,jstring pChildNode,jstring pPossibleTotalScore,jstring pDwellAdjValue,jstring pFlightAdjValue,jstring pReflectionAdjValue)
  {
	  jboolean iscopy;
	  double lResult = 0;
	//  lock = obj;
	  	try
	  	{
		  	env->ExceptionClear();
  			env->MonitorEnter(obj);

	  		 char *mAttribute = jstringToChar(env, pAttribute);
	  		 char *mReferenceAttributes = jstringToChar(env, pReferenceAttributes);
	  		 char *mChildNode = jstringToChar(env, pChildNode);
	  		 char *mPossibleTotalScore = jstringToChar(env, pPossibleTotalScore);
			 char *mDwellAdjValue = jstringToChar(env, pDwellAdjValue);
			 char *mFlightAdjValue = jstringToChar(env, pFlightAdjValue);
			 char *mReflectionAdjValue = jstringToChar(env, pReflectionAdjValue);

		CAlgo *algo = new CAlgo();

	  		lResult = algo->getPatternScore(mAttribute,mReferenceAttributes,mChildNode,mPossibleTotalScore,mDwellAdjValue,mFlightAdjValue,mReflectionAdjValue);

/*	  		env->ReleaseStringUTFChars( pAttribute, mAttribute );
	  		env->ReleaseStringUTFChars( pReferenceAttributes, mReferenceAttributes );
	  		env->ReleaseStringUTFChars( pChildNode, mChildNode );
	  		env->ReleaseStringUTFChars( pPossibleTotalScore, mPossibleTotalScore );
			env->ReleaseStringUTFChars( pDwellAdjValue, mDwellAdjValue );
	  		env->ReleaseStringUTFChars( pFlightAdjValue, mFlightAdjValue );
	  		env->ReleaseStringUTFChars( pReflectionAdjValue, mReflectionAdjValue );*/

		if(mAttribute)
			delete mAttribute;
		if(mReferenceAttributes)
			delete mReferenceAttributes;
		if(mChildNode)
			delete mChildNode;
		if(mPossibleTotalScore)
			delete mPossibleTotalScore;
		if(mDwellAdjValue)
			delete mDwellAdjValue;
		if(mFlightAdjValue)
			delete mFlightAdjValue;
		if(mReflectionAdjValue)
			delete mReflectionAdjValue;

if(algo)
	delete algo;
		  	env->MonitorExit(obj);

		  if(env->ExceptionOccurred()) {
			cout << "ERROR IN GET PATTERN SCORE" << endl;
			env->ExceptionDescribe();
			env->ExceptionClear();
		  }
		}catch(exception &e){

			cout<<"GOT THE EXCEPTION IN PATTERN SCORE..."<<e.what()<<endl;
		}
		catch(...)
		{
			cout<<"Algo Exception::"<<GetLastError()<<endl;
		}

		return lResult;
  }
