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
	// Groupoff& groupoff; --> Call and receive FWATCard from groupoff, no need to store
	unsigned int id, numPurchases;
    BottlingPlant::Flavours favouriteFlavour;
	FWATCard watCard, giftCard;
	VendingMachine * vm;

	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
	~Student();
};

#endif