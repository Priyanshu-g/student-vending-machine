#ifndef STUDENT_H
#define STUDENT_H

#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "bottlingplant.h"
#include "watcard.h"
#include "vendingmachine.h"

_Task Student {
	Printer & prt;
	NameServer & nameServer;
	WATCardOffice & cardOffice;
	unsigned int id, numPurchases; 
	unsigned int numDrank = 0, numFree = 0;
    BottlingPlant::Flavours favouriteFlavour;
	FWATCard watCard, giftCard;
	VendingMachine * vm;

	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};

#endif