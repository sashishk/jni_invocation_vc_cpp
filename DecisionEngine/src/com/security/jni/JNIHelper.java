/**
 * 
 */
package com.security.jni;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;

/**

 * 
 */ 
public class JNIHelper {
	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		JNIInvocation lInvocation = new JNIInvocation();
		String lReferenceAttribute = readReferenceAttributeFile();
		String lLoginAttribute = readLoginAttributeFile();
		try {
			for (int lI = 0; lI < 10; lI++) {
				int lAttributeScore = lInvocation.getScore(lLoginAttribute,
						lReferenceAttribute, "TimeStamp");
				System.out.println("Attribute Score = " + lAttributeScore);
				 double lPatternScore =
				 lInvocation.getPatternScore(lLoginAttribute,
				 lReferenceAttribute, "Pattern", "100", "1", "0.50", "0.10");
				
				 System.out.println("Pattern Score = " + lPatternScore);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Reads the attribute.xml file which ll return a string
	 * 
	 * @return
	 */
	private static String readLoginAttributeFile() {
		StringBuilder lLoginAttributes = new StringBuilder();
		try {
			FileInputStream lFileInputStream = new FileInputStream(
					"attribute.xml");
			// Get the object of DataInputStream
			DataInputStream lDataInputStream = new DataInputStream(
					lFileInputStream);
			BufferedReader lBufferedReader = new BufferedReader(
					new InputStreamReader(lDataInputStream));
			// Read File Line By Line
			String lResultString;
			while ((lResultString = lBufferedReader.readLine()) != null) {
				// Print the content on the console
				lLoginAttributes.append(lResultString);
			}
			// Close the input stream
			lFileInputStream.close();

		} catch (Exception e) {
			e.printStackTrace();
		}
		// System.out.println(lLoginAttributes.toString());
		return lLoginAttributes.toString();
	}

	/**
	 * Reads sample.xml file which will return a string
	 * 
	 * @return
	 */
	private static String readReferenceAttributeFile() {
		StringBuilder lReferenceAttributes = new StringBuilder();

		try {
			FileInputStream lFileInputStream = new FileInputStream("sample.xml");
			// Get the object of DataInputStream
			DataInputStream lDataInputStream = new DataInputStream(
					lFileInputStream);
			BufferedReader lBufferedReader = new BufferedReader(
					new InputStreamReader(lDataInputStream));
			// Read File Line By Line
			String lResultString;
			while ((lResultString = lBufferedReader.readLine()) != null) {
				// Print the content on the console
				lReferenceAttributes.append(lResultString);
			}
			// Close the input stream
			lFileInputStream.close();

		} catch (Exception e) {
			e.printStackTrace();
		}
		// System.out.println(lReferenceAttributes.toString());
		return lReferenceAttributes.toString();
	}
}
