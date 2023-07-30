/*
 * omahacomp.cpp
 *
 *  Created on: 17 Apr 2023
 *      Author: Assaf
 */


#include "OHLHandsProcessor.h" ;
#include <fstream> ;

// command line args 1 and 2 for input and output files correspondingly

int main(int argc, char *argv[] ) {

	string iPath = argv[1] ;
	string oPath = argv[2] ;
	// trying to open given files, could use try / catch for i/o exceptions
	ifstream in(iPath) ;
	ofstream out(oPath) ;
	if ((in.is_open() == false) || (out.is_open() == false)) {
		cout << "could not open both files properly \nterminating" << endl ;
		exit(1) ;
	}
	OHLHandsProcessor ohlhp(in,out)  ;

	ohlhp.processAll() ;

	return 0 ;
}


