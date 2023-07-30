/*
 * PokerStructures.h
 *
 *  Created on: 16 Apr 2023
 *      Author: Assaf
 */

#ifndef POKERSTRUCTURES_H_
#define POKERSTRUCTURES_H_


#include<iostream> ;
#include <cstring>



using namespace std ;

typedef struct Card {

	enum r {ace = 1, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king}
	rank ;
	enum s {club = 1, diamond, heart, spade}
	suit ;

//	Card() { rank = ace ;   }  //   set default values as (1,1)

	Card(char rankk, char suitt) {

		switch(rankk) {
			case ('A') :
					rank = ace ;
					break ;
			case ('K') :
					rank = king ;
					break ;
			case ('Q') :
					rank = queen ;
					break ;
			case ('J') :
					rank = jack ;
					break ;
			case ('T') :
					rank = ten ;
					break ;

			default :
				//do not handle bad values
				rank =  static_cast<r>((int)(rankk - '0')) ;
				break ;
			}
		switch(suitt) {
			case ('d') :
					suit = diamond ;
					break ;
			case ('c') :
					suit = club ;
					break ;
			case ('h') :
					suit = heart ;
					break ;
			case ('s') :
					suit = spade ;
					break ;
			default :
					std::cerr << "bad card suit input !" << std::endl ;
					break ;
		}
	}

	// "<" operator overloading, enough for sorting related purposes of Card stl containers
	int operator<(const Card& right) {
		return rank < right.rank;
	}

	Card() {
	 memset(this, 0 , sizeof(Card)) ;
	}

	// comperator function for reverse card oredering

	bool revOrder (Card i,Card j) { return !(i<j) ; }

	static short cmp_ranks(r r1, r r2) {
		short x1,x2 ;
		if (r1 == ace) x1 = 14 ;
		else x1 = static_cast<short>(r1) ;
		if (r2 == ace) x2 = 14 ;
		else x2 = static_cast<short>(r2) ;
		if (x1 > x2) return 0 ;
		if (x1 < x2) return 1 ;
		return 2 ;

	}

} Card ;


/*
 * struct to hold statistics of a card sequence which are relevent to win Hi
 *
 */

typedef struct Stats {

	Card hiCard ;  // highest rank in the seq
	bool flush  ; // flush bit
	bool isConseq ; // true for a completely (i.e. poker straight manner)
	// below are in the contex of suits
	short nPairs ; // number of different pairs(only!) within
	bool has3,has4 ; // true if consist an exactly 3 / 4

	Stats() {
		memset(this, 0 , sizeof(Stats));
	}

} Stats ;

#endif /* POKERSTRUCTURES_H_ */
