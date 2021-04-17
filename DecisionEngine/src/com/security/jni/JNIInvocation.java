/**
 * Created On 25th April,09
 */
package com.security.jni;


/**
 * JNIInvocation class invokes all the native methods declared to get the
 * attribute score
 * 
 */
public class JNIInvocation {
	/**
	 * Loads the DLL from java.path.entry
	 */
	static {		
		System.loadLibrary("libJNIInvok");
	}

	/** Native method to get the attribute score */
	public native int getScore(String pAttribute, String pReferenceAttributes,
			String pChildNode);

	/** Native method to get the pattern score */
	public native double getPatternScore(String pAttribute,
			String pReferenceAttributes, String pChildNode,
			String pPossibleTotalScore, String pDwellAdjValue,
			String pFlightAdjValue, String pReflectionAdjValue);

}
