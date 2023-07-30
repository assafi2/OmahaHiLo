/*
 * HandsProcessor.cpp
 *
 *  Created on: 16 Apr 2023
 *      Author: Assaf
 */

#include "HandsProcessor.h"  ;
//#include <string> ;
#include <sstream> ;
#include <iostream> ;
# include <fstream> ;


HandsProcessor::HandsProcessor(const istream& inn = cin, const ostream& outt = cout, int handSizee = 2, int boardSizee = 5)
	: in(inn), out(outt), handSize(handSizee), boardSize(boardSizee) {

/*
	hand1 = new Card[handSize] ;
	hand2 = new Card[handSize] ;
	board = new Card[boardSize] ;
*/
	// calloc to avoid unnecessary constructor calls, no need to default value enums
	hand1 = (Card*)calloc(handSize, sizeof(Card)) ;
	hand2 = (Card*)calloc(handSize, sizeof(Card)) ;
	board = (Card*)calloc(boardSize, sizeof(Card)) ;

}

int HandsProcessor::processAll() {
	int total ;
	while (processNext() == true )  {
		++total ;
		(ostream&)out << '\n' ;
	}
	return total ;
}

bool HandsProcessor::processNext() {

		// TODO create as function extractData

		if (in.eof()) return false ;
		// read next line from the input file to buffer
//		in.getline(cur_line, MAX_LINE_SIZE) ;
		string line ;
		getline((istream&)in, line) ;
//		string line(cur_line) ;

		// outputting input line (could be a user configured feature)

		(ostream&)out << line << endl ;

		// tokenizing an input line according to a game's relevant delimiter
		vector <string> tokens;
		stringstream sLine(line); // for tokenizing
		string token ;
		char del = setDelimiter() ;
		while(getline(sLine, token, del))
			{
		        tokens.push_back(token);
		    }

		// if we had support a more generalized case of variable
		// hands number we would use a different logic, also below

		vector<string>::iterator iter =  tokens.begin()  ;
		processToken(*iter++, hand1,  handSize) ;
		if (iter != tokens.end()) processToken(*iter++, hand2, handSize) ;
		if (iter != tokens.end()) processToken(*iter++, board , boardSize) ;

		compareHands() ;

		outputResult() ;

		return true ;
	}

	short HandsProcessor::compareHands() {


		Generator& bGen = getBoardGen() ; // also set Lo eligibility


		for(int* bSeq = bGen++ ; bSeq != 0 ; bSeq = bGen++) {
			Generator& hGen = getHandsGen() ;
			for(int* hSeq = hGen++ ; hSeq != 0 ; hSeq = hGen++) {
				// sequences check
	/*			cout << "before update comp : " << endl ;
				cout << "hands : " << hSeq[0] << " " << hSeq[1] << endl ;
				cout << "board : " << bSeq[0] << " " << bSeq[1] << " " << bSeq[2]  << endl ;
	*/
				udpateComparsion(hSeq, bSeq) ;
    			}
		}
		return 0 ;  // garbish value
	}


HandsProcessor::~HandsProcessor() {

	free(hand1) ;
	free(hand2) ;
	free(board) ;
	if (&in != &cin) { } ; //  close()
	if (&out != &cout) { } ; //  close()
}
