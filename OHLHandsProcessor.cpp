/*
 * OHLHandsProcessor.cpp
 *
 *  Created on: 16 Apr 2023
 *      Author: Assaf
 */

#include "OHLHandsProcessor.h" ;
#include <bits/stdc++.h> ;


OHLHandsProcessor::OHLHandsProcessor(const istream& inn ,const ostream& outt )
	: HandsProcessor(inn,outt,4,5) /* OHL size values */, loWin(BIG_LOW), winnerLow(2)/* N/A values */
	,below8(true) , hiRank(0)  { }

bool OHLHandsProcessor::processNext() {

	if (!HandsProcessor::processNext()) return false ;

	//clear necessary filed values
	loWin = BIG_LOW ;
	hiRank =1 ;

	return true ;
}

// returns a white space delimiter for input line tokenizing
char OHLHandsProcessor::setDelimiter() {return ' ' ;}  ;

void OHLHandsProcessor::processToken(string& token, Card* cards, int numOfCards) {

	// we assume input as well as a specific token are COMPLETLY VALID in all aspects
	int i  = 0 ; // cards index
	for (int j = 6 	/* exclude prefix */ ; j < token.length() ; ++j) {
		cards[i] = Card(token[j],token[j+1]) ;
		j += 2 ;
		i++ ;
	}
}

Generator& OHLHandsProcessor::getHandsGen()  {
	return *(new Generator2ofn(4)) ;
}

Generator& OHLHandsProcessor::getBoardGen() {

	// add logic to set board eligibility for Lo competition
	int loCount = 0 ;
	for(int i = 0 ; i < boardSize ; i++) {
		if (board[i].rank <= 8) loCount++ ;
	}
	if (loCount < 3) below8 = false ;

	// main part of the function
	return *(new Generator3of5()) ;
}

// OMAHA Hi Lo implementation
void OHLHandsProcessor::udpateComparsion(int hSeq[] , int bSeq[]) {



	vector<Card> comb1,comb2  ;
	// generate combo 1
	for(int i = 0 ; i < 2  ; ++i ) {
		comb1.push_back((hand1[hSeq[i]])) ;
	}
	for(int i = 0 ; i < 3  ; ++i ) {
		comb1.push_back((board[bSeq[i]])) ;
	}
	// generate combo 2
	for(int i = 0 ; i < 2  ; ++i ) {
		comb2.push_back((hand2[hSeq[i]])) ;
	}
	for(int i = 0 ; i < 3  ; ++i ) {
		comb2.push_back((board[bSeq[i]])) ;
	}
	// using "natural" overloaded "<" operator for Card
	sort(comb1.begin(),comb1.end()) ;
	sort(comb2.begin(),comb2.end()) ;

	if (below8) udpateLo(comb1, comb2) ;
	udpateHi(comb1, comb2) ;
}

// get rank logic
short OHLHandsProcessor::getRank(Stats st) {

	cout << "stats struct : " << endl ;
	cout <<  st.flush << " "  << st.has4 << " "  <<st.has3 <<   " " << st.nPairs <<
			" " << st.isConseq  << " "  << st.hiCard.rank << " "  <<  endl ;

	if ((st.flush == true) && (st.isConseq == true)) {
		return 9 ;
	}
	if (st.has4 == true) {cout << " got 4-of !!!! " << endl ; return 8 ;} ;
	if ((st.has3 == true) && (st.nPairs == 1)) {
		cout << "got full house !!! " << endl ;
		return 7 ;
	}
	if (st.flush) return 6 ;
	if (st.isConseq) return 5 ;
	if (st.has3) return 4 ;
	if (st.nPairs == 2) return 3 ;
	if (st.nPairs == 1) return 2 ;
	return 1 ;

}

// will be file scoped
// return a win high hand of identical ranks, 0 - left, 1 - right
// return 0 in case of a complete tie
// we assume same sized combos
static short breakEvenRanks(vector<Card> comb1, vector<Card> comb2, short rank) {

	// should contain logic for the different ranks ...
	// review rank descriptions for understanding
	short ret = 2 ; // return value

	switch (rank) {
		case(1): // complete tie !
			return 2 ;
			break ;
		case(2):
			// locate pair within each combo
			short p1r,p2r ; // pairs ranks
			for(int i = 0 ; i < comb1.size()-1 ; ++i ) {
				if (comb1[i].rank == comb1[i+1].rank) {
					p1r = static_cast<short>(comb1[i].rank) ;
				}
				if (comb2[i].rank == comb2[i+1].rank) {
					p2r = static_cast<short>(comb2[i].rank) ;
				}
			}
			if (p2r > p1r) ret = 1 ;
			if (p2r < p1r) ret = 0 ;
			break ;
		case(3):
		{
			short p11r = 0 ,p21r = 0 ,p12r = 0 ,p22r = 0  ; // pairs ranks
			for(int i = 0 ; i < comb1.size()-1 ; ++i ) {
				if (comb1[i].rank == comb1[i+1].rank) {
					if (p11r != 0)  p21r = static_cast<short>(comb1[i].rank) ;
					else p11r = static_cast<short>(comb1[i].rank) ;
				}
				if (comb2[i].rank == comb2[i+1].rank) {
					if (p12r != 0)  p22r = static_cast<short>(comb2[i].rank) ;
					else p12r = static_cast<short>(comb1[i].rank) ;
				}
			}
			// combos ordered asc
			if  (p21r > p22r) ret = 0 ;
			else if (p21r < p22r) ret = 1 ;
			// even high pairs
			else {
				if  (p11r > p12r) ret = 0 ;
				else if (p11r < p12r) ret = 1 ;
			}
			break ;
		}
		case(4):
			// locate 3io within each combo
			short t1r,t2r ; // trios ranks
			for(int i = 0 ; i < comb1.size()-2 ; ++i ) {
				if ((comb1[i].rank == comb1[i+1].rank) && (comb1[i+2].rank == comb1[i+1].rank))
				{
					t1r = static_cast<short>(comb1[i].rank) ;
				}
			}
			for(int i = 0 ; i < comb2.size()-2 ; ++i ) {
				if ((comb2[i].rank == comb2[i+1].rank) && (comb2[i+2].rank == comb2[i+1].rank))
					{
					t2r = static_cast<short>(comb2[i].rank) ;
					}
			}
			if (t2r > t1r) ret = 1 ;
			if (t2r < t1r) ret = 0 ;
			break ;

		case(5):
			short h1,h2 ; // highs
			if (static_cast<short>(comb1[0].rank) == 1) h1 = 14 ; // ace case
			else h1 = static_cast<short>(comb1[comb1.size()].rank) ;
			if (static_cast<short>(comb2[0].rank) == 1) h2 = 14 ; // ace case
			else h2 = static_cast<short>(comb2[comb2.size()].rank) ;
			if (h1 > h2) ret = 0 ;
			if (h1 > h2) ret = 1 ;
			break ;
		case(6):
			Card::s s1,s2 ;
			Card::r r1,r2 ;
			for(int i = 0 ; i < 3 ; ++i ) { // we assume flash (4 cards)
				if (comb1[i].suit == comb1[i+1].suit) {
					s1 = comb1[i].suit ;
				}
				if (comb2[i].suit == comb2[i+1].suit) {
					s2 = comb2[i].suit ;
				}
			}
			if (comb1[comb1.size()].suit == s1) r1 = comb1[comb1.size()].rank ;
			else r1 = comb1[comb1.size()-1].rank ;
			if (comb2[comb2.size()].suit == s2) r2 = comb2[comb2.size()].rank ;
			else r2 = comb2[comb2.size()-1].rank ;
			ret = Card::cmp_ranks(r1,r2) ;
			break ;

		case(7):
			// GET TRIOS RANKS FIRST
			// locate 3io within each combo
	//		short t1r,t2r ; // trios ranks
			for(int i = 0 ; i < comb1.size()-2 ; ++i ) {
				if ((comb1[i].rank == comb1[i+1].rank) && (comb1[i+2].rank == comb1[i+1].rank))
				{
					t1r = static_cast<short>(comb1[i].rank) ;
				}
			}
			for(int i = 0 ; i < comb2.size()-2 ; ++i ) {
				if ((comb2[i].rank == comb2[i+1].rank) && (comb2[i+2].rank == comb2[i+1].rank))
					{
					t2r = static_cast<short>(comb2[i].rank) ;
					}
			}
			if (t2r > t1r) {ret = 1 ; break ; }
			if (t2r < t1r) {ret = 0 ; break ; }
			// TIE TRIOS - get paris ranks
			// locate pair within each combo
	//		short p1r,p2r ; // pairs ranks
			for(int i = 0 ; i < comb1.size()-1 ; ++i ) {
				if (comb1[i].rank == comb1[i+1].rank) {
					p1r = static_cast<short>(comb1[i].rank) ;
				}
				if (comb2[i].rank == comb2[i+1].rank) {
					p2r = static_cast<short>(comb2[i].rank) ;
				}
			}
			if (p2r > p1r) ret = 1 ;
			if (p2r < p1r) ret = 0 ;
			break ;
		case(8):
			ret = Card::cmp_ranks(comb1[3].rank,comb2[3].rank) ;
			break ;
		default : // the case as well as for rank 9 which will lead to further comparison
			break ;
	}

	if (ret == 2) { // apply kicker ! // will work as well for rank 9
	return Card::cmp_ranks((comb1[comb1.size()].rank),comb2[comb2.size()].rank) ;
		}
		//		if (static_cast<short>(comb1[comb1.size()].rank) > static_cast<short>(comb2[comb2.size()].rank))
//				return 0 ;
//		if (static_cast<short>(comb1[comb1.size()].rank) < static_cast<short>(comb2[comb2.size()].rank))
//		return 2 ;
//   	return ret ; // complete tie ! return 2

	return ret ;
}



void OHLHandsProcessor::udpateHi(vector<Card> comb1, vector<Card> comb2) {


	// wish to iterate over sequence to deduce a relevant stats struct

	Stats st1 = extractStats(comb1) ;
	Stats st2 = extractStats(comb2) ;

	// get ranks using Stats obj

	short r1 ,r2 ; // ranks
/*
	cout << "combo1  : " << endl ;

	for(int i = 0 ; i < 5 ; i++){
		cout << comb1[i].rank << " "<< comb1[i].suit << endl ;
	}
*/
	r1 = getRank(st1) ;
/*
	cout << "combo2  : " << endl ;

	for(int i = 0 ; i < 5 ; i++){
		cout << comb2[i].rank << " "<< comb2[i].suit << endl ;
	}
*/
	r2 = getRank(st2) ;

	// SHOULD BE LESS VERBOSE WITH FUNCTIONS USE

	if (r1 < r2) {
		if (r2 > hiRank) {
			hiWin = 1 ; // player 2
			hiCo = comb2 ;
			hiRank = r2 ;
		}
		else if (r2 == hiRank) {
			short ret = breakEvenRanks(hiCo,comb2,hiRank) ;
			if (breakEvenRanks(hiCo,comb2,hiRank) == 1) {
				hiCo = comb2 ;
				hiWin = 1 ;
			}
			if ((ret == 2) && (hiWin == 0)) {hiWin = 2 ; } ; // split pot !!
		}
	} // r1 >= r2
	if (r1 > r2) {
		if (r1 > hiRank) {
			hiWin = 0 ;
			hiCo = comb1 ;
			hiRank = r1 ;
		}
		else if (r1 == hiRank) {
			short ret = breakEvenRanks(hiCo,comb1,hiRank) ;
			if (ret == 1) {
				hiCo = comb1 ;
				hiWin = 0 ;
			}
			if ((ret == 2) && (hiWin == 1)) {hiWin = 2 ; } ; // split pot !!
		}
	}
	else {  // r1 == r2
		short ret = breakEvenRanks(comb1,comb2,r1) ;
		if  (ret == 0)  {  // r1 against leader
					if (r1 > hiRank) {
						hiWin = 0 ;
						hiCo = comb1 ;
						hiRank = r1 ;
					}
					if (r1 == hiRank) {
						short ret = breakEvenRanks(hiCo,comb1,hiRank) ;
						if (ret == 1) {
							hiCo = comb1 ;
							hiWin = 0 ;
						}
						if ((ret == 2) && (hiWin == 1)) {hiWin = 2 ; } ; // split pot !!
						}
				}
		else { // r2 against leader
			if (r2 > hiRank) {
				hiWin = 1 ; // player 2
				hiCo = comb2 ;
				hiRank = r2 ;
			}
			else if (r2 == hiRank) {
				short ret = breakEvenRanks(hiCo,comb2,hiRank) ;
				if (ret == 1) {
					hiCo = comb2 ;
					hiWin = 1 ;
				}
				if ((ret == 2) && (hiWin == 0)) {hiWin = 2 ; } ; // split pot !!

			}
		}
	}

}


// extract stats help method
// precond : card combo is sorted, otherwise must sort within ...

Stats OHLHandsProcessor::extractStats(vector<Card> comb) {

	// compact enough to be treated by value (could be more but not necessary for us)
	static Stats st ;
	// mapping purposes, first cells at both stays empty
	static short ranks[14] ;
	static short suits[5] ;
	// zero inits
	memset(&st, 0 , sizeof(Stats));
	memset(ranks, 0 , sizeof(ranks));
	memset(suits, 0 , sizeof(suits));

	vector<Card>::iterator iter = comb.begin() ;
	st.hiCard.rank = comb[comb.size()-1].rank ; // asc sorted combo
	st.hiCard.suit = comb[comb.size()-1].suit ; // asc sorted combo
	st.isConseq = true ;
	for(Card &cur = *iter  ; iter != comb.end() ; cur = (*iter) ) {
		// mapping
		++ranks[static_cast<int>(cur.rank)] ;
		++suits[static_cast<int>(cur.suit)] ;

		if ((&(*iter))  != (&(comb[comb.size()-1]))) {
			++iter ;
			if ((((static_cast<int>(cur.rank)  != static_cast<int>((*(iter)).rank) - 1))
					&& !(((static_cast<int>(cur.rank) == 13 )) && (static_cast<int>((*(iter)).rank) == 1)))
					&& !(((static_cast<int>(cur.rank) == 1 )) && (static_cast<int>((*(iter)).rank) == 10)))
			{
			st.isConseq = false ;
			}
		}
		else ++iter ;
	}
/*
	cout << "post init ranks : " << endl ;

	for(int i = 0 ; i < 14 ; i++) {
		cout << ranks[i] ;
	}

	cout << endl ;
*/
	// complete the stats strcut
	for(int i = 0 ; i < 14 ; i++) {
		if (ranks[i] == 2) st.nPairs++ ;
		else if (ranks[i] == 3) st.has3 = true ;
		else if (ranks[i] == 4) st.has4 = true ;
	}
	for(int i = 0 ; i < 14 ; i++) {
		if (suits[i] == 5) st.flush = true ;
	}
	return st ;

}

void OHLHandsProcessor::udpateLo(vector<Card> comb1, vector<Card> comb2) {

	// Player 1 logic
	// player 2 duplicated code ! to be changed !
	
	vector<Card>::iterator iter = comb1.begin() ;
	bool valid = true ; // indicates for a valid sequence (i.e. no duplicates and below 8)
	int loNum = 0 ;
	int digit = 1 ;
	while (valid && (iter != comb1.end())) {
		if ((*iter).rank <= 8) {
			loNum += digit * ((*iter).rank) ;
		}
		else valid = false ;
		digit *= 10 ;
		if (iter != comb2.end())  // avoid dereferncing null for comparsion
			if ((*iter).rank ==  (*(++iter)).rank) {valid = false ; } // duplicate ranks
	}

	if (!valid) loNum = BIG_LOW ;
	if (loNum <= loWin) {

		if (!((loNum == loWin) && ((winnerLow == 0) || (winnerLow == 3)))) {
		 	  loWin = loNum ;
		 	  winnerLow = 0 ; // left hand player
		 	 }
		// ELSE KEEP THE SAME
	}

	// APPLY SAME LOGIC FOR PLAYER 2 COMBO , SHUOLD NOT BE DUPLICATED CODE !!!

	iter = comb2.begin() ;
	valid = true ; // indicates for a valid sequence (i.e. no duplicates and below 8)
	loNum = 0 ;
	digit = 1 ;
	while (valid && (iter != comb2.end())) {
		if ((*iter).rank <= 8) {
			loNum += digit * ((*iter).rank) ;
		}
		else valid = false ;
		digit *= 10 ;
		if (iter != comb2.end())  // avoid dereferncing null for comparsion
			if ((*iter).rank ==  (*(++iter)).rank) {valid = false ; } // duplicate ranks
	}

	if (!valid) loNum = BIG_LOW ;
	if (loNum <= loWin) {

		if (!((loNum == loWin) && ((winnerLow == 1) || (winnerLow == 3)))) {
		 	  loWin = loNum ;
		 	  winnerLow = 0 ; // left hand player
		 	 }
		// ELSE KEEP THE SAME
	}

}


void OHLHandsProcessor::outputResult() {


	string rank, hiRes ; // Hi results string

	switch(hiRank) {
		case (1) :
			rank = "(High Card)" ;
			break ;
		case (2) :
			rank = "(One Pair)" ;
			break ;
		case (3) :
			rank = "(Two Pair)" ;
			break ;
		case (4) :
			rank = "(3-of-a-Kind)" ;
			break ;
		case (5) :
			rank = "(Straight)" ;
			break ;
		case (6) :
			rank = "(Flush)" ;
			break ;
		case (7) :
			rank = "(Full House)" ;
			break ;
		case (8) :
			rank = "(4-of-a-Kind)" ;
			break ;
		case (9) :
			rank = "(Straight Flush)" ;
	}

	// could be easier with string format methods

	switch (hiWin) {
		case (0) :
			hiRes = "HandA wins Hi " ;
			break ;
		case (1) :
			hiRes = "HandA wins Hi " ;
			break ;
		case (2) :
			hiRes = "HandA wins Hi " ;
			break ;
		defult :
			hiRes = "INVALID output results for Hi" ;
			break ;
	}

	hiRes += rank ;

	// output Hi
	(ostream&)out << hiRes << '\t' ;

	switch(winnerLow) {
		case (0):
				(ostream&)out << "HandA Wins Lo " << loWin  << " \n"  ;
				break ;
		case (1) :
				(ostream&)out << "HandB wins Lo " << loWin  << " \n"  ;
				break ;
		case (2):
				(ostream&)out << "no hand qualified for Low \n"  ;
				break ;
		case (3) :
				(ostream&)out << "Split Pot Lo " << loWin  << " \n"  ;
				break ;
		default :
				(ostream&)out << "bad Lo result value  \n" ;
				break ;
	}
} ;


