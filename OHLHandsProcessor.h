/*
 * OHLHandsProcessor.h
 *
 *  Created on: 16 Apr 2023
 *      Author: Assaf
 */

#include "HandsProcessor.h" ;
#include "PokerStructures.h" ;

#ifndef OHLHANDSPROCESSOR_H_
#define OHLHANDSPROCESSOR_H_


class OHLHandsProcessor : public HandsProcessor {

	static const int BIG_LOW  = 999999999 ;
public :

	// could be also protected

	// Lo data

	bool below8 ; // indicate if the given board is eligible for Lo winning
	int loWin  ; // BIG_LOW when not applicable in the round
	short winnerLow  ; // 0 left , 1 right, 2 N/A , 3 - both !

	// Hi data
	short hiRank ; // rank of current leading Hi score, 1-9 (according to POKER RANKINGS section) , other N/A
	short hiWin ; // current high winner, 0 - left , 1 - right , 2 - both
	vector<Card> hiCo; // highest combo so far

	OHLHandsProcessor( const istream& inn = cin , const ostream& outt = cout ) ;

	// process the next OMHL two hands and board input and appending results to the given output
	bool processNext() ;

	// outputs an (single) HiLo(!) hands comparison results to given output
	void outputResult() ;
private:

	char setDelimiter() ;

protected :

	void processToken(string& token, Card* cards, int numOfCards) ;

	Generator& getHandsGen()  ;
	Generator& getBoardGen()  ;

	// OMAHA Hi Lo implementation
	void udpateComparsion(int hSeq[] , int bSeq[]) ;

	// receive assumably 2 sorted 5 cards combos
	void udpateLo(vector<Card> comb1, vector<Card> comb2)  ;

	// receive assumably 2 sorted 5 cards combos
	void udpateHi(vector<Card> comb1, vector<Card> comb2)  ;

	// extract stats help method
	// precond : card combo is sorted, otherwise must sort within ...

	Stats extractStats(vector<Card> comb) ;

	short getRank(Stats stats) ;
};

#endif /* OHLHANDSPROCESSOR_H_ */
