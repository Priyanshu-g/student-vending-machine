#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "printer.h"
#include "watcard.h"

#include <vector>

_Task Groupoff {
	Printer & prt;
	unsigned int numStudents, sodaCost, groupoffDelay;

	// std::vector<WATCard::FWATCard> giftCards {};
	// std::vector<unsigned int> listPermutation {};
	WATCard::FWATCard giftCards[numStudents];
	unsigned int listPermutation[numStudents];
	unsigned int targetGiftCard = 0;

	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard( unsigned int id );
};

#endif