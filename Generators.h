/*
 * Generators.h
 *
 *  Created on: 17 Apr 2023
 *      Author: Assaf
 */

/*
 * Genarators classes with unify API which generate all possible
 * distinct cards choices out of a card group
 */

#include <string.h>

class Generator {

	protected :

		bool exahust ; // true when finish generate sequences

	public :

		Generator() : exahust(false) {} ;

		// Postfix ++ op
		// should return an inline int[] that hold the next sequence element
		friend int*	operator++(Generator& g, int) {
			if (g.exahust) return 0 ;
			return g.next() ;
		}
		virtual int* next() = 0  ;
		bool iSExahust() {return exahust ; }
} ;


class Generator2ofn : public Generator {

	public :

		int vals[2] ;
		int n ;

		Generator2ofn(int nn) : n(nn) {

			if (nn < 2) throw (0) ;  // bad init value
			vals[0] = 0 ;
			vals[1] = 0 ;
		}
		int* next() {
			if (vals[1] < n-1) {
				++vals[1]  ;
			}
			else {
				vals[1] = ++vals[0] + 1 ;
			}
			if (vals[1] < n) return vals ;
			{exahust = true ; return 0 ; }
//			return ((vals[1] < 4) ? vals : 0)
		}
} ;

// based on Generator2ofn(5) instance - combinatorial motivation
class Generator3of5 : public Generator {

	Generator2ofn gen25 ;
	bool exclude[5] ;
	int vals[3] ; // inline result of a generated 3of5 unique sequence

	public :
	Generator3of5() : gen25(5) {}

	int* next() {
		memset((void*)exclude, 0 , 5 * sizeof(int)) ;
		int* tup = gen25++ ;
		if (tup == 0) return 0 ;  // finish generating
		// exclude
		exclude[tup[0]] = true ;
		exclude[tup[1]] = true ;
		int j = 0 ;
		for (int i = 0 ; i < 5 ; i++) {
			if (exclude[i] == 0) { // include i
				vals[j++] = i ;
			}
		}
		return vals ;
	}

} ;


