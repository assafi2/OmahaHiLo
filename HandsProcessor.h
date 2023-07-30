/*
 * HandsProcessor.h
 *
 *  Created on: 16 Apr 2023
 *      Author: Assaf
 */

#include <istream> ;
#include <iostream> ; // can be excluded ?
#include <vector> ;
#include <string> ;

#include "PokerStructures.h" ;
#include "Generators.h" ;

using namespace std ;

#ifndef HANDSPROCESSOR_H_
#define HANDSPROCESSOR_H_

#endif /* HANDSPROCESSOR_H_ */


/*
 * an abstract base class for POKER HandProcessor
 * process inputs (from a given input) which include by default
 * two cards hands (groups) and a cards board (group)
 * to determine results and write it to given output
 */

class HandsProcessor{

	// DATA MEMBERS

//	static const int MAX_LINE_SIZE = 300 ;

	vector<string> tokens ;  // extracted from a current input line


	protected :

	const istream& in ; // input stream
	const ostream& out ; // output stream
//	char cur_line[MAX_LINE_SIZE] ;  // inline char buffer
	// other protected data members below ...

	public :

		HandsProcessor( const istream& inn  , const ostream& outt  , int handSize, int boardSize) ;

		// process all available inputs from the given input file and write their results in the given output file
		// return the number of successfully procced inputs
		virtual int processAll() ;

		// process the next two hands and board input and write the result in the output file
		// return true for a successfully processed input
		virtual bool processNext() ;

		// make a static version if necessary, eligible to use as a compartor function
		// in sorting algo of more that two hands, can apply static version as a friend

		// return "binary" values between 0-6 while first bit high winner,
		// 2nd bit low winner (both 0 for left winning), 3rd bit low validity (1 -> valid)
		virtual short compareHands() ;

		// outputs an(single) hands comparison results to given output
		virtual void outputResult()  = 0 ;

		// dest logic close either of the i/o files which is different from standard i/o
		virtual ~HandsProcessor() ;



	private :

		// game's relevent delimiter to tokenize an input line
		virtual char setDelimiter() = 0 ;

	protected :

		// DATA MEMBERS

		//arrays on heap
		Card* hand1 ;
		Card* hand2 ;
		Card* board ;
		const int handSize,boardSize ; // corresponding sizes in elements


		// process a string token into a suitable Card array
		virtual void processToken(string& token, Card* cards, int numOfCards) = 0  ; // for now we'll keep it pure virtual
		virtual Generator& getHandsGen() = 0 ;
		virtual Generator& getBoardGen() = 0 ;


		// update current comparison (process) results of two hands according to
		// specific possible sequences of both {hands and board}
		// hSec sequence choice to apply on both hands cards
		// qSeq sequence choice to apply on board cards
		virtual void udpateComparsion(int hSeq[] , int bSeq[]) = 0 ; // being pure virtual for now

};
